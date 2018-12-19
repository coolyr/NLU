//author xhz105925
//date 2016/5/31

#include "pattern_engine.h"
#include <utility>
#include "string_util.h"
#include "../reader/config_reader.h"
#include "dm_request.h"
#include "../mylib/crfpp.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::vector;
using std::set;
using std::pair;
using std::map;
using pcrepp::Pcre;

DIALOG_SEG_LOG_SETUP(qp, PatternEngine);


PatternEngine::PatternEngine() {}
PatternEngine::~PatternEngine() {}
int PatternEngine::Configure(const char* conf_path)
{
	map<string, string> domain_xml;
	domain_xml["map"] = "pattern.MAP_PATH";			//./data_test/demo/map_template.xml
	domain_xml["music"] = "pattern.MUSIC_PATH";		//./data_test/demo/music_template.xml
	domain_xml["telephone"] = "pattern.CALL_PATH";	//./data_test/demo/call_template.xml
	/*
		pattern : {
			  MAP_PATH = "./data_test/demo/map_template.xml";
			  MUSIC_PATH = "./data_test/demo/music_template.xml";
			  CALL_PATH = "./data_test/demo/call_template.xml";
		};
	*/
	try
	{
		ConfigReader reader(conf_path);
		for (map<string, string>::iterator it = domain_xml.begin(); it != domain_xml.end(); it++)
		{
			string xml_path = reader.Get<string>(it->second);
			if (!xml_path.empty())
			{
				support_domains_.insert(it->first); //<1>	[map, telepthon, music]     calendar?
				if (!it->first.compare("map"))
				{
					domain_ptr_[it->first] = new DomainMap(); //<2>	map -> DomainMap()
				}
				else if (!it->first.compare("music"))
				{
					domain_ptr_[it->first] = new DomainMusic();
				}
				else if (!it->first.compare("telephone"))
				{
					domain_ptr_[it->first] = new DomainCall();
				}
				else
				{
					domain_ptr_[it->first] = new PatternDomain(); //其他？
				}
				xml_paths_[it->first] = xml_path;// <3>	"./data_test/demo/map_template.xml"
				LOG(INFO, "Support domain %s, template file: %s", it->first.c_str(), xml_path.c_str());
			}
			else
			{
				LOG(INFO, "Support domain map, but template path is emptyp");
				return -1;
			}
		}
	}
	catch (const libconfig::FileIOException &e)
	{
		LOG(INFO, "can not find configure file:%s", conf_path);
		return -1;
	}
	catch (const libconfig::ParseException &e)
	{
		LOG(INFO, "There is synx error in file, when parsing configure :%s", conf_path);
		return -1;
	}
	catch (...)
	{
		LOG(INFO, "There is synx error in file :%s", conf_path);
		return -1;
	}
	return 0;
}
bool PatternEngine::Matched(const string expr, const string stuff)
{
	Pcre pcre(expr);
	return pcre.search(stuff);
}

vector<string> PatternEngine::GetSubstr(const string expr, const string stuff)
{
	vector<string> substrs;
	Pcre pcre(expr);
	if (pcre.search(stuff) == true)
	{
		for (int pos = 0; pos < pcre.matches(); pos++)
		{
			substrs.push_back(pcre.get_match(pos));
		}
	}
	return substrs;
}

vector<pair<size_t, size_t> > PatternEngine::GetSubstrPos(const string expr, const string stuff)
{
	vector<pair<size_t, size_t> > begins_ends;
	Pcre pcre(expr);
	if (pcre.search(stuff) == true)
	{
		for (int pos = 0; pos < pcre.matches(); pos++)
		{
			begins_ends.push_back(std::make_pair(pcre.get_match_start(pos), pcre.get_match_end(pos)));
		}
	}
	return begins_ends;
}

int PatternEngine::Init(const char* conf_path)
{
	//support_domains_		[设置支持域 map,music,telephon,calendar缺少]
	//domain_ptr_			[map -> DomainMap] 
	//xml_paths_			[map -> 模板路径]
	if (Configure(conf_path) == -1)
	{
		LOG(INFO, "Init failed");
		return -1;
	}
	for (set<string>::iterator it = support_domains_.begin(); it != support_domains_.end(); it++)// [map,music,telepthon,(calendar沒有)]
	{
		string name = *it;
		if (xml_paths_.find(name) != xml_paths_.end())
		{
			PatternSlots* patt_slots = new PatternSlots;		//PatternSlots
			patt_slots->SetDomainName(name);
			PatternTemplates* patt_tmps = new PatternTemplates;	//PatternTemplates
			patt_tmps->SetDomainName(name);

			const char* xml = xml_paths_[name].c_str();
			//<slot name = "【路线】" weight = "high" synx = "路线|路径|路途|捷径" / >
			//<slot name="【导航】" weight="high" synx="导航|定位"/>
			int ret_s = patt_slots->LoadSlot(xml); // 加载 slot
			if (ret_s == -1)
			{
				LOG(INFO, "Init %s slot failed", name.c_str());
				return -1;
			}
			domains_slots_[name] = patt_slots; // map -> PatternSlots;
			/*
			<template id="ROUTE">
				<description>导航</description>
				<pattern type="EXACT">^(【我】)?(【想】)?【导航】$</pattern>
				<semantics>
					<domain>map</domain>
					<intent>route</intent>
				</semantics>
			</template>
			*/
			//@@@@@@@@@@@@@@@@@@@@@@@@@修改过@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			int ret_t = patt_tmps->LoadTemplates(xml); // 加载模板
			//@@@@@@@@@@@@@@@@@@@@@@@@@修改过@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

			if (ret_t == -1)
			{
				LOG(INFO, "Init %s template failed", name.c_str());
				return -1;
			}
			//设置map_domain_templates_    <map  -> set<PatternTemplates*>> 一个领域可以有多套模板
			GenMapDomainTemplates(name, patt_tmps);
		}
	}
	return 0;
}

