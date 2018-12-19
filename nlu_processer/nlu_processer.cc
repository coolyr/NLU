#include "nlu_processer.h"
#include "../classifier/classifier_config.h"
#include "../classifier/classifier_factory.h"
#include "../wordseg/word_segmenter.h"
#include "../crf_model/crf_model.h"
#include "../dict_matcher/dict_matcher.h"
#include "../classifier/abstract_classifier.h"
#include "../post_processer/post_processer.h"
#include "../pattern/pattern_engine.h"
#include "../log.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, NLUProcesser);

using std::string;
NLUProcesser::NLUProcesser() : word_segmenter_(NULL),
crf_model_(NULL),
dict_matcher_(NULL),
classifier_(NULL),
post_processer_(NULL),
pattern_engine_(NULL)
{
}

NLUProcesser::~NLUProcesser()
{
	if (word_segmenter_)
	{
		delete word_segmenter_;
		word_segmenter_ = NULL;
	}
	if (crf_model_)
	{
		delete crf_model_;
		crf_model_ = NULL;
	}
	if (dict_matcher_)
	{
		delete dict_matcher_;
		dict_matcher_ = NULL;
	}
	if (classifier_)
	{
		delete classifier_;
		classifier_ = NULL;
	}
	if (post_processer_)
	{
		delete post_processer_;
		post_processer_ = NULL;
	}
	if (pattern_engine_)
	{
		pattern_engine_->Destroy();
		delete pattern_engine_;
		pattern_engine_ = NULL;
	}
}

int NLUProcesser::Init(ws::AliTokenizerFactory* factory, const string& cfg_path)
{
	//#######################################�ִ���####################################################
	word_segmenter_ = new WordSegmenter();//�ִ�������װ�ࡣ�ڲ���װ����ִ���[AliTokenizerFactory]
	int ret = word_segmenter_->Init(factory, cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "WordSegmenter init failed");
		return -1;
	}
	//#######################################ʵ��ʶ����################################################
	crf_model_ = new CRFModel();// CRF����װ�ࡣ�ڲ���װCRF++�� ����ģ�Ͷ����ƣ�crf_0524.model
	ret = crf_model_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "CrfModel init failed");
		return -1;
	}
	//#######################################�ʵ�ƥ��###################################################
	dict_matcher_ = new DictMatcher();
	ret = dict_matcher_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "DictMatcher init failed");
		return -1;
	}
	//#########################################������###################################################
	ClassifierConfig classifier_config;//�����������ò���
	if (!classifier_config.Init(cfg_path))
	{
		LOG(ERROR, "classifier config init failed");
		return -1;
	}
	ClassifierFactory classifier_factory;//����������,�������ò����ࣺClassifierConfig
	if (!classifier_factory.Init(classifier_config))
	{
		LOG(ERROR, "classifier factory init failed");
		return -1;
	}
	classifier_ = classifier_factory.CreateClassifier();//������Ӧ�����ã����������ط�������MaxEntropyClassifier
	if (!classifier_)
	{
		LOG(ERROR, "classifier get failed");
		return -1;
	}
	if (!classifier_->Init(classifier_config))//�������ò�������ʼ����������MaxEntropyClassifier��
	{
		LOG(ERROR, "classifier init failed");
		return -1;
	}
	//########################################������##################################################
	post_processer_ = new PostProcesser();
	ret = post_processer_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "Post processer init failed");
		return -1;
	}
	//########################################ģ��ƥ����#######################################################
	pattern_engine_ = new PatternEngine();
	if (pattern_engine_->Init(cfg_path.c_str()) != 0)
	{
		LOG(ERROR, "pattern engine inti failed");
		return -1;
	}
	return 0;
}
int NLUProcesser::Process(const string session_str, const string context_str, const string query, string& context_cache_str, string& nlu_result_str)
{
	QpRequest qp_request;
	qp_request.set_query(query);

	NluResult nlu_result;
	nlu_result.set_query(query);
	int ret = Process(qp_request, &nlu_result);
	if (ret != 0)
	{
		return -1;
	}
	int ret_p = nlu_result.SerializeToString(&nlu_result_str);
	if (!ret_p)
	{
		return -1;
	}
	return 0;
}

/*
message QpRequest {
	required string query = 1;   	//ԭʼquery
	optional Session session = 2;	//�Ựsession����SDS(�ܿ�ģ��)������session ID,��ͨ��socketͨ�ŵġ�
}

message NluResult {
	required string query = 1;		//ԭʼquery  raw_query
	optional string seg = 2;		//�ո�������д�  seg_sen = "���� ȥ �����"
	repeated Domain domains = 3;	//���� ��list��
}
*/

