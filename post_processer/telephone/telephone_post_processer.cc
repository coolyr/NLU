#include "telephone_post_processer.h"
#include "../../common_macros.h"
#include "../../log.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, TelephonePostProcesser);

using std::string;
using std::vector;
using std::map;
using std::set;

int TelephonePostProcesser::Init(const string& cfg_path)
{
	return 0;
}

int TelephonePostProcesser::Uinit()
{
	return 0;
}

/*
message DomainQpResult {
	required string domain = 1;			//域名 [map, calendar, music, telephone， other]
	optional float domain_score = 2;	//得分
	repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后分类器填充， 填充实体中所有属于当前domain的实体词]
	required string ne_sentense = 4;	//MaxEnt未填充 [后分类器填充， 把query中属于当前domain的实体词，用识别出来的实体词，进行标记后的query]
}
*/
int TelephonePostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)
	{
		if (qp_result->domain_qp_results(i).domain().compare("telephone") != 0)//非telepthon类跳过
		{
			continue;
		}
		/*
		message NeTerm{
			required string ne_type = 1;	//实体类别
			required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
			required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
			required uint32 end = 4;		//词的结束位置,在原始query中的.
		}
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i);
		map<int, int> ne_index_map; //<per.begin, per在实体词列表排序index>
		for (int j = 0; j < qp_result->ne_terms_size(); ++j)
		{
			if (qp_result->ne_terms(j).ne_type().compare("per") == 0)//识别出 CRF [per]实体
			{
				NeTerm* ne_term = domain_qp_result->add_ne_terms();
				ne_term->CopyFrom(qp_result->ne_terms(j));
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
				continue;
			}
		}

		const string& query = qp_result->raw_query();//原始query
		string ne_sentence("");
		for (size_t index = 0; index < query.size();)//按字节遍历原始query
		{
			map<int, int>::const_iterator it = ne_index_map.find(index);
			if (it == ne_index_map.end())//不是实体词per的字节开始处
			{
				ne_sentence += query[index];//拼接上 当前字符
				index = index + 1;//字符后移
				if (index == query.size())
				{
					ne_sentence.append("/o");//结尾添加'/o'
				}
				continue;
			}
			//it != ne_index_map.end()   执行到这 ->  index是实体词的开始位置
			if (index != 0)
			{
				ne_sentence.append("/o");
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text());
			ne_sentence.append("/");
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());
			index = qp_result->ne_terms(it->second).end();//跳过当前实体
		}//按字节遍历原始query
		domain_qp_result->set_ne_sentense(ne_sentence);//添加实体词替换后的query ==> "打电话给/o李明/per(/o)"
	}
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
