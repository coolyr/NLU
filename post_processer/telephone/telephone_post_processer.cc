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
	required string domain = 1;			//���� [map, calendar, music, telephone�� other]
	optional float domain_score = 2;	//�÷�
	repeated NeTerm ne_terms = 3;		//MaxEntδ��� [���������䣬 ���ʵ�����������ڵ�ǰdomain��ʵ���]
	required string ne_sentense = 4;	//MaxEntδ��� [���������䣬 ��query�����ڵ�ǰdomain��ʵ��ʣ���ʶ�������ʵ��ʣ����б�Ǻ��query]
}
*/
int TelephonePostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)
	{
		if (qp_result->domain_qp_results(i).domain().compare("telephone") != 0)//��telepthon������
		{
			continue;
		}
		/*
		message NeTerm{
			required string ne_type = 1;	//ʵ�����
			required string ne_text = 2;	//ʵ��� ���п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
			required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf - 8 ����3���ֽ�]Ϊ��λ������ҿ�
			required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
		}
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i);
		map<int, int> ne_index_map; //<per.begin, per��ʵ����б�����index>
		for (int j = 0; j < qp_result->ne_terms_size(); ++j)
		{
			if (qp_result->ne_terms(j).ne_type().compare("per") == 0)//ʶ��� CRF [per]ʵ��
			{
				NeTerm* ne_term = domain_qp_result->add_ne_terms();
				ne_term->CopyFrom(qp_result->ne_terms(j));
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
				continue;
			}
		}

		const string& query = qp_result->raw_query();//ԭʼquery
		string ne_sentence("");
		for (size_t index = 0; index < query.size();)//���ֽڱ���ԭʼquery
		{
			map<int, int>::const_iterator it = ne_index_map.find(index);
			if (it == ne_index_map.end())//����ʵ���per���ֽڿ�ʼ��
			{
				ne_sentence += query[index];//ƴ���� ��ǰ�ַ�
				index = index + 1;//�ַ�����
				if (index == query.size())
				{
					ne_sentence.append("/o");//��β���'/o'
				}
				continue;
			}
			//it != ne_index_map.end()   ִ�е��� ->  index��ʵ��ʵĿ�ʼλ��
			if (index != 0)
			{
				ne_sentence.append("/o");
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text());
			ne_sentence.append("/");
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());
			index = qp_result->ne_terms(it->second).end();//������ǰʵ��
		}//���ֽڱ���ԭʼquery
		domain_qp_result->set_ne_sentense(ne_sentence);//���ʵ����滻���query ==> "��绰��/o����/per(/o)"
	}
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