//	通过实体识别结果调整query
//	seg_token_query = "我 想 听 singer 的 song"
void PatternEngine::ReConcatByNer(PatternRequest& request, vector<PatternResult>& results)
{
	//<1>	空格隔开的切词  seg_sen = "导航 去 五道口"
	StringUtils su;
	string seg_sen = ""; //空格隔开的切词  seg_sen = "导航 去 五道口"
	string raw = request.GetRawQuery();//raw_query
	string corr_query = request.GetCorrectedQuery();//corrected_query ?空
	const vector<QueryTerm>* terms = request.GetQueryTerm();//切词Term 列表
	for (vector<QueryTerm>::const_iterator it = terms->begin(); it != terms->end(); it++)
	{
		seg_sen += (it->GetText() + " ");// 空格隔开的切词  seg_sen = "导航 去 五道口"
	}
	seg_sen = su.Trim(seg_sen);
	//<2> 如果无 实体词，把每个MaxEnt分类结果DomainQpResult封装成相应的PatternResult对象【目前只有一个】
	const vector<QueryNeTerm>* neterms = request.GetQueryNeTerm();//实体词NeTerm列表
	if (neterms->empty())//如果无 实体词
	{
		const vector<QueryDomainQpResult>* domain_results = request.GetDomainResult();	//MaxEnt分类结果DomainQpResult 列表【目前只有一个】
		for (vector<QueryDomainQpResult>::const_iterator it = domain_results->begin(); it != domain_results->end(); it++)
		{
			/*
			message DomainQpResult {
				required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
				optional float domain_score = 2;	//MaxEnt得分
				repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后分类器填充 : 刘德华/singer   忘情水/song]
				required string ne_sentense = 4;	//MaxEnt未填充 ["我想听/o刘德华/singer的/o忘情水/song(/o)"]
			}
			*/
			string domain = it->GetDomain();		//域名
			float score = it->GetDomainScore();	//得分
			PatternResult result;				//与MaxEnt分类结果DomainQpResult对应
			result.SetRawQuery(raw);			//原始query  raw_query
			result.SetSegQuery(seg_sen);		//空格隔开的切词  seg_sen = "导航 去 五道口"
			result.SetDomain(domain);			//域名
			result.SetScore(score);				//得分
			result.SetHasNer(false);			//是否由 实体词
			result.SetSegTokenQuery(seg_sen);	//空格隔开的切词  seg_token_query = seg_sen = "导航 去 五道口"
			results.push_back(result);
		}//MaxEnt分类结果DomainQpResult 列表【目前只有一个】
		//###################################################################
		return;// 无实体词， 直接返回
		//###################################################################
	}//如果无 实体词


	//  for (vector<QueryNeTerm>::const_iterator it = neterm->begin(); it != neterm->end(); it++) {
	//    LOG(INFO, "netext:%s, type:%s, begin:%d, end:%d", it->GetNeText().c_str(), it->GetNeType().c_str(), it->GetBegin(), it->GetEnd());
	//  }

	//<3>	当有 实体词, 把每个MaxEnt分类结果DomainQpResult封装成相应的PatternResult对象【目前只有一个】
	const vector<QueryDomainQpResult>* domain_results = request.GetDomainResult();
	for (vector<QueryDomainQpResult>::const_iterator it = domain_results->begin(); it != domain_results->end(); it++)//MaxEnt分类结果DomainQpResult 列表【目前只有一个】
	{
		/*
		message DomainQpResult {
			required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
			optional float domain_score = 2;	//MaxEnt得分
			repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
			required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充 : "我想听/o刘德华/singer的/o忘情水/song(/o)"]
		}
		*/
		string ne_replace = raw;				//raw_query
		string concat_string = "";
		vector<pair<int, int> > replaced_pos;	//记录query中替换的实体的begin和end位置
		string domain = it->GetDomain();			//域名
		float score = it->GetDomainScore();		//得分
		string ne_sen = it->GetNeSentence();		//ne_sentense => MaxEnt未填充 [后处理器填充 : "我想听/o刘德华/singer的/o忘情水/song(/o)"]
		if (domain.empty() || score < 0.1)		//如果域名Domain为空，或者得分过低＜0.1
		{
			continue;	//遍历下一个分类域  => 【目前只有一个】
		}
		PatternResult result;
		result.SetRawQuery(raw);		//raw_query
		result.SetSegQuery(seg_sen);	//空格隔开的切词  seg_sen = "导航 去 五道口"
		result.SetDomain(domain);		//域名
		result.SetScore(score);			//得分
		result.SetNerQuery(ne_sen);		//ne_sentense => MaxEnt未填充 [后处理器填充 :"我想听/o刘德华/singer的/o忘情水/song(/o)"]

		//<4> 当有 实体词, 遍历当前分类domain内的实体词, 原始query中的 【实体】 换成 【实体POI】   => "我想听singer的song"
		const vector<QueryNeTerm>* neterms = it->GetNeTerms();	//后处理器获取的当前domain的实体词
		for (vector<QueryNeTerm>::const_iterator ii = neterms->begin(); ii != neterms->end(); ii++)//遍历当前分类domain内的实体词
		{
			/*
			message NeTerm{
				required string ne_type = 1;	//实体类别
				required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
				required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
				required uint32 end = 4;		//词的结束位置,在原始query中的.
			}
			*/
			string text = ii->GetNeText();	// 刘德华  => 后处理器获得的DomainQpResult中的实体
			string type = ii->GetNeType();	// singer
			int begin = ii->GetBegin();		// begin
			int end = ii->GetEnd();			// end
			//######################################################################
			result.AddNersKV(type, text);	// <type, text>  => <singer, 刘德华>
			result.AddNersPair(type, text); // <type, text>  => <singer, 刘德华>
			//######################################################################
			result.SetHasNer(true);			// 设置有实体

			if (ne_replace.find(text) != string::npos)
			{
				//su.Replace(ne_replace, text, "【"+type+"】");
				su.Replace(ne_replace, text, type); //原始query中的 【实体】 换成 【实体POI】   => "我想听singer的song"
			}
			replaced_pos.push_back(std::make_pair(begin, end)); //记录query中替换的实体的begin和end位置
		}

		//<5>  当有 实体词, 遍历 切词Term 列表, 把每个切词term两边加上空格
		for (vector<QueryTerm>::const_iterator it = terms->begin(); it != terms->end(); it++)//切词Term 列表
		{
			/*
			message Term {
				required string text = 1;		//切词
				required string pos = 2;		//词性
				required uint32 begin = 3;      //词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开
				required uint32 end = 4;		//词的结束位置,在原始query中的.
				optional float weight = 5;      //term权重 [切词时未填充]
			}
			*/
			bool flag = true;
			string text = it->GetText();			//切词
			int beg = it->GetBegin();			//词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开
			int end = it->GetEnd();				//词的结束位置,在原始query中的.
			for (vector<pair<int, int> >::iterator ie = replaced_pos.begin(); ie != replaced_pos.end(); ie++)//记录query中替换的实体的begin和end位置
			{
				if (beg >= ie->first && end <= ie->second) //  <Ne.B <word.B, word.E> Ne.E>  《实体词 包含 切词》
				{
					flag = false;//当前切词 被 实体POI 替换了   刘德华 => singer
				}
			}//记录query中替换的实体的begin和end位置

			if (flag)//如果当前Term 没有被 实体POI【singer】替换 => ["我"， “想”， “听”， “的”]
			{
				size_t pos = 0;
				if ((pos = ne_replace.find(text)) != string::npos)//找到切词， 两边加空格
				{
					ne_replace.replace(pos, text.length(), " " + text + " ");//"我想听singer的song" ==>  "我 想 听 singer 的 song"
				}
			}
		}//切词Term 列表

		//<6>当有 实体词, 把所有的两个空格替换为一个空格
		size_t rpos = 0;
		while ((rpos = ne_replace.find("  ")) != string::npos)//找到两个空格
		{
			ne_replace.replace(rpos, 2, " ");//替换成一个空格
		}
		result.SetSegTokenQuery(ne_replace); //"我 想 听 singer 的 song"
		results.push_back(result);
	}//MaxEnt分类结果DomainQpResult 列表【目前只有一个】
}



