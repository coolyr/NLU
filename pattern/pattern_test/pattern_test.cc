#include "pattern/pattern_test/pattern_test.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
DIALOG_SEG_LOG_SETUP(qp, PatternTest);

CPPUNIT_TEST_SUITE_REGISTRATION(PatternTest);

PatternTest::PatternTest() {
}

PatternTest::~PatternTest() {
}


void PatternTest::PattUnitTest() {

  LOG(INFO, "===NLUResult is: ");
  string log_cfg("./conf/log_cfg");
  LOG_CONFIG(log_cfg.c_str());
  PatternEngine* pattern_engine_ = new PatternEngine();
  if (pattern_engine_->Init("./conf/nlu.conf") != 0) {
    LOG(ERROR, "pattern engine inti failed");
    return;
  }
  NluResult nlu_result;
  QpResult* qp_result = new QpResult();
  
  int fd = open("data_test/seri.res", O_RDONLY);
  qp_result->ParseFromFileDescriptor(fd);
  close(fd);
  int ret = pattern_engine_->Process(qp_result, nlu_result, false);
  if (ret != 0) {
    LOG(ERROR, "pattern engine process failed");
    return;
  }
  if (qp_result) {
    delete qp_result;
  }
  LOG(INFO, "nlu result is %s", nlu_result.Utf8DebugString().c_str());

  if (pattern_engine_) {
    pattern_engine_->Destroy();
    delete pattern_engine_;
  }


  LOG(INFO, "result:%s, expected:%s", "te", "st");
}

DIALOG_SEG_END_NAMESPACE(qp);
