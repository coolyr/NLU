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
	lm_ = new LanguageModel();		//����ģ�� - Trigram
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
	required string domain = 1;			//���� [map, calendar, music, telephone�� other]
	optional float domain_score = 2;	//�÷�
	repeated NeTerm ne_terms = 3;		//MaxEntδ��� [���������䣬 ���ʵ�����������ڵ�ǰdomain��ʵ���]
	required string ne_sentense = 4;	//MaxEntδ��� [���������䣬 ��query�����ڵ�ǰdomain��ʵ��ʣ���ʶ�������ʵ��ʣ������滻�ͱ�Ǻ��query]
}
*/
int MusicPostProcesser::PostProcess(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)//��������� ÿ������ DomainQpResult ��Ŀǰֻ��һ����
	{
		if (qp_result->domain_qp_results(i).domain().compare("music") != 0)//���˵��ǡ�music�� ��
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
		message NeTerm{
			required string ne_type = 1;	//ʵ�����
			required string ne_text = 2;	//ʵ��� ���п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
			required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ�
			required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
		}
		*/
		NeTerm ne_term_temp;
		vector<NeTerm> ne_singer_terms; //singerʵ��NeTerm
		vector<NeTerm> ne_song_terms;	//songʵ��NeTerm
		vector<int> ne_singer_index;	//singerʵ��������ʵ��ʵ�����index
		ne_singer_index.push_back(-1);	//Ĭ��������һ��
		vector<int> ne_song_index;
		ne_song_index.push_back(-1);
		ne_singer_terms.push_back(ne_term_temp);
		ne_song_terms.push_back(ne_term_temp);

		for (int j = 0; j < qp_result->ne_terms_size(); ++j)//����ʵ��� CRF[per/poi/loc] Dit[singer/songer/app/website]
		{
			if (qp_result->ne_terms(j).ne_type().compare("singer") == 0)
			{
				ne_singer_terms.push_back(qp_result->ne_terms(j));//���singerʵ��
				ne_singer_index.push_back(j);//���singer���ܵ�queryʶ�����ʵ��ʵ�����index
				continue;
			}
			if (qp_result->ne_terms(j).ne_type().compare("song") == 0)
			{
				ne_song_terms.push_back(qp_result->ne_terms(j));
				ne_song_index.push_back(j);
				continue;
			}
		}//����ʵ��� CRF[per/poi/loc] Dit[singer/songer/app/website]

		vector<string> temp_terms;
		int max_singer_index = -1;
		int max_song_index = -1;
		float max_score = -10000.0;
		float temp_score = 0.0;

		for (size_t j = 0; j < ne_singer_terms.size(); ++j)//����singerʵ��[Ĭ��������һ��]
		{
			for (size_t k = 0; k < ne_song_terms.size(); ++k)//����songʵ��[Ĭ��������һ��]
			{
				if (Violate(ne_singer_terms[j], ne_song_terms[k]) != 0)//�ж�:singer[j]��song[k]��ʵ����Ƿ񡾽��桿
				{
					continue;
				}

				temp_terms.clear();
				for (size_t l = 0; l < qp_result->terms_size(); ++l)//�����ִ�
				{
					/*
					message Term {
						required string text = 1;		//�д�
						required string pos = 2;		//����
						required uint32 begin = 3;      //�ʵĿ�ʼλ��,��ԭʼquery�е�. ���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ� 
						required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
						optional float weight = 5;      //termȨ�� [�д�ʱδ���]
					}

					*/
					if (qp_result->terms(l).begin() == ne_singer_terms[j].begin() && (j != 0))//j=0��λ�� ���Լ����õĶ��󡾺��ԡ�
					{
						temp_terms.push_back("singer");//���дʵ��ֽ�beginλ�ú�singer[j]ʵ��ʵ�beginλ����ͬʱ�����singerʵ��
						continue;
					}
					if (qp_result->terms(l).begin() == ne_song_terms[k].begin() && (k != 0))
					{
						temp_terms.push_back("song");
						continue;
					}
					if ((qp_result->terms(l).begin() > ne_singer_terms[j].begin())// ���дʵ�begin > ʵ���begin && �д�end <= ʵ���end  =>  ʵ������ж��term��ɣ����˵��м��term
						&& (qp_result->terms(l).end() <= ne_singer_terms[j].end()))//<singer.B <term.B, term.E=singer.E>
					{
						continue;
					}
					if ((qp_result->terms(l).begin() > ne_song_terms[k].begin())
						&& (qp_result->terms(l).end() <= ne_song_terms[k].end()))
					{
						continue;
					}

					temp_terms.push_back(qp_result->terms(l).text());//ʵ���֮��� term
				}//�����ִ�
				//################################################################################################################
				// temp_terms =	["��", ���롱, "��", "singer", "��", "song"]	"������singer��song"     <==  "���������»��ĸ�"
				// ��ͨ������ģ�ͼ���÷� => "������singer��song"
				lm_->ComputeScore(temp_terms, temp_score); //ÿ��<singer,song>ʵ����滻����� => ���������滻��һ�����
				if (temp_score > max_score)//ͨ��trigram�������÷ֵ�singer��songʵ��������ʽ
				{
					max_score = temp_score;
					max_singer_index = j;	//��¼���÷ֵ�singer��songʵ���index
					max_song_index = k;		//[���ܵ�ʵ�����ȡ����singer��song��ʵ���б��е�����index]
				}
				//################################################################################################################
			}//����songʵ��
		}//����singerʵ��

		/*
		message DomainQpResult {
			required string domain = 1;		 //���� [map, calendar, music, telephone�� other]
			optional float domain_score = 2; //�÷�
			repeated NeTerm ne_terms = 3;	 //MaxEntδ��� [��������䣬 ���ʵ�����������ڵ�ǰdomain��ʵ���]
			required string ne_sentense = 4; //MaxEntδ��� [��������䣬 ��query�����ڵ�ǰdomain��ʵ��ʣ���ʶ�������ʵ��ʣ����б�Ǻ��query]  => '����ȥ/o�����/poi(/o)'
		}
		*/
		DomainQpResult* domain_qp_result = qp_result->mutable_domain_qp_results(i);//��ǰ�����qp_result.DomainQpResult
		map<int, int> ne_index_map;//<ʵ�����ԭʼquery�е��ֽ�begin, ʵ�����ԭʼqueryʶ�����ʵ����б��е�����index>
		if ((max_singer_index != -1) && (max_singer_index != 0))//-1����ʼֵ 0�����ֵ
		{
			NeTerm* ne_term = domain_qp_result->add_ne_terms();
			ne_term->CopyFrom(qp_result->ne_terms(ne_singer_index[max_singer_index]));//ȡ��singerʵ��NeTerm, ��ȿ�����DomainQpResult��ʵ���б���
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
			required string raw_query = 1;		//ԭʼquery = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;			//���������д� ��WordSegmenter����ɡ�
			repeated NeTerm ne_terms = 4;		//ʵ��CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5; //����� �����domain
		}
		*/
		const string& query = qp_result->raw_query();
		string ne_sentence("");
		for (size_t index = 0; index < query.size();)//���ֽڱ���ԭʼquery
		{
			//map<int, int> ne_index_map   ==>  <ʵ�����ԭʼquery�е��ֽ�begin, ʵ�����ԭʼqueryʶ�����ʵ����б��е�����index>
			map<int, int>::const_iterator it = ne_index_map.find(index); //ne_index_mapֻȡ��һ��trigram����÷���ߵĿ������ => ������ �� һ�� ���»�/singer �� ����ˮ/song��
			if (it == ne_index_map.end())//��singer��songʵ��ʿ�ʼλ��
			{
				ne_sentence += query[index];//��ӵ�ǰbyte
				index = index + 1;
				if (index == query.size())
				{
					ne_sentence.append("/o");//��β "/o"
				}
				continue;
			}
			//��ǰ�ֽ�index��singer��song��ʼλ��
			if (index != 0)//ʵ��ʿ�ʼλ�ò���query����ʼλ��
			{
				ne_sentence.append("/o");// ������ �� һ�� ���»�/singer �� ����ˮ/song��  =>    ��������һ��/o��
			}
			ne_sentence.append(qp_result->ne_terms(it->second).ne_text()); //��������һ��/o���»���
			ne_sentence.append("/");//��������һ��/o���»�/��
			ne_sentence.append(qp_result->ne_terms(it->second).ne_type());////��������һ��/o���»�/singer��
			index = qp_result->ne_terms(it->second).end();//������ǰʵ���
		}//���ֽڱ���ԭʼquery

		domain_qp_result->set_ne_sentense(ne_sentence); //���ʵ����滻���query =>  "������һ��/o���»�/singer��/o����ˮ/song(/o)"
	}//��������� ÿ������ domain ��Ŀǰֻ��һ����
	return 0;
}
/*
message NeTerm{
	required string ne_type = 1;	//ʵ�����
	required string ne_text = 2;	//ʵ��� ���п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
	required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ�
	required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
}
*/
int MusicPostProcesser::Violate(const NeTerm& ne_termi, const NeTerm& ne_termj)
{
	if ((ne_termi.ne_text().empty()) || (ne_termj.ne_text().empty()))
	{
		return 0;//�κ�Ϊ��
	}
	if (ne_termi.begin() < ne_termj.begin())
	{
		if (ne_termi.end() <= ne_termj.begin())
		{
			return 0;// <i_b  i_e> <j_b, j_e>  ����
		}
		else
		{
			return -1;// <i_b <j_b   i_e>  ����
		}
	}
	if (ne_termj.begin() < ne_termi.begin())
	{
		if (ne_termj.end() <= ne_termi.begin())
		{
			return 0;//  <j_b  j_e> <i_b, i_e>  ����
		}
		else
		{
			return -1; //<j_b <i_b   j_e>  ����
		}
	}
	return -1;
}

DIALOG_SEG_END_NAMESPACE(qp);
