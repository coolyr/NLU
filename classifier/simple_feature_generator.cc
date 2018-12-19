#include "../classifier/simple_feature_generator.h"

#include <string>
#include <map>
#include <vector>

using std::map;
using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, SimpleFeatureGenerator);

bool Comparator(const Term& a, const Term& b)
{
	return a.begin() < b.begin();
}

/*
message FeatureElement {
	required string key = 1;	//����
	required uint32 count = 2;  //Ƶ��
}
message Feature {
	required string label = 1;
	repeated FeatureElement elements = 2;
}
*/
bool SimpleFeatureGenerator::GenerateFeature(const QpRequest& qp_request, const QpResult& qp_result, Feature* feature)
{
	/*

	���y     ����x	      Ȩ��W      ===>>    ����f ������Ĺ��̾��ǲ������Ĺ��̡�
	__________________________
	7		�ٷ�_��վ	0.477547
	14		��Ϣ_��		0.032232
	13		loc_��		-0.012150
	___________________________
	*/
	if (NULL == feature)
	{
		return false;
	}
	vector<Term> term_vec;
	map<string, int> feature_map;
	for (int i = 0; i < qp_result.terms_size(); ++i) //��������   =>>  feature_map["����"] = count("����")
	{
		term_vec.push_back(qp_result.terms(i));
		feature_map[qp_result.terms(i).text()]++; //���ʼ��� +1
	}

	/*
	message Term {
		required string text = 1;		//�д�
		required string pos = 2;		//����
		required uint32 begin = 3;     //�ʵĿ�ʼλ�� ����ԭʼquery�еġ������ֽ�Ϊ��λ������ҿ�
		required uint32 end = 4;	   //�ʵĽ���λ�� ����ԭʼquery�еġ�
		optional float weight = 5;     //termȨ�� ��word_segmenter δ��ӡ�
	}
	*/
	// "����  ����  ��  ��"
	for (size_t i = 1; i < term_vec.size(); ++i) //�ʵ�bigram����   =>   feature_map["��������"] = count("��������")
	{
		string bgram;
		if (term_vec[i - 1].end() == term_vec[i].begin())
		{
			bgram = term_vec[i - 1].text() + term_vec[i].text();
		}
		feature_map[bgram]++;//bigram��������
	}

	//    "���»�/[per | singer]  ��  ��  һ��  ����ˮ/song"   ==>   per��CRFʶ��ģ�singer��song��ͨ���ʵ�ʶ���
	for (int i = 0; i < qp_result.ne_terms_size(); ++i) //ʵ��ʵ�bigram����   =>   feature_map["singer_��"] = count("singer_��")
	{
		/*
		message NeTerm {
			required string ne_type = 1;	//ʵ������    CRF��[per/poi/loc]    DIC�� [singer/song/app/website]
			required string ne_text = 2;	//ʵ��� ��CRF�п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
			required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf - 8 ����3���ֽ�]Ϊ��λ������ҿ�
			required uint32 end = 4;
		}
		*/
		string bgram_for_ne_term;
		//####################################����ʼ�� �� �������� ���⴦��###########################################
		if (0 == qp_result.ne_terms(i).begin())//ʵ����ǿ�ʼ
		{
			// start
			bgram_for_ne_term = "S_" + qp_result.ne_terms(i).ne_type();		// "S_per"   "S_singer"
			feature_map[bgram_for_ne_term]++;
		}
		else if (qp_request.query().length() ==	qp_result.ne_terms(i).end()) //ʵ����ǽ���
		{
			// end
			bgram_for_ne_term = qp_result.ne_terms(i).ne_type() + "_E";		// "song_E"
			feature_map[bgram_for_ne_term]++;
		}
		//#################################################################################################

		//##########################################ʵ��ʺ͡�ǰ��ʡ�������#################################
		vector<int> indexes_;
		const NeTerm& ne_term = qp_result.ne_terms(i);
		//��ǰʵ���ne_term�� ��ǰһ���ʡ��� ����һ���ʡ��Ĵ���������
		if (!FindMatch(term_vec, ne_term, &indexes_))
		{
			// failure  ??????������->��indexes_==NULL����false
			LOG(INFO, "Failed to find matched Term for NeTerm!"); //�д� �� ʵ��� ��ƥ��   ==>  [ʵ���ȡ�������ʵ��м䲿��]
			continue;
		}
		// found matched and update
		for (size_t i = 0; i < indexes_.size(); ++i)//ʵ��ʺ͡�ǰ��ʡ�������
		{
			const Term&  term = term_vec[indexes_[i]];
			if (term.begin() == ne_term.end())//��ǰ����ʵ��ʵ� ����һ���ʡ�
			{
				bgram_for_ne_term = ne_term.ne_type() + "_" + term.text();// "singer_��"  "per_��"
				feature_map[bgram_for_ne_term]++;
			}
			else if (term.end() == ne_term.begin())//��ǰ����ʵ��ʵ� ��ǰһ���ʡ�
			{
				bgram_for_ne_term = term.text() + "_" + ne_term.ne_type(); // "һ��_song"
				feature_map[bgram_for_ne_term]++;
			}
			else
			{
				LOG(ERROR, "Match Error for term:%s, ne_term:%s", term.text().c_str(), ne_term.ne_type().c_str());
			}
		}//ʵ��ʺ͡�ǰ��ʡ�������
	}//ʵ��ʵ�bigram����   =>   feature_map["singer_��"] = count("singer_��")
	/*
	message FeatureElement {
		required string key = 1; //����
		required uint32 count = 2; //Ƶ��
	}
	message Feature {
		required string label = 1;
		repeated FeatureElement elements = 2;
	}
	*/
	map<string, int>::iterator iter;
	for (iter = feature_map.begin(); iter != feature_map.end(); ++iter)
	{
		FeatureElement* element = feature->add_elements();
		element->set_key(iter->first); //����
		element->set_count(iter->second); //count(feature)
	}
	return true;
}

/*
vector<int> indexes_;
const NeTerm& ne_term = qp_result.ne_terms(i);
if (!FindMatch(term_vec, ne_term, &indexes_))
	{continue;}
*/

/*
��ǰʵ���ne_term�� ��ǰһ���ʡ��� ����һ���ʡ��Ĵ���������

��Aʵ���B��    =��   index[0] = index[A], index[1] = index[B]
��ʵ���C��     =��   index[0] = index[B]
��Aʵ��ʡ�     =��   index[0] = index[A]
*/
bool SimpleFeatureGenerator::FindMatch(const vector<Term>& term_vec, const NeTerm& ne_term,	vector<int>* index)
{
	if (NULL == index)
	{
		return false;
	}
	index->resize(0);//���ó���Ϊ0
	for (size_t i = 0; i < term_vec.size(); ++i)
	{
		if (term_vec[i].end() == ne_term.begin() || term_vec[i].begin() == ne_term.end())
		{
			index->push_back(i); //��ǰʵ���ne_term�� ��ǰһ���ʡ��� ����һ���ʡ��Ĵ���������
		}
	}
	return true;
}

DIALOG_SEG_END_NAMESPACE(qp);