/*
message QpResult {
	required string raw_query = 1;					//原始query = QpRequest.query
	optional string corrected_query = 2;			//PostProcesser处理完后，该字段还没有填充！？
	repeated Term terms = 3;						//检索粒度切词 【WordSegmenter类完成】
	repeated NeTerm ne_terms = 4;					//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
	repeated DomainQpResult domain_qp_results = 5;	//MaxEnt分类结果
}
message DomainQpResult {
	required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
	optional float domain_score = 2;	//MaxEnt得分
	repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
	required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充 :"我想听/o刘德华/singer的/o忘情水/song(/o)"]
}
*/
int PatternEngine::Process(QpResult* qp_result, NluResult& nlu_result, bool is_direct)
{
	//<1>	QpResult封装成PatternRequest对象request
	StringUtils su;
	PatternRequest request;			//封装QpResult的Bean对象
	request.InputInfo(*qp_result);	//QpResult封装成PatternRequest对象request
	//################################################################################
	//<2>	如果是 “直接匹配” 模式，则进行直接匹配
	//默认 is_direct = False
	if (is_direct)
	{
		//<1>	获取原始query
		string raw_query = request.GetRawQuery();

		//<2>	进行直接匹配
		int ret = DirectMatch(raw_query, results_);
		//if (ret != 0)
		//{
		//	return -1;
		//}
		if (ret == 0)//直接匹配成功
		{
			//<3>	生成nlu_result对象
			FormateNluResult(results_, nlu_result);

			//<4>	清空PatternResult列表
			ClearRecommendDomain();//无用？
			ClearResults();
			return 0;

		}
		else
		{
			//直接匹配失败，清空results_重新匹配
			results_.clear();
		}
	}
	//################################################################################

	//<3>	通过实体识别结果调整query   [seg_token_query = "我 想 听 singer 的 song"]
	vector<PatternResult> patt_results;
	ReConcatByNer(request, patt_results);//通过实体识别结果调整query   [seg_token_query = "我 想 听 singer 的 song"]
	
	//<4>	遍历PatternResult对象，匹配意图和抽取slot
	for (vector<PatternResult>::iterator it = patt_results.begin(); it != patt_results.end(); it++)//遍历PatternResult， MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】
	{
		size_t cnt = 0;	//不支持的domain计数
		string synx_token = it->GetSegTokenQuery();	//<6>	"我 想 听 singer 的 song" 【当无实体词时==> 空格隔开的切词  seg_token_query = seg_sen = "今天 天气 很 好"】
		string reco_domain = it->GetDomain();		//域名 [map,music,telepthon,(calendar沒有)]
		//LOG(INFO, "domain: %s", reco_domain.c_str());
		if (domains_slots_.find(reco_domain) == domains_slots_.end()) //domains_slots_ ： map -> PatternSlots
		{
			cnt++;	//不支持的domain计数
			LOG(INFO, "Unknow domain:%s", reco_domain.c_str());
			if (cnt == (patt_results.size() - 1))
			{
				return -1;//如果没有其余的PatternResult => 目前只有一个
			}
			continue;//继续循环 PatternResult 【MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】】
		}

		//<5>	对实体替换后的切词列表进行slot归一化，并将归一化后的 非实体 <归一slot， slot>添加到slots_pair里
		// LOG(INFO, "synx_token:::::%s", synx_token.c_str());
		// slots_pair 里面加载的是非实体？  => [<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]    <= "我 想 听 singer 的 song"
		// 【注意：真实情况【我】是去掉中括号的】
		// it =  PatternResult   MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】
		NormallizeBySlot(*it, synx_token);//synx_token=seg_token_query="我 想 听 singer 的 song"  【当无实体词时==> 空格隔开的切词  seg_token_query = seg_sen = "今天 天气 很 好"】


		//<6> 将seg_token_query替换为slot归一化后的 synx_token=seg_token_query="【我】 【要】 【听】 【singer】 【的】 【song】"
		it->SetSegTokenQuery(synx_token); //synx_token=seg_token_query= "【我】 【要】 【听】 【singer】 【的】 【song】"
		synx_token = su.Trim(synx_token);


		//<7>	进行模板匹配
		//slots_kv 添加意图对象 <intent, play_instrument> <domain, music>
		// it = PatternResult
		MatchTemplate(*it, synx_token);//synx_token=seg_token_query= "【我】 【要】 【听】 【singer】 【的】 【song】"

		//<8>	获取相应Domain的PatternDomain对象，对模板匹配的结果进行AdjustSlot => 把识别出实体和归一化后的slot对添加到分别添加到ners_kv 和 slots_kv
		PatternDomain* pdomain = domain_ptr_[reco_domain];
		pdomain->AdjustSlot(*it);// it = PatternResult

		//<9>	添加到匹配的结果PatternResult中
		results_.push_back(*it);
	}//遍历PatternResult，MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】

	//<10>	生成nlu_result对象
	FormateNluResult(results_, nlu_result);//生成 nlu_result

	//<11>	清空PatternResult列表
	ClearRecommendDomain();//??无用
	ClearResults();
	return 0;
}


