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
	required string domain = 1;			//���� [map, calendar, music, telephone�� other]
	optional float domain_score = 2;	//MaxEnt����÷�
	repeated NeTerm ne_terms = 3;		//MaxEntδ��� [���������䣬 ���ʵ�����������ڵ�ǰdomain��ʵ���]
	required string ne_sentense = 4;	//MaxEntδ��� [��������䣬 ��query�����ڵ�ǰdomain��ʵ��ʣ���ʶ�������ʵ��ʣ����б�Ǻ��query]  => '����ȥ/o�����/poi(/o)'
}
*/
int MapPostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)//��������� ÿ������ domain ��Ŀǰֻ��һ����
	{
		if (qp_result->domain_qp_results(i).domain().compare("map") != 0)//���˵��� map��
		{
			continue;
		}
		/*
		message QpResult {
			required string raw_query = 1;					//ԭʼquery = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;						//���������д� ��WordSegmenter����ɡ�
			repeated NeTerm ne_terms = 4;					//ʵ��CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;	//����� �����domain
		}
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i); // mutable ???���޸ģ�
		map<int, int> ne_index_map; //[ʵ�����ԭʼquery�е��ֽ�begin, ʵ��ʵ�����index]
		for (int j = 0; j < qp_result->ne_terms_size(); ++j)//����ʵ��� CRF[per/poi/loc]	Dit[singer/songer/app/website]
		{
			if (qp_result->ne_terms(j).ne_type().compare("loc") == 0)
			{
				/*
				message NeTerm{
					required string ne_type = 1;	//ʵ�����
					required string ne_text = 2;	//ʵ��� ���п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
					required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf - 8 ����3���ֽ�]Ϊ��λ������ҿ�
					required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
				}
			*/
				NeTerm* ne_term = domain_qp_result->add_ne_terms();//��DomainQpResult��������ڵ�ǰdomain��ʵ���
				ne_term->CopyFrom(qp_result->ne_terms(j));//����locʵ�� => ��ӵ�DomainQpResult��
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));// [loc.begin, loc������index]
				continue;
			}
			if (qp_result->ne_terms(j).ne_type().compare("poi") == 0)
			{
				NeTerm* ne_term = domain_qp_result->add_ne_terms();
				ne_term->CopyFrom(qp_result->ne_terms(j));
				ne_index_map.insert(std::make_pair(qp_result->ne_terms(j).begin(), j));
				continue;
			}
		}//����ʵ��� CRF[per/poi/loc] Dit[singer/songer/app/website]

		const string& query = qp_result->raw_query();// ԭʼquery
		string ne_sentence("");//ʵ��� �滻���query
		for (size_t index = 0; index < query.size();)//���ֽڱ���ԭʼquery
		{
			map<int, int>::const_iterator it = ne_index_map.find(index);// [loc.begin, loc������index]
			if (it == ne_index_map.end())//��ǰ�ֽ�index����ʵ��ʵĿ�ʼλ��
			{
				ne_sentence += query[index];//ƴ���� ��ǰ�ַ�
				index = index + 1;//�ַ�λ�ú���
				if (index == query.size())//�Ƶ�ĩβ
				{
					ne_sentence.append("/o");//��β "/o"
				}
				continue;//����
			}
			//it != ne_index_map.end()   ִ�е��� ->  index��ʵ��ʵĿ�ʼλ��
			if (index != 0)//ʵ��ʿ�ʼλ�ò���query����ʼλ��
			{
				ne_sentence.append("/o");// �����»�/[per,singer] �� �� һ�� ����ˮ/song��  =>    �����»�����һ��/o��
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text()); // �����»�����һ��/o����ˮ��
			ne_sentence.append("/");// �����»�����һ��/o����ˮ/��
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());// �����»�����һ��/o����ˮ/song��
			index = qp_result->ne_terms(it->second).end();//index������ǰʵ���,��ֵΪ��һ����ʵ���ַ�
		}//���ֽڱ���ԭʼquery

		domain_qp_result->set_ne_sentense(ne_sentence);//���ʵ��ʱ�Ǻ��query ==> "����ȥ/o�����/poi(/o)"
	}//��������� ÿ������ domain ��Ŀǰֻ��һ����
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
