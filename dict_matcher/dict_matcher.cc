#include "dict_matcher.h"
#include "../reader/config_reader.h"
#include <iostream>
#include <fstream>"

using std::map;
using std::string;
using std::vector;
using std::ifstream;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, DictMatcher);

int DictMatcher::Init(const string& cfg_path)
{
	ConfigReader reader(cfg_path);
	/*
		dict : {
			DICT_FILE = "./conf/dict.txt";
		};
	*/
	const string& file_path = reader.Get<string>("dict.DICT_FILE");
	ifstream infile(file_path.c_str());
	if (!infile)
	{
		LOG(ERROR, "dict file can not be open: %s", file_path.c_str());
		return -1;
	}
	/*
		class NeInfor
		{
			public:
				std::string type_;				//实体类型【singer, song, app, website】
				std::string normal_text_;		//DictMatcher中未填充此字段
		};

	*/
	ne_dict_ = new map<string, vector<NeInfor> >();
	string line;
	while (getline(infile, line))
	{
		size_t index = line.find("\t"); //白冬冬		singer
		if (index == string::npos)
		{
			LOG(ERROR, "wrong format line: %s", line.c_str());
		}
		string cur_key = line.substr(0, index); //白冬冬
		NeInfor ne_info;
		ne_info.type_ = line.substr(index + 1); //singer 
		map<string, vector<NeInfor> >::iterator it = ne_dict_->find(cur_key);
		if (it != ne_dict_->end())
		{
			it->second.push_back(ne_info);
		}
		else
		{
			vector<NeInfor> ne_infos;
			ne_infos.push_back(ne_info);
			ne_dict_->insert(std::make_pair(cur_key, ne_infos));
		}
	}
	return 0;
}

/*
O(n2)遍历切词的每种可能性，任意的 【i - j】 的切词组合， 然后去实体字典【singer,song,app,website】中去检查是否是这四类实体词，
可能【i-j】会从实体词典中匹配出多个结果
如果是，加入qp_result.NeTerm

*/
int DictMatcher::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	/*
	message Term {
		required string text = 1;		//切词
		required string pos = 2;		//词性
		required uint32 begin = 3;		//词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开
		required uint32 end = 4;		//词的结束位置,在原始query中的.
		optional float weight = 5;		//term权重 [切词时未填充]
	}
	*/
	/*
	【电视台/说/今天/天气/很/温暖/？】
	【  0，  1， 2， 3， 4,  5,  6】		<-   term_index
	*/
	string candidate_ne;
	for (int i = 0; i < qp_result->terms_size(); ++i) //遍历每个切词 i
	{
		const Term& term_i = qp_result->terms(i);
		candidate_ne = term_i.text(); //候选实体词
		for (int j = i; j < qp_result->terms_size(); j++)//遍历每个切词 j
		{
			const Term& term_j = qp_result->terms(j);
			if (j != i)
			{
				candidate_ne.append(term_j.text()); //拼接 i-j 的切词
			}
			map<string, vector<NeInfor> >::iterator it = ne_dict_->find(candidate_ne);
			if (it != ne_dict_->end()) //检查是否在实体词典中
			{
				vector<NeInfor>& ne_infos = it->second;
				for (int k = 0; k < ne_infos.size(); k++) //字典匹配可能有多个结果【不同类实体】
				{
					NeTerm* ne_term = qp_result->add_ne_terms();
					ne_term->set_ne_text(candidate_ne);
					ne_term->set_ne_type(ne_infos[k].type_);
					ne_term->set_begin(term_i.begin());
					ne_term->set_end(term_j.end());
				}
			}
		}//遍历每个切词 j
	}//遍历每个切词 i
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