int NLUProcesser::Process(const QpRequest& qp_request, NluResult* nlu_result)
{
	/*
		message QpResult {
			required string raw_query = 1;		//ԭʼquery = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;			//���������д� ��WordSegmenter����ɡ�
			repeated NeTerm ne_terms = 4;		//ʵ��CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;
		}
	*/
	QpResult* qp_result = new QpResult();
	qp_result->set_raw_query(qp_request.query()); //QpResult.raw_query = QpRequest.query

	//<1>	��query����ʺ���ӵ�qp_result�е�Term������
	int ret = word_segmenter_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "word segmenter process failed");
		return -1;
	}
	//<2>	ʶ���ʵ�壬��ӵ�qp_result�е�NeTerm
	ret = crf_model_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "CRF Model process failed");
		return -1;
	}

	/*
	<3>
		O(n2)�����дʵ�ÿ�ֿ����ԣ������ ��i - j�� ���д���ϣ� Ȼ��ȥʵ���ֵ䡾singer,song,app,website����ȥ����Ƿ���������ʵ��ʣ�
		���ܡ�i-j�����ʵ��ʵ���ƥ���������
		����ǣ�����qp_result.NeTerm
	*/
	ret = dict_matcher_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "Dict matcher process failed");
		return -1;
	}
	/*
		message DomainQpResult {
			required string domain = 1;		 //���� [map, calendar, music, telephone�� other]
			optional float domain_score = 2; //�÷�
			repeated NeTerm ne_terms = 3;	 //MaxEntδ��� [��������䣬 ���ʵ�����������ڵ�ǰdomain��ʵ���]
			required string ne_sentense = 4; //MaxEntδ��� [��������䣬 ��query�����ڵ�ǰdomain��ʵ��ʣ���ʶ�������ʵ��ʣ����б�Ǻ��query]  => '����ȥ/o�����/poi(/o)'
		}
	*/

	//<4>	����ֻ������һ�࣬���뵽��qp_result.DomainQpResult 
	//		�޸�˼·�� ����֧��domain�ڵĶ�����
	if (!classifier_->Predict(qp_request, qp_result))
	{
		LOG(ERROR, "classify failed");
		return -1;
	}
	/*
		message DomainQpResult {
			required string domain = 1;			//���� [ music]  ==> "�������»�/[per singer]������ˮ/song"
			optional float domain_score = 2;	//MaxEnt�Ե�ǰ���÷�
			repeated NeTerm ne_terms = 3;		//MaxEntδ��� [��������� : ���»�/singer   ����ˮ/song]
			required string ne_sentense = 4;	//MaxEntδ��� ["������/o���»�/singer��/o����ˮ/song(/o)"]
		}
	*/
	//<5>	������ʶ������ʹ������ĺ�������ƥ���ض������ʵ���
	ret = post_processer_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "Post processer process failed");
		return -1;
	}
	/*
		message QpResult {
			required string raw_query = 1;					//ԭʼquery = QpRequest.query
			optional string corrected_query = 2;			//PostProcesser������󣬸��ֶλ�û����䣡��
			repeated Term terms = 3;						//���������д� ��WordSegmenter����ɡ�
			repeated NeTerm ne_terms = 4;					//ʵ��CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;	//MaxEnt������
		}
	*/
	//<6>	������ʶ����������ģ��ƥ�� => ����ΪTrue!!
	//		ret = pattern_engine_->Process(qp_result, *nlu_result, true);
	ret = pattern_engine_->Process(qp_result, *nlu_result, false);
	if (ret != 0)
	{
		LOG(ERROR, "pattern process failed");
		return -1;
	}
	LOG(INFO, "qp reqsult is: %s", qp_result->Utf8DebugString().c_str());
	if (qp_result != NULL)
	{
		delete qp_result;
		qp_result = NULL;
	}

	return 0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
<template id="BOOK_TRAIN_TICKET">
	<pattern type="EXACT" rule="��һ��@<date>ȥ@<loc>�Ļ�Ʊ">^��һ��.*ȥ.*�Ļ�Ʊ$</pattern>
	<semantics>
		<domain>map</domain>
		<intent>book_train_ticket</intent>
	</semantics>
</template>

˼�룺��̬�����û��Զ���ģ��
	<1>����ָ��NLU֧�ֵ�inent,
	<2>Map���󸲸Ǵ洢������Ҫ�뱣���û����Զ���ģ����뱣��һ��ǰ�˵Ķ������ݡ�
	��map_synx_rule_ || map_synx_template_ || map_synx_rule_��
	<3>����ǰ���޸����Զ���ģ�壬��ʵ�Ǹ��ǵķ�ʽ�����õġ�
*/

int NLUProcesser::AddRules(const std::string domain, const std::string intent, const std::set<std::string> rules)
{
	int ret = pattern_engine_->ModifyTemplatesByRule(domain, intent, rules);
	if (ret != 0)
	{
		LOG(ERROR, "add rule failed");
		return -1;
	}
	return 0;
}
DIALOG_SEG_END_NAMESPACE(qp);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