//synx_token=seg_token_query="我 想 听 singer 的 song"  【当无实体词时==> 空格隔开的切词  seg_token_query = seg_sen = "今天 天气 很 好"】
//归一化slot
void PatternEngine::NormallizeBySlot(PatternResult& result, string& synx_token)
{
	//<1>	获取实体词替换后的词列表[‘我’， ‘想’， ‘听’， ‘singer’， ‘的’， ‘song’]
	StringUtils string_util;
	vector<string> words;
	string_util.Split(synx_token, words, " ");	// words = [我, 想, 听, singer, 的, song]

	//<2>	获取相应MaxEnt分类的domain中的synx2pcre => <synx, prce(synx)>  =>  <synx="(想)?要|想" , Pcre(synx)>
	string reco_domain = result.GetDomain();	// MaxEnt域名
	//LOG(INFO, "domain: %s", reco_domain.c_str());
	const map<string, Pcre*>* synx_pcre_ptr = domains_slots_[reco_domain]->GetSynxPcre();//synx2pcre => <synx, prce(synx)>  =>  <synx="(想)?要|想" , Pcre(synx)>
	size_t pos = 0;

	//<3>	遍历切词 “[我, 想, 听, singer, 的, song]” 进行Normallize操作
	for (vector<string>::iterator ii = words.begin(); ii != words.end(); ii++)//遍历切词 “[我, 想, 听, singer, 的, song]”
	{
		
		//<4>	遍历所有的正则<synx, prce(synx)>
		//LOG(INFO, "word: %s", (*ii).c_str());
		for (map<string, Pcre*>::const_iterator iit = synx_pcre_ptr->begin(); iit != synx_pcre_ptr->end(); iit++)//遍历正则<synx, prce(synx)>
		{
		
			//<5>	如果找到匹配的正则Pcre =>  Pcre匹配当前word   <synx = "(想)?要|想", Pcre(synx)>
			if (iit->second->search(*ii))//如果正则 Pcre匹配当前word   <synx = "(想)?要|想", Pcre(synx)>
			{
				//<6>	获取当前word归一化后的name =>【要】

				//LOG(INFO, "synx :%s, word:%s", iit->first.c_str(), (*ii).c_str());
				//<slot name = "【我】" synx = "我|朕|me|偶" / >
				//<slot name="【要】" synx="(想)?要|想"/>
				string slot_id = domains_slots_[reco_domain]->GetSynx2Slot()->at(iit->first).GetName(); //获取当前word归一化后的name =>【要】


				//<7>	当前word归一化  "【我】 【要】 听 singer 的 song"
				if ((pos = synx_token.find(*ii, pos)) != string::npos)//从pos位置查找当前 (word=*ii)
				{
					synx_token.replace(pos, (*ii).length(), slot_id);//当前word归一化  "【我】 【要】 听 singer 的 song"
					pos += slot_id.length();//pos后移到下一个词
				}

				//<8>	去掉归一化后的slot中的【】  =>  "【要】" -> "要"
				const map<string, string>* ner_map_ptr = result.GetNersMap();// ners_kv = 后处理器获得的DomainQpResult中的实体 <type, text>  => <singer, 刘德华>
				string slot_key = slot_id;// “【要】”
				if (slot_id.find_first_of("【") != string::npos && slot_id.find_last_of("】") != string::npos)
				{
					slot_key = slot_id.substr(3, slot_id.length() - 6);// slot_key = 要
				}
				/*
					<slot name="【singer】" weight="high" synx="singer"/>
					<slot name="【song】" weight="high" synx="song"/>
					<slot name="【loc】" weight="high" group_name="location" synx="poi|loc"/>
					<slot name="【person】" synx="per|poi|loc"/>0
				*/

				//<9>	如果当前slot非实体, 添加到slots_pair  <=  [<"【我】, 我">, <"【要】, 想">] 【注意：真实情况【我】是去掉中括号的】
				//#################################################################################################
				if (ner_map_ptr->find(slot_key) == ner_map_ptr->end())//如果非实体
				{
					if (!ii->compare("loc") || !ii->compare("poi")) // (*ii=word)  为 loc 或者 poi ?不是非实体吗？
					{
						continue;//继续循环
					}
					// slots_pair 里面加载的是非实体？？  => [<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]  【注意：真实情况【我】是去掉中括号的】
					result.AddSlotsPair(slot_key, *ii);// vector<std::pair<std::string, std::string>> slots_pair  <=  [<"【我】, 我">, <"【要】, 想">] 【注意：真实情况【我】是去掉中括号的】
				}
				//#################################################################################################
			}//如果正则 Pcre匹配当前word
		}//遍历正则<synx, prce(synx)>
	}//遍历切词 “[我, 想, 听, singer, 的, song]”
}

