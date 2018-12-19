#include <iostream>
#include <fstream>
#include "../proto/qp.pb.h"
#include "../proto/nlu_dm.pb.h"
#include "../crf_model/crf_model.h"
#include "../wordseg/word_segmenter.h"
#include "../dict_matcher/dict_matcher.h"
#include "../post_processer/post_processer.h"
#include "../classifier/classifier_factory.h"
#include "../classifier/feature_generator_factory.h"
#include "../pattern/pattern_engine.h" 
#include "../log.h"
#include "../nlu_processer/nlu_processer.h"
#include "../mylib/Logger.h"
#include "../mylib/ali_tokenizer.h"
#include "../mylib/ali_tokenizer_define.h"

using namespace dialog::qp;
using namespace std;

DECLARE_AND_SETUP_LOGGER(main);

int main(int argc, char* argv[])
{
	string log_cfg("./conf/log_cfg");
	LOG_CONFIG(log_cfg.c_str());

	/* ws::AliTokenizerFactory* factory = new ws::AliTokenizerFactory();
	 bool ret = factory->Init("./conf/AliTokenizer.conf");
	 if (ret == false) {
	 LOG(ERROR, "can not static init word seg factory");
	 return -1;
	 }

	 WordSegmenter word_segmenter;
	 int ret2 = word_segmenter.Init(factory, "./conf/nlu.conf");
	 if (ret2 != 0) {
	 LOG(ERROR, "can not init wordseg");
	 return -1;
	 }

	 DictMatcher dict_matcher;
	 ret2 = dict_matcher.Init("./conf/nlu.conf");
	 if (ret2 != 0) {
	 LOG(ERROR, "can not init dict matcher");
	 return -1;
	 }

	 CRFModel crf_model;
	 if (crf_model.Init("./conf/nlu.conf") != 0) {
	 LOG(ERROR, "load crf model fail");
	 return -1;
	 }

	 PostProcesser post_processer;
	 ret2 = post_processer.Init("./conf/nlu.conf");
	 if (ret2 != 0) {
	 LOG(ERROR, "init post processer fail");
	 return -1;
	 }

	 QpRequest qp_request;
	 qp_request.set_query("我爱北京天安门");
	 QpResult* qp_result = new QpResult();
	 qp_result->set_raw_query("我要去和平饭店");
	 ret2 = word_segmenter.Process(qp_request, qp_result);
	 if (ret2 != 0) {
	 LOG(ERROR, "word seg fail");
	 return -1;
	 }
	 ret2 = crf_model.Process(qp_request, qp_result);
	 if (ret2 != 0) {
	 LOG(ERROR, "crf process fail");
	 return -1;
	 }
	 ret2 = dict_matcher.Process(qp_request, qp_result);
	 if (ret2 != 0) {
	 LOG(ERROR, "dict matcher process fail");
	 return -1;
	 }


	 // classify
	 ClassifierConfig config;
	 string cfg_path = "./conf/nlu.conf";
	 if (NULL == config.Init(cfg_path)) {
	 LOG(ERROR, "config init failed");
	 return -1;
	 }
	 ClassifierFactory  classifier_factory;
	 if (!classifier_factory.Init(config)) {
	 LOG(ERROR, "classifier_factory init failed");
	 return -1;
	 }
	 AbstractClassifier* classifier = NULL;
	 classifier = classifier_factory.CreateClassifier();
	 if (NULL == classifier) {
	 LOG(ERROR, "Classify create failure");
	 return -1;
	 }
	 if (!classifier->Init(config)) {
	 LOG(ERROR, "classifier init failure");
	 return -1;
	 }

	 if (!classifier->Predict(qp_request, qp_result)) {
	 LOG(ERROR, "Classify failure");
	 return -1;
	 }

	 ret2 = post_processer.Process(qp_request, qp_result);
	 LOG(INFO, "qp result is %s", qp_result->Utf8DebugString().c_str());

	 if (classifier) {
	 delete classifier;
	 classifier = NULL;
	 }

	 PatternEngine pe;
	 pe.Init("./conf/nlu.conf");
	 NluResult nr;
	 pe.Process(qp_result, nr,false);
	 LOG(INFO, "NLUResult is: %s", nr.Utf8DebugString().c_str());
	 pe.Destroy();

	 if (qp_result != NULL) {
	 delete qp_result;
	 }*/
	ws::AliTokenizerFactory* factory = new ws::AliTokenizerFactory();
	bool ret1 = factory->Init("./conf/AliTokenizer.conf");
	if (ret1 == false)
	{
		LOG(ERROR, "can not static init word seg factory");
		return -1;
	}
	NLUProcesser nlu_processer;
	int ret = nlu_processer.Init(factory, "./conf/nlu.conf");
	if (ret != 0)
	{
		LOG(ERROR, "can not init nlu processer");
		return -1;
	}
	std::ifstream ifs("./data_test/test.txt", std::ifstream::in);
	std::string query;
	while (std::getline(ifs, query))
	{
		/*
		message QpRequest {
			required string query = 1;   	//原始query
			optional Session session = 2;	//会话session
		}
		message Session {
			optional QpResult qp_result = 1;
		}

		*/
		QpRequest qp_request;	//原始输入query
		//qp_request.set_query("五道口附近的东方宫牛肉拉面");
		qp_request.set_query(query);
		NluResult nlu_result;	//返回的NLU解析结果
		ret = nlu_processer.Process(qp_request, &(nlu_result));
		if (ret != 0)
		{
			LOG(ERROR, "nlu   process failed");
			return -1;
		}
		LOG(INFO, "nlu result is %s", nlu_result.Utf8DebugString().c_str());
	}
	ifs.close();

	return 0;
}
