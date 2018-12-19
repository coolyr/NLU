#include "nlu_test/nlu_test.h" 

#include <assert.h>
#include <cppunit/TestAssert.h>

#include <iostream>
#include <fstream>
#include "proto/qp.pb.h"
#include "proto/nlu_dm.pb.h"
#include "crf_model/crf_model.h"
#include "wordseg/word_segmenter.h"
#include "dict_matcher/dict_matcher.h"
#include "post_processer/post_processer.h"
#include "classifier/classifier_factory.h"
#include "classifier/feature_generator_factory.h"
#include "pattern/pattern_engine.h" 
#include "log.h"
#include "nlu_processer/nlu_processer.h"

using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, NluTest);

CPPUNIT_TEST_SUITE_REGISTRATION(NluTest);

NluTest::NluTest() {
}

NluTest::~NluTest() {
}


void NluTest::NluUnitTest() {

  string log_cfg("./conf/log_cfg");
  LOG_CONFIG(log_cfg.c_str());

  ws::AliTokenizerFactory* factory = new ws::AliTokenizerFactory();
  bool ret1 = factory->Init("./conf/AliTokenizer.conf");
  if (ret1 == false) {
    LOG(ERROR, "can not static init word seg factory");
    return;
  }
  NLUProcesser nlu_processer;
  int ret = nlu_processer.Init(factory, "./conf/nlu.conf");
  if (ret != 0) {
    LOG(ERROR, "can not init nlu processer");
    return;
  }
  std::ifstream ifs("./data_test/test.txt", std::ifstream::in);
  std::string query;
  while (std::getline(ifs, query)) {
    QpRequest qp_request;
    qp_request.set_query(query);
    NluResult nlu_result;
    ret = nlu_processer.Process(qp_request, &(nlu_result));
    if (ret != 0) {
      LOG(ERROR, "nlu   process failed");
      return;
    }
    LOG(INFO, "nlu result is %s", nlu_result.Utf8DebugString().c_str());
  }
  ifs.close();
  if (factory) {
    delete factory;
  }

  LOG(INFO, "result:%s, expected:%s", "te", "st");
}

DIALOG_SEG_END_NAMESPACE(qp);