/*
 0:keep, 1:change, -1:conflict
 规则 ：			
 匹配type	>	设置ltype		True
 匹配weight	>	设置weght		True

 weight	:	越大越好
 type	:	EXACT>lOCAL>FUZZY

 FUZZY	:	模糊匹配的（指的也是正则匹配，但是规则稍微放松些），
 LOCAL	:	带上下文的匹配
 EXACT	:	严格正则匹配

 */
int  PatternEngine::TemplatePriority(PatternResult& result, const string type, const size_t weight)
{
	/*
	result	:	PatternResult对象
	type	:	匹配的正则类型 【"EXACT" > "FUZZY" > "LOCAL"】
	weight	:	匹配的模板权重，匹配粒度 => 通过template id 计算
	*/
	string ltype = result.GetPatternType();//设置的匹配类型 => [初始化?]
	if (ltype.empty())
	{
		ltype = "FUZZY";//“模糊”
	}
	if (!type.compare(ltype))// 匹配正则的 type = 设置的 ltype
	{
		size_t lweight = result.GetWeight();//设置的权重，默认初始化为0
		if (lweight > weight)//设置的权重 > 模板的权重			False
		{
			return 0;
		}
		else if (lweight < weight)//设置的权重 < 模板的权重		True
		{
			return 1;
		}
		else
		{
			LOG(ERROR, "There are synx conflict!!!!");			//False
			return -1;
		}
	}
	else// 匹配的正则的 type != 设置的 ltype			级别 	=>          EXACT  >  LOCAL  >  FUZZY
	{
		if (!ltype.compare("EXACT") && type.compare("EXACT"))		//设置['EXACT']  and  匹配['FUZZY', 'LOCAL']		False
		{
			return 0;
		}
		//################################################目前设置为空 ‘FUZZY’######################################################
		else if (!ltype.compare("FUZZY") && !type.compare("EXACT"))	//设置['FUZZY']  and  匹配['EXACT']		True
		{
			return 1;
		}
		else if (!ltype.compare("FUZZY") && !type.compare("LOCAL"))	//设置['FUZZY']  and  匹配['LOCAL']		True
		{
			return 1;
		}
		//################################################目前设置为空 ‘FUZZY’#######################################################
		else if (!ltype.compare("LOCAL") && !type.compare("EXACT"))	//设置['LOCAL']  and  匹配['EXACT']		True
		{
			return 1;
		}
		else if (!ltype.compare("LOCAL") && !type.compare("FUZZY"))	//设置['LOCAL']  and  匹配['FUZZY']		False
		{
			return 0;
		}
	}
}

