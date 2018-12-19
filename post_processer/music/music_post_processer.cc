#include "music_post_processer.h"
#include "../../reader/config_reader.h"
#include "../../common_macros.h"
#include "../../log.h"

#include <map>
#include <set>
#include <iostream>

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, MusicPostProcesser);

using std::string;
using std::vector;
using std::map;
using std::set;

int MusicPostProcesser::Init(const string& cfg_path)
{
	ConfigReader reader(cfg_path);
	const string& model_path = reader.Get<string>("music_model.MODEL_FILE"); //./models/music.model
	lm_ = new LanguageModel();		//语言模型 - Trigram
	int ret = lm_->Init(model_path);//./models/music.model
	if (ret != 0)
	{
		LOG(ERROR, "can not init music lm");
		return -1;
	}
	return 0;
}

int MusicPostProcesser::Uinit()
{
	return 0;
}
/*
message DomainQpResult {
	required string domain = 1;			//域名 [map, calendar, music, telephone， other]
	optional float domain_score = 2;	//得分
	repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后分类器填充， 填充实体中所有属于当前domain的实体词]
	required string ne_sentense = 4;	//MaxEnt未填充 [后分类器填充， 把query中属于当前domain的实体词，用识别出来的实体词，进行替换和标记后的query]
}
*/
int MusicPostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)//遍历最大熵 每个分类 DomainQpResult 【目前只有一个】
	{
		if (qp_result->domain_qp_results(i).domain().compare("music") != 0)//过滤掉非“music” 类
		{
			continue;
		}
		/*
		message QpResult {
			required string raw_query = 1;					//原始query = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;						//检索粒度切词 【WordSegmenter类完成】
			repeated NeTerm ne_terms = 4;					//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;	//最大熵 分类的domain
		}
		message NeTerm{
			required string ne_type = 1;	//实体类别
			required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
			required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf-8 汉字3个字节]为单位，左闭右开
			required uint32 end = 4;		//词的结束位置,在原始query中的.
		}
		*/
		NeTerm ne_term_temp;
		vector<NeTerm> ne_singer_terms; //singer实体NeTerm
		vector<NeTerm> ne_song_terms;	//song实体NeTerm
		vector<int> ne_singer_index;	//singer实体在所有实体词的排序index
		ne_singer_index.push_back(-1);	//默认里面有一个
		vector<int> ne_song_index;
		ne_song_index.push_back(-1);
		ne_singer_terms.push_back(ne_term_temp);
		ne_song_terms.push_back(ne_term_temp);

		for (int j = 0; j < qp_result->ne_terms_size(); ++j)//遍历实体词 CRF[per/poi/loc] Dit[singer/songer/app/website]
		{
			if (qp_result->ne_terms(j).ne_type().compare("singer") == 0)
			{
				ne_singer_terms.push_back(qp_result->ne_terms(j));//添加singer实体
				ne_singer_index.push_back(j);//添加singer在总的query识别出的实体词的排序index
				continue;
			}
			if (qp_result->ne_terms(j).ne_type().compare("song") == 0)
			{
				ne_song_terms.push_back(qp_result->ne_terms(j));
				ne_song_index.push_back(j);
				continue;
			}
		}//遍历实体词 CRF[per/poi/loc] Dit[singer/songer/app/website]

		vector<string> temp_terms;
		int max_singer_index = -1;
		int max_song_index = -1;
		float max_score = -10000.0;
		float temp_score = 0.0;

		for (size_t j = 0; j < ne_singer_terms.size(); ++j)//遍历singer实体[默认里面有一个]
		{
			for (size_t k = 0; k < ne_song_terms.size(); ++k)//遍历song实体[默认里面有一个]
			{
				if (Violate(ne_singer_terms[j], ne_song_terms[k]) != 0)//判断:singer[j]和song[k]的实体词是否【交叉】
				{
					continue;
				}

				temp_terms.clear();
				for (size_t l = 0; l < qp_result->terms_size(); ++l)//遍历分词
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
					if (qp_result->terms(l).begin() == ne_singer_terms[j].begin() && (j != 0))//j=0的位置 是自己设置的对象【忽略】
					{
						temp_terms.push_back("singer");//当切词的字节begin位置和singer[j]实体词的begin位置相同时，添加singer实体
						continue;
					}
					if (qp_result->terms(l).begin() == ne_song_terms[k].begin() && (k != 0))
					{
						temp_terms.push_back("song");
						continue;
					}
					if ((qp_result->terms(l).begin() > ne_singer_terms[j].begin())// 当切词的begin > 实体词begin && 切词end <= 实体词end  =>  实体词是有多个term组成，过滤掉中间的term
						&& (qp_result->terms(l).end() <= ne_singer_terms[j].end()))//<singer.B <term.B, term.E=singer.E>
					{
						continue;
					}
					if ((qp_result->terms(l).begin() > ne_song_terms[k].begin())
						&& (qp_result->terms(l).end() <= ne_song_terms[k].end()))
					{
						continue;
					}

					temp_terms.push_back(qp_result->terms(l).text());//实体词之外的 term
				}//遍历分词
				//################################################################################################################
				// temp_terms =	["我", “想”, "听", "singer", "的", "song"]	"我想听singer的song"     <==  "我想听刘德华的歌"
				// 再通过语言模型计算得分 => "我想听singer的song"
				lm_->ComputeScore(temp_terms, temp_score); //每个<singer,song>实体对替换后计算 => 不是所有替换后一起计算
				if (temp_score > max_score)//通过trigram计算最大得分的singer和song实体词组合形式
				{
					max_score = temp_score;
					max_singer_index = j;	//记录最大得分的singer和song实体词index
					max_song_index = k;		//[从总的实体词中取出的singer和song的实体列表中的索引index]
				}
				//################################################################################################################
			}//遍历song实体
		}//遍历singer实体

		/*
		message DomainQpResult {
			required string domain = 1;		 //域名 [map, calendar, music, telephone， other]
			optional float domain_score = 2; //得分
			repeated NeTerm ne_terms = 3;	 //MaxEnt未填充 [后处理器填充， 填充实体中所有属于当前domain的实体词]
			required string ne_sentense = 4; //MaxEnt未填充 [后处理器填充， 把query中属于当前domain的实体词，用识别出来的实体词，进行标记后的query]  => '导航去/o五道口/poi(/o)'
		}
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i);//当前处理的qp_result.DomainQpResult
		map<int, int> ne_index_map;//<实体词在原始query中的字节begin, 实体词在原始query识别出的实体词列表中的排序index>
		if ((max_singer_index != -1) && (max_singer_index != 0))//-1：初始值 0：填充值
		{
			NeTerm* ne_term = domain_qp_result->add_ne_terms();
			ne_term->CopyFrom(qp_result->ne_terms(ne_singer_index[max_singer_index]));//取出singer实体NeTerm, 深度拷贝到DomainQpResult的实体列表里
			ne_index_map.insert(std::make_pair(ne_singer_terms[max_singer_index].begin(), ne_singer_index[max_singer_index]));
		}
		if ((max_song_index != -1) && (max_song_index != 0))
		{
			NeTerm* ne_term = domain_qp_result->add_ne_terms();
			ne_term->CopyFrom(qp_result->ne_terms(ne_song_index[max_song_index]));
			ne_index_map.insert(std::make_pair(ne_song_terms[max_song_index].begin(), ne_song_index[max_song_index]));
		}

		/*DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i);
		map<int, int> ne_index_map;
		for (int j = 0; j < qp_result->ne_terms_size(); ++j) {
		if (qp_result->ne_terms(j).ne_type().compare("singer") == 0) {
		NeTerm* ne_term = domain_qp_result->add_ne_terms();
		ne_term->CopyFrom(qp_result->ne_terms(j));
		ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
		continue;
		}
		if (qp_result->ne_terms(j).ne_type().compare("song") == 0) {
		NeTerm* ne_term = domain_qp_result->add_ne_terms();
		ne_term->CopyFrom(qp_result->ne_terms(j));
		ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
		continue;
		}
		}*/

		/*
		message QpResult {
			required string raw_query = 1;		//原始query = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;			//检索粒度切词 【WordSegmenter类完成】
			repeated NeTerm ne_terms = 4;		//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5; //最大熵 分类的domain
		}
		*/
		const string& query = qp_result->raw_query();
		string ne_sentence("");
		for (size_t index = 0; index < query.size();)//按字节遍历原始query
		{
			//map<int, int> ne_index_map   ==>  <实体词在原始query中的字节begin, 实体词在原始query识别出的实体词列表中的排序index>
			map<int, int>::const_iterator it = ne_index_map.find(index); //ne_index_map只取了一种trigram计算得分最高的可能组合 => “我想 听 一首 刘德华/singer 的 忘情水/song”
			if (it == ne_index_map.end())//非singer和song实体词开始位置
			{
				ne_sentence += query[index];//添加当前byte
				index = index + 1;
				if (index == query.size())
				{
					ne_sentence.append("/o");//结尾 "/o"
				}
				continue;
			}
			//当前字节index是singer和song开始位置
			if (index != 0)//实体词开始位置不是query的起始位置
			{
				ne_sentence.append("/o");// “我想 听 一首 刘德华/singer 的 忘情水/song”  =>    “我想听一首/o”
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text()); //“我想听一首/o刘德华”
			ne_sentence.append("/");//“我想听一首/o刘德华/”
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());////“我想听一首/o刘德华/singer”
			index = qp_result->ne_terms(it->second).end();//跳过当前实体词
		}//按字节遍历原始query

		domain_qp_result->set_ne_sentense(ne_sentence); //添加实体词替换后的query =>  "我想听一首/o刘德华/singer的/o忘情水/song(/o)"
	}//遍历最大熵 每个分类 domain 【目前只有一个】
	return 0;
}
/*
message NeTerm{
	required string ne_type = 1;	//实体类别
	required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
	required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf-8 汉字3个字节]为单位，左闭右开
	required uint32 end = 4;		//词的结束位置,在原始query中的.
}
*/
int MusicPostProcesser::Violate(const NeTerm& ne_termi, const NeTerm& ne_termj)
{
	if ((ne_termi.ne_text().empty()) || (ne_termj.ne_text().empty()))
	{
		return 0;//任何为空
	}
	if (ne_termi.begin() < ne_termj.begin())
	{
		if (ne_termi.end() <= ne_termj.begin())
		{
			return 0;// <i_b  i_e> <j_b, j_e>  独立
		}
		else
		{
			return -1;// <i_b <j_b   i_e>  交叉
		}
	}
	if (ne_termj.begin() < ne_termi.begin())
	{
		if (ne_termj.end() <= ne_termi.begin())
		{
			return 0;//  <j_b  j_e> <i_b, i_e>  独立
		}
		else
		{
			return -1; //<j_b <i_b   j_e>  交叉
		}
	}
	return -1;
}

DIALOG_SEG_END_NAMESPACE(qp);
