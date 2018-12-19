#include "../classifier/maxentropy_classifier.h"

#include <limits>
#include <string>
#include <vector>
#include <fstream>

#include "../mylib/StringUtil.h"
#include "../classifier/feature_generator_factory.h"

using std::map;
using std::string;
using std::vector;
using std::ifstream;
//using namespace autil;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, MaxEntropyClassifier);

const int MaxEntropyClassifier::PER_LINEBUFF_SIZE = 1000;
const float MaxEntropyClassifier::OTHER_DOMAIN_SCORE = 1.0f;
const string MaxEntropyClassifier::OTHER_DOMAIN("other");
const string MaxEntropyClassifier::CLASS_ID_SEPEARTOR(" ");
const string MaxEntropyClassifier::SUPPORT_DOMAIN_SEPEARTOR(",");

/*
weather 1
telephone 2
map 3
music 4
...
joke 30
phone_charges 31
baike 32
*/
//��ȡ�������� �������id -> class��
bool MaxEntropyClassifier::LoadClassIDFile(const string& file_path)
{
	ifstream f_stream(file_path.c_str());
	char buffer[PER_LINEBUFF_SIZE];
	while (f_stream.getline(buffer, PER_LINEBUFF_SIZE)) //map 3
	{
		vector<string> classid_pair = StringUtil::split(buffer, CLASS_ID_SEPEARTOR);
		if (classid_pair.size() != 2)
		{
			LOG(ERROR, "classid_pair is not a pair, value:%s", buffer);
			continue;
		}
		class_id_map_[classid_pair[1]] = classid_pair[0]; //3 : map
		LOG(TRACE1, "key:%s, value:%s", classid_pair[1].c_str(), classid_pair[0].c_str());
	}
	f_stream.close();
	return true;
}

bool MaxEntropyClassifier::Init(const ClassifierConfig& config)
{
	// cache config
	config_ = config;

	// get max min ratio [Ĭ��9.0f]
	max_min_score_ratio_ = config_.GetMaxMinRatio();

	// get support domains [map,calendar,music,telephone]
	support_domain_set_.clear();
	string support_domain = config_.GetSupportDomain();// supported domain [ ��map,calendar,music,telephone�� ]
	vector<string> parsed_result = StringUtil::split(support_domain, SUPPORT_DOMAIN_SEPEARTOR);
	for (size_t i = 0; i < parsed_result.size(); ++i)
	{
		support_domain_set_.insert(parsed_result[i]);
	}

	// get class_id
	// ��ȡ�������� �������id -> class��
	// class_id_map_[id]  =  class
	string class_id_path = config_.GetClassIDPath();// class id path [./conf/class_id.txt]
	if (!LoadClassIDFile(class_id_path))//���֧�ֵķ������Map<string, string>  class_id_map_['2'] = telepthon
	{
		LOG(ERROR, "LoadClassIDFile failed");
		return false;
	}

	// load model
	//����MaxEntģ��
	/*

		���y    ����x	    Ȩ��W      ===>>    ����f ������Ĺ��̾��ǲ������Ĺ��̡�
		______________________________
		7		�ٷ�_��վ	0.477547
		14		��Ϣ_��		0.032232
		13		loc_��		-0.012150
		______________________________
	*/
	string model_path = config_.GetModelPath();// model path [./models/model]
	model_.load_from_file(model_path.c_str());

	// get feature generator
	// ��ȡ������ȡ��
	FeatureGeneratorFactory feature_generator_factory;//������ȡ������
	if (!feature_generator_factory.Init(config_))
	{
		LOG(INFO, "feature generator factory init failed");
		return false;
	}
	feature_generator_ = feature_generator_factory.CreateFeatureGenerator();//ͨ�����ò�����ȡ �����������ȡ����AbstractFeatureGenerator��
	if (NULL == feature_generator_)
	{
		LOG(INFO, "feature generator is NULL");
		return false;
	}

	return true;
}