//synx_token=seg_token_query= "【我】-【要】-【听】-【singer】-【的】-【song】"
// - 空格
//slots_kv添加意图对象 <intent, play_instrument> <domain, music>
void PatternEngine::MatchTemplate(PatternResult& result, string& synx_token)
{
	LOG(INFO, ",,,,,synx,,,,,:%s", synx_token.c_str());\

	//<1>	查找领域模板PatternTemplates【支持多个模板，目前只有一份】
	map<string, set<PatternTemplates*> >::iterator irt = map_domain_templates_.find(result.GetDomain());
	if (irt != map_domain_templates_.end())//有该domain的模板
	{
		//<2>	遍历当前domain的模板PatternTemplates 【支持多个模板，目前只有一份】
		// iri -> PatternTemplates
		for (set<PatternTemplates*>::iterator iri = irt->second.begin(); iri != irt->second.end(); iri++)//遍历当前domain的模板PatternTemplates 【支持多个模板，目前只有一份】
		{

			//<3>	遍历当前模板templates的正则Prce(synx)    => map_synx_pcre_
			// synx   "^(【我】)?(【想】)?【导航】$" -> Pcre(synx)
			for (map<string, Pcre*>::const_iterator itt = (*iri)->GetMapSynxPcre()->begin(); itt != (*iri)->GetMapSynxPcre()->end(); itt++)//遍历当前模板templates的正则Prce
			{
				//<4>	找到匹配的正则Prce(synx)
				size_t weight = 0;
				//	itt = <synx, Prce(synx)>
				//	<pattern type = "FUZZY">【如何】 【去】 【loc】< / pattern>
				//	synx_token = seg_token_query = "【我】-【要】-【听】-【singer】-【的】-【song】"
				if (itt->second->search(synx_token))//正则匹配 ,可能匹配到多个，然后根据weight和type进行选择
				{
					LOG(INFO, "matched_query by %s", itt->first.c_str());


					//<5>	获取当前正则Prce(synx)所在的模板  => map_synx_template_
					//<template id="ROUTE_ARRIVAL">
					PatternTemplate patt_temp = (*iri)->GetTemplate(itt->first); //iri -> Templates


					//<6>	获取当前正则的类型Type
					//<pattern type="EXACT">^(【我】)?(【想】)?【导航】$</pattern>
					/*
					EXACT	=>	精确匹配【全匹配】
					LOCAL	=>	?
					FUZZY	=>	?
					*/
					string type = patt_temp.GetSynxType(itt->first);//["EXACT" "LOCAL" "FUZZY"]	   <==  <pattern type= "EXACT">^【play】 song$</pattern>
					LOG(INFO, "type:%s", type.c_str());


					//<7>	通过上下文获得最佳匹配
					// temp adjust the best result by context
					size_t pos = 0;

					//<8>	获取Template的id【可以看成是意图id】
					//<template id="ROUTE">
					const string tid = patt_temp.GetId();// [MUSIC,  MUSIC_SONG,  MUSIC_COUNTRY_TYPE]

					//<9>	通过id计算匹配的意图权重weight
					while ((pos = tid.find("_", pos + 1)) != string::npos)
					{
						weight++;//权重越高，匹配粒度越细，意图越明确
					}


					//<10>	判断匹配到的Priority【weight，type】是否比设置的高
					//LOG(INFO, "weight::::%s, %d",tid.c_str(), weight);
					/*
						result	:	PatternResult对象
						type	:	匹配的正则类型 【"EXACT" "LOCAL" "FUZZY"】
						weight	:	匹配的模板权重，匹配粒度 => 通过template id 计算
					*/
					int ret = TemplatePriority(result, type, weight);// 0:keep, 1:change, -1:conflict

					if (ret == 0 || ret == -1)
					{
						continue;//重新匹配
					}
					else if (ret == 1)//匹配成功
					{
						//<11>	遍历语义哈希，添加匹配到的意图kv 和 匹配类型type
						result.SetPatternType(type);
						map<string, string> semantic_map = patt_temp.GetMapSemantics();
						for (map<string, string>::iterator im = semantic_map.begin(); im != semantic_map.end(); im++)
						{
							//LOG(INFO, "sem_key:%s, sem_value:%s", im->first.c_str(), im->second.c_str());
							/*
							<semantics>
								<domain>music</domain>
								<intent>play_instrument</intent>
							</semantics>

							<domain, music>
							<intent, play_instrument>
							*/
							result.AddSlotsKV(im->first, im->second);//slots_kv添加意图对象 <intent, play_instrument> <domain, music>
						}

					}//匹配成功
				}//正则匹配 ,可能匹配到多个，然后根据weight和type进行选择
			}//遍历当前模板的正则Prce
		}//遍历当前domain的模板 【支持多个模板，目前只有一份】
	}//有该domain的模板
}


