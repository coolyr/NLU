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
				std::string type_;				//ʵ�����͡�singer, song, app, website��
				std::string normal_text_;		//DictMatcher��δ�����ֶ�
		};

	*/
	ne_dict_ = new map<string, vector<NeInfor> >();
	string line;
	while (getline(infile, line))
	{
		size_t index = line.find("\t"); //�׶���		singer
		if (index == string::npos)
		{
			LOG(ERROR, "wrong format line: %s", line.c_str());
		}
		string cur_key = line.substr(0, index); //�׶���
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
O(n2)�����дʵ�ÿ�ֿ����ԣ������ ��i - j�� ���д���ϣ� Ȼ��ȥʵ���ֵ䡾singer,song,app,website����ȥ����Ƿ���������ʵ��ʣ�
���ܡ�i-j�����ʵ��ʵ���ƥ���������
����ǣ�����qp_result.NeTerm

*/
int DictMatcher::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	/*
	message Term {
		required string text = 1;		//�д�
		required string pos = 2;		//����
		required uint32 begin = 3;		//�ʵĿ�ʼλ��,��ԭʼquery�е�. ���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ�
		required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
		optional float weight = 5;		//termȨ�� [�д�ʱδ���]
	}
	*/
	/*
	������̨/˵/����/����/��/��ů/����
	��  0��  1�� 2�� 3�� 4,  5,  6��		<-   term_index
	*/
	string candidate_ne;
	for (int i = 0; i < qp_result->terms_size(); ++i) //����ÿ���д� i
	{
		const Term& term_i = qp_result->terms(i);
		candidate_ne = term_i.text(); //��ѡʵ���
		for (int j = i; j < qp_result->terms_size(); j++)//����ÿ���д� j
		{
			const Term& term_j = qp_result->terms(j);
			if (j != i)
			{
				candidate_ne.append(term_j.text()); //ƴ�� i-j ���д�
			}
			map<string, vector<NeInfor> >::iterator it = ne_dict_->find(candidate_ne);
			if (it != ne_dict_->end()) //����Ƿ���ʵ��ʵ���
			{
				vector<NeInfor>& ne_infos = it->second;
				for (int k = 0; k < ne_infos.size(); k++) //�ֵ�ƥ������ж���������ͬ��ʵ�塿
				{
					NeTerm* ne_term = qp_result->add_ne_terms();
					ne_term->set_ne_text(candidate_ne);
					ne_term->set_ne_type(ne_infos[k].type_);
					ne_term->set_begin(term_i.begin());
					ne_term->set_end(term_j.end());
				}
			}
		}//����ÿ���д� j
	}//����ÿ���д� i
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