bool MaxEntropyClassifier::Predict(const QpRequest& qp_request, QpResult* qp_result)
{
	if (NULL == qp_result)
	{
		LOG(ERROR, "qp_result is NULL");
		return false;
	}
	/*
	message FeatureElement {
		required string key = 1;	//����
		required uint32 count = 2;  //Ƶ��
	}

	message Feature {
		required string label = 1;
		repeated FeatureElement elements = 2;	//����
	}
	*/

	Feature feature;
	/*
	����   �����»�/[per singer]	/��	/��	/һ��	/����ˮ/[song]s��
	����				��һ�ס�
	�ʵ�bigram		�����ˡ�
	ʵ��ʵ�bigram	
		 ��ʼ		��S_per��  ��S_singer��
		 ����		��song_E��
	   ǰ���		��per_���� ��singer_���� ��һ��_song��
 	*/
	//<1>	������ȡ
	if (!GenerateFeature(qp_request, *qp_result, &feature))
	{
		return false;
	}

	//<2>	Ԥ������sample����
	ME_Sample sample;//Ԥ������
	for (int i = 0; i < feature.elements_size(); ++i)
	{
		sample.add_feature(feature.elements(i).key().c_str(), feature.elements(i).count()); //��� <feature : count(feature)>
	}

	// classifier predict first
	//<3>	����Ԥ�� - �������÷�
	vector<double> scores = model_.classify(sample);

	// get right class_id mapping
	//<4>	��ȡ���� id - lable
	string number_label = sample.label;
	map<string, string>::iterator iter = class_id_map_.find(number_label);
	if (iter == class_id_map_.end())
	{
		LOG(ERROR, "found mapping:%s in class_id_map failed", number_label.c_str());
		return false;
	}

	/*
	message DomainQpResult {
		required string domain = 1;			//���� [map, calendar, music, telephone�� other]
		optional float domain_score = 2;	//�÷�
		repeated NeTerm ne_terms = 3;		//MaxEntδ���
		required string ne_sentense = 4;	//MaxEntδ���
	}
	*/
	// check whether support current domain
	// ����Ƿ�֧�� ��ǰdomain �����
	DomainQpResult* domain_qp_result = qp_result->add_domain_qp_results(); // ֧�ֵ�domain  [map,calendar,music,telephone]
	if (support_domain_set_.find(iter->second) ==support_domain_set_.end())// ��֧��domain
	{
		domain_qp_result->set_domain(OTHER_DOMAIN);				//����domain = "other"
		domain_qp_result->set_domain_score(OTHER_DOMAIN_SCORE);	//����domain�ķ� = 1.0f;
		return true;//����True
	}

	/*
		weather 1
		telephone 2
		map 3
		music 4
		alarm 5
		app 6
		website 7
		hotel 8
		stock 9
		calendar 10
	*/
	domain_qp_result->set_domain(iter->second); // ���� ����

	// check max_min_score_ratio criteria
	// ���Ե÷���� �� ��͵ı���
	float max_score = std::numeric_limits<float>::min();
	float min_score = std::numeric_limits<float>::max();
	int max_idx = -1;
	int min_idx = -1;
	int expected_idx = -1; //����index
	for (int i = 0; i < model_.num_classes(); ++i)
	{
		if (0 == sample.label.compare(model_.get_class_label(i)))   //  <int>idx <--> <str>lable[id] -> domain
		{
			expected_idx = i; //expected_idx == int(sample.lable)  lable��string���ͣ�ת��������
		}
		if (scores[i] > max_score)
		{
			max_score = scores[i];
			max_idx = i;
		}
		if (scores[i] < min_score)
		{
			min_score = scores[i];
			min_idx = i;
		}
	}
	if (min_score != 0.0f && max_score < (min_score * max_min_score_ratio_)) // ���min_score == 0.0f  ?? max_score �ܴ� => ������Ҫ��
	{
		LOG(TRACE1, "failed to reach ratio:%f, max:%f, min:%f, max_idx:%d, min_idx:%d",
			max_min_score_ratio_, max_score, min_score, max_idx, min_idx);
		return false;
	}
	if (expected_idx < 0 || expected_idx != max_idx) // expected_idx == int(sample.lable) == max_idx ������
	{
		LOG(ERROR, "expected_idx:%d is minus or unexpected", expected_idx);
		return false;
	}
	domain_qp_result->set_domain_score(scores[expected_idx]); // ���õ÷�
	return true;
}

/*
message FeatureElement {
	required string key = 1;	//����
	required uint32 count = 2;  //Ƶ��
}

message Feature {
	required string label = 1;
	repeated FeatureElement elements = 2;	//����
}
*/
bool MaxEntropyClassifier::GenerateFeature(const QpRequest& qp_request,	const QpResult& qp_result,	Feature* feature)
{
	if (!feature_generator_->GenerateFeature(qp_request, qp_result, feature))
	{
		return false;
	}
	return true;
}

DIALOG_SEG_END_NAMESPACE(qp);