/*
<template id="BOOOK_FLIGHT">
	<description>订机票</description>
	<pattern type="EXACT" rule="订一张@<date>去@<loc>的机票">^订一张.*去.*的机票$</pattern>
	<semantics>
		<domain>map</domain>
		<intent>book_flight</intent>
	</semantics>
</template>

*/
int PatternEngine::DirectMatch(string query, vector<PatternResult> & results)
{
	size_t domain_cnt = 0;
	//<1>	遍历 所有领域 <map->set<PatternTemplates*>>
	for (map<string, set<PatternTemplates*> >::iterator iter_tms_set = map_domain_templates_.begin(); iter_tms_set != map_domain_templates_.end(); iter_tms_set++)//遍历 所有领域 <map->set<PatternTemplates*>>
	{
		//<2>	遍历一个特定领域【map】可以有多套模板
		domain_cnt++;
		// iter_domain_tems : PatternTemplates
		for (set<PatternTemplates*>::iterator iter_domain_tms = iter_tms_set->second.begin(); iter_domain_tms != iter_tms_set->second.end(); iter_domain_tms++)// 遍历一个特定领域【map】可以有多套模板 
		{
			//<3>	遍历 当前模板的所有正则Pcre(synx)
			for (map<string, Pcre*>::const_iterator iter_synx_pcre = (*iter_domain_tms)->GetMapSynxPcre()->begin(); iter_synx_pcre != (*iter_domain_tms)->GetMapSynxPcre()->end(); iter_synx_pcre++)//遍历 当前模板的所有正则Pcre(synx)
			{
				//<4>	从Templates里检测到不存在rule,直接pass [虽然通过Hash检测，但是可以完善算法！]
				//		<pattern type = "EXACT" rule="订一张@<date>去@<loc>的机票">^订一张.*去.*的机票$< / pattern>
				//		自定义	map_synx_rule_
				if (((*iter_domain_tms)->GetMapSynxRule())->find(iter_synx_pcre->first) == (*iter_domain_tms)->GetMapSynxRule()->end())
				{
					continue;
				}
				//<5>	如果当前正则Pcre(synx)匹配成功
				//		itt = <"^订一张.*去.*的机票$" ,  Pcre(synx)>
				if (iter_synx_pcre->second->search(query))//如果当前正则Pcre(synx)匹配成功
				{
					LOG(INFO, "matched_query by %s", iter_synx_pcre->first.c_str());

					//<6>	获取当前synx所在模板PatternTemplate
					PatternTemplate patt_temp = (*iter_domain_tms)->GetTemplate(iter_synx_pcre->first);
					
					//<7>	获取当前正则的类型Type
					string type = patt_temp.GetSynxType(iter_synx_pcre->first);//["EXACT" "LOCAL" "FUZZY"]	   <==  <pattern type= "EXACT">^【play】 song$</pattern>
					
					//<8>	直接匹配必须是：EXACT 严格正则
					if (type.compare("EXACT") != 0 && domain_cnt < map_domain_templates_.size())
					{
						continue;
					}

					//<9>	组装PatternResult
					PatternResult result;
					string domain_name = iter_tms_set->first;//Domain名
					result.SetPatternType(type);	//匹配到的正则的type  = [EXACT, FUZZY, LOCAL]
					result.SetRawQuery(query);		//原始query  raw_query
					result.SetDomain(domain_name);	//Domain名
					result.SetScore(1.0);			//[MaxEnt]得分
					result.SetHasNer(false);		//是否由 实体词
					result.SetSlotNum(0);			//设置slot数   =>  ?重设？
					map<string, string> semantic_map = patt_temp.GetMapSemantics();
					/*
					<semantics>
						<domain>music</domain>
						<intent>play_instrument</intent>
					</semantics>

					<domain, music>
					<intent, play_instrument>
					*/
					//<10>	遍历并添加匹配到的意图
					for (map<string, string>::iterator iter_map = semantic_map.begin(); iter_map != semantic_map.end(); iter_map++)
					{
						// LOG(INFO, "sem_key:%s, sem_value:%s", im->first.c_str(), im->second.c_str());
						result.AddSlotsKV(iter_map->first, iter_map->second);
					}

					//<11>	根据rule抽取slot
					string rule = patt_temp.GetSynxRule(iter_synx_pcre->first);
					map<string, string> slot_map;
					//###########################GetSlotUseRule###########################
					bool bExtraction = patt_temp.GetSlotUseRule(query, rule, slot_map);
					//###########################GetSlotUseRule###########################
					if (!bExtraction)
					{
						continue;//抽取失败
					}
					//<12>	遍历并添加rule抽取的slot信息
					for (map<string, string>::iterator iter = slot_map.begin(); iter != slot_map.end(); iter++)
					{
						result.AddSlotsKV(iter->first, iter->second);
					}

					//<12>	利用PatternDomain调整slot信息 ?? => domain不是支持的领域？
					//##############################################################
					PatternDomain* pdomain = domain_ptr_[domain_name];
					pdomain->AdjustSlot(result);
					//##############################################################

					//<13>	添加到结果results
					results.push_back(result);
				}
			}
		}
	}

	if (results.size() == 0)
	{
		return -1;
	}
	return 0;
}

void PatternEngine::AddSupportDomain(string domain)
{
	support_domains_.insert(domain);
}

void PatternEngine::AddRecommendDomain(string domain)
{
	recommend_domains_.push_back(domain);
}

