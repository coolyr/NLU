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
	//#######################################分词器####################################################
	word_segmenter_ = new WordSegmenter();//分词器，包装类。内部封装阿里分词器[AliTokenizerFactory]
	int ret = word_segmenter_->Init(factory, cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "WordSegmenter init failed");
		return -1;
	}
	//#######################################实体识别器################################################
	crf_model_ = new CRFModel();// CRF，包装类。内部封装CRF++， 加载模型二进制：crf_0524.model
	ret = crf_model_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "CrfModel init failed");
		return -1;
	}
	//#######################################词典匹配###################################################
	dict_matcher_ = new DictMatcher();
	ret = dict_matcher_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "DictMatcher init failed");
		return -1;
	}
	//#########################################分类器###################################################
	ClassifierConfig classifier_config;//分类器的配置参数
	if (!classifier_config.Init(cfg_path))
	{
		LOG(ERROR, "classifier config init failed");
		return -1;
	}
	ClassifierFactory classifier_factory;//分类器工厂,持有配置参数类：ClassifierConfig
	if (!classifier_factory.Init(classifier_config))
	{
		LOG(ERROR, "classifier factory init failed");
		return -1;
	}
	classifier_ = classifier_factory.CreateClassifier();//根据相应的配置，创建并返回分类器：MaxEntropyClassifier
	if (!classifier_)
	{
		LOG(ERROR, "classifier get failed");
		return -1;
	}
	if (!classifier_->Init(classifier_config))//根据配置参数，初始化分类器【MaxEntropyClassifier】
	{
		LOG(ERROR, "classifier init failed");
		return -1;
	}
	//########################################后处理器##################################################
	post_processer_ = new PostProcesser();
	ret = post_processer_->Init(cfg_path);
	if (ret != 0)
	{
		LOG(ERROR, "Post processer init failed");
		return -1;
	}
	//########################################模板匹配器#######################################################
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
	required string query = 1;   	//原始query
	optional Session session = 2;	//会话session【与SDS(总控模块)交互的session ID,是通过socket通信的】
}

message NluResult {
	required string query = 1;		//原始query  raw_query
	optional string seg = 2;		//空格隔开的切词  seg_sen = "导航 去 五道口"
	repeated Domain domains = 3;	//领域 【list】
}
*/

int NLUProcesser::Process(const QpRequest& qp_request, NluResult* nlu_result)
{
	/*
		message QpResult {
			required string raw_query = 1;		//原始query = QpRequest.query
			optional string corrected_query = 2;
			repeated Term terms = 3;			//检索粒度切词 【WordSegmenter类完成】
			repeated NeTerm ne_terms = 4;		//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;
		}
	*/
	QpResult* qp_result = new QpResult();
	qp_result->set_raw_query(qp_request.query()); //QpResult.raw_query = QpRequest.query

	//<1>	把query切完词后，添加到qp_result中的Term对象中
	int ret = word_segmenter_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "word segmenter process failed");
		return -1;
	}
	//<2>	识别出实体，添加到qp_result中的NeTerm
	ret = crf_model_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "CRF Model process failed");
		return -1;
	}

	/*
	<3>
		O(n2)遍历切词的每种可能性，任意的 【i - j】 的切词组合， 然后去实体字典【singer,song,app,website】中去检查是否是这四类实体词，
		可能【i-j】会从实体词典中匹配出多个结果
		如果是，加入qp_result.NeTerm
	*/
	ret = dict_matcher_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "Dict matcher process failed");
		return -1;
	}
	/*
		message DomainQpResult {
			required string domain = 1;		 //域名 [map, calendar, music, telephone， other]
			optional float domain_score = 2; //得分
			repeated NeTerm ne_terms = 3;	 //MaxEnt未填充 [后处理器填充， 填充实体中所有属于当前domain的实体词]
			required string ne_sentense = 4; //MaxEnt未填充 [后处理器填充， 把query中属于当前domain的实体词，用识别出来的实体词，进行标记后的query]  => '导航去/o五道口/poi(/o)'
		}
	*/

	//<4>	分类只给出了一类，加入到了qp_result.DomainQpResult 
	//		修改思路： 给出支持domain内的多个类别
	if (!classifier_->Predict(qp_request, qp_result))
	{
		LOG(ERROR, "classify failed");
		return -1;
	}
	/*
		message DomainQpResult {
			required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
			optional float domain_score = 2;	//MaxEnt对当前类别得分
			repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
			required string ne_sentense = 4;	//MaxEnt未填充 ["我想听/o刘德华/singer的/o忘情水/song(/o)"]
		}
	*/
	//<5>	按领域识别结果，使用领域的后处理器，匹配特定领域的实体词
	ret = post_processer_->Process(qp_request, qp_result);
	if (ret != 0)
	{
		LOG(ERROR, "Post processer process failed");
		return -1;
	}
	/*
		message QpResult {
			required string raw_query = 1;					//原始query = QpRequest.query
			optional string corrected_query = 2;			//PostProcesser处理完后，该字段还没有填充！？
			repeated Term terms = 3;						//检索粒度切词 【WordSegmenter类完成】
			repeated NeTerm ne_terms = 4;					//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
			repeated DomainQpResult domain_qp_results = 5;	//MaxEnt分类结果
		}
	*/
	//<6>	按领域识别结果，进行模板匹配 => 设置为True!!
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
	<pattern type="EXACT" rule="订一张@<date>去@<loc>的火车票">^订一张.*去.*的火车票$</pattern>
	<semantics>
		<domain>map</domain>
		<intent>book_train_ticket</intent>
	</semantics>
</template>

思想：动态加载用户自定义模板
	<1>必须指定NLU支持的inent,
	<2>Map对象覆盖存储，所以要想保存用户的自定义模板必须保存一份前端的定义数据。
	【map_synx_rule_ || map_synx_template_ || map_synx_rule_】
	<3>所以前段修改了自定义模板，其实是覆盖的方式起作用的。
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
