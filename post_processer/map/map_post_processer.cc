#include "map_post_processer.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include "../../common_macros.h"
#include "../../log.h"
#include "../../proto/qp.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, MapPostProcesser);

using std::string;
using std::vector;
using std::map;
using std::set;

int MapPostProcesser::Init(const string& cfg_path)
{
	return 0;
}

int MapPostProcesser::Uinit()
{
	return 0;
}
/*
message DomainQpResult {
	required string domain = 1;			//域名 [map, calendar, music, telephone， other]
	optional float domain_score = 2;	//MaxEnt分类得分
	repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后分类器填充， 填充实体中所有属于当前domain的实体词]
	required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充， 把query中属于当前domain的实体词，用识别出来的实体词，进行标记后的query]  => '导航去/o五道口/poi(/o)'
}
*/
int MapPostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)//遍历最大熵 每个分类 domain 【目前只有一个】
	{
		if (qp_result->domain_qp_results(i).domain().compare("map") != 0)//过滤掉非 map类
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
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i); // mutable ???可修改？
		map<int, int> ne_index_map; //[实体词在原始query中的字节begin, 实体词的排序index]
		for (int j = 0; j < qp_result->ne_terms_size(); ++j)//遍历实体词 CRF[per/poi/loc]	Dit[singer/songer/app/website]
		{
			if (qp_result->ne_terms(j).ne_type().compare("loc") == 0)
			{
				/*
				message NeTerm{
					required string ne_type = 1;	//实体类别
					required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
					required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
					required uint32 end = 4;		//词的结束位置,在原始query中的.
				}
			*/
				NeTerm* ne_term = domain_qp_result->add_ne_terms();//往DomainQpResult中添加属于当前domain的实体词
				ne_term->CopyFrom(qp_result->ne_terms(j));//拷贝loc实体 => 添加到DomainQpResult中
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));// [loc.begin, loc的排序index]
				continue;
			}
			if (qp_result->ne_terms(j).ne_type().compare("poi") == 0)
			{
				NeTerm* ne_term = domain_qp_result->add_ne_terms();
				ne_term->CopyFrom(qp_result->ne_terms(j));
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
				continue;
			}
		}//遍历实体词 CRF[per/poi/loc] Dit[singer/songer/app/website]

		const string& query = qp_result->raw_query();// 原始query
		string ne_sentence("");//实体词 替换后的query
		for (size_t index = 0; index < query.size();)//按字节遍历原始query
		{
			map<int, int>::const_iterator it = ne_index_map.find(index);// [loc.begin, loc的排序index]
			if (it == ne_index_map.end())//当前字节index不是实体词的开始位置
			{
				ne_sentence += query[index];//拼接上 当前字符
				index = index + 1;//字符位置后移
				if (index == query.size())//移到末尾
				{
					ne_sentence.append("/o");//结尾 "/o"
				}
				continue;//继续
			}
			//it != ne_index_map.end()   执行到这 ->  index是实体词的开始位置
			if (index != 0)//实体词开始位置不是query的起始位置
			{
				ne_sentence.append("/o");// “刘德华/[per,singer] 唱 了 一首 忘情水/song”  =>    “刘德华唱了一首/o”
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text()); // “刘德华唱了一首/o忘情水”
			ne_sentence.append("/");// “刘德华唱了一首/o忘情水/”
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());// “刘德华唱了一首/o忘情水/song”
			index = qp_result->ne_terms(it->second).end();//index跳过当前实体词,赋值为向一个非实体字符
		}//按字节遍历原始query

		domain_qp_result->set_ne_sentense(ne_sentence);//添加实体词标记后的query ==> "导航去/o五道口/poi(/o)"
	}//遍历最大熵 每个分类 domain 【目前只有一个】
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