PatternSlots* PatternEngine::GetSlotsByDomain(string domain)
{
	return domains_slots_[domain];
}
/*
message NluResult {
	required string query = 1;
	optional string seg = 2;
	repeated Domain domains = 3;  //领域 【list】
}
*/
/*
FormateNluResult(results_, nlu_result);
*/
void PatternEngine::FormateNluResult(vector<PatternResult>& patt_results, NluResult& nlu_result)
{
	for (vector<PatternResult>::iterator it = results_.begin(); it != results_.end(); it++)//遍历PatternResult， MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】
	{
		//it = > PatternResult
		it->ScoreIntent();							//意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
		DmRequest drequest;							//  <=> NluResult
		drequest.SetQuery(it->GetRawQuery());		//原始query  raw_query
		drequest.SetSeg(it->GetSegQuery());			//空格隔开的切词  seg_sen = "导航 去 五道口"
		DmDomain domain;
		domain.SetName(it->GetDomain());				//域名
		domain.SetDomainScore(it->GetScore());		//[分类]得分

		//#######################################################################################################################
		DmIntent intent;
		intent.SetName(it->GetSlotByKey("intent"));	//意图名:	play_instrument   
		//slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		intent.SetScore(it->GetIntentScore());		//意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
		DmSlot slot;
		//slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		for (map<string, string>::const_iterator ii = it->GetSlotMap()->begin(); ii != it->GetSlotMap()->end(); ii++)//slots_kv
		{
			string key = ii->first;					//【我】，  intent
			string value = ii->second;				// 我 ，    play_instrument
			slot.AddKeyValue(key, value);
		}//slots_kv

		//<type, text>  ners_kv = <singer, 刘德华>
		for (map<string, string>::const_iterator ii = it->GetNersMap()->begin(); ii != it->GetNersMap()->end(); ii++)//ners_kv
		{
			string key = ii->first;					//loc,		singer
			string value = ii->second;				//五道口，	刘德华
			slot.AddKeyValue(key, value);
		}//ners_kv

		intent.SetSlot(slot);			//itent里slot【唯一】
		domain.AddIntent(intent);		//domain可以有【多个】intent
		//########################################################################################################################
		drequest.AddDomain(domain);		//DmRequest(NluResult)可以有【多个】domain
		drequest.OutputInfo(nlu_result);//drequest内容填充到nlu_result
	}//遍历PatternResult， MaxEnt分类结果DomainQpResult解析成PatternResult【目前只有一个】
}

void PatternEngine::ClearRecommendDomain()
{
	recommend_domains_.clear();
}

void PatternEngine::ClearResults()
{
	results_.clear();
}

void PatternEngine::GenMapDomainTemplates(string domain, PatternTemplates* patt_tmps)
{
	//map_domain_templates_    <map  -> set<PatternTemplates*>> 一个领域可以有多套模板
	if (map_domain_templates_.find(domain) != map_domain_templates_.end())
	{
		map_domain_templates_[domain].insert(patt_tmps);
	}
	else
	{
		set<PatternTemplates*> set_tmp_ptr;//set集合，可以有多个PatternTemplates
		set_tmp_ptr.insert(patt_tmps);
		map_domain_templates_[domain] = set_tmp_ptr;
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
<template id="BOOK_AIR_TICKET">
	<pattern type="EXACT" rule="订一张@<date>去@<loc>的机票">^订一张.*去.*的机票$</pattern>
	<semantics>
		<domain>map</domain>
		<intent>book_air_ticket</intent>
	</semantics>
</template>
*/
int PatternEngine::ModifyTemplatesByRule(const std::string domain, const std::string intent, const std::set<std::string> rules)
{
	if (support_domains_.find(domain) == support_domains_.end())
	{
		LOG(INFO, "not support domain => %s", domain);
		return -1;
	}

	map<string, set<PatternTemplates*> >::iterator iter_tmps_set = map_domain_templates_.find(domain);
	if (iter_tmps_set != map_domain_templates_.end())
	{
		LOG(INFO, "not find patternTemplates");
		return -1;
	}

	set<PatternTemplates*>::iterator iter_tmps = iter_tmps_set->second.begin();//获取第一个PatternTemplates
	int ret = (*iter_tmps)->AddTemplatesByRule(domain, intent, rules);
	if (ret != 0)
	{
		LOG(INFO, "add patternTemplate error");
		return -1;
	}

	return 0;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


void PatternEngine::Destroy()
{
	for (map<string, PatternSlots*>::iterator it = domains_slots_.begin(); it != domains_slots_.end(); it++)
	{
		if (it->second != NULL)
		{
			it->second->Destroy();
			delete it->second;
		}
	}
	for (map<string, set<PatternTemplates*> >::iterator it = map_domain_templates_.begin(); it != map_domain_templates_.end(); it++)
	{
		for (set<PatternTemplates*>::iterator ii = it->second.begin(); ii != it->second.end(); ii++)
		{
			if (*ii)
			{
				(*ii)->Destroy();
				delete (*ii);
			}
		}
	}

	for (map<string, PatternDomain*>::iterator it = domain_ptr_.begin(); it != domain_ptr_.end(); it++)
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
	}
}


DIALOG_SEG_END_NAMESPACE(qp);


