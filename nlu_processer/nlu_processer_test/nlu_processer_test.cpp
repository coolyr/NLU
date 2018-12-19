#include "nlu_processer/nlu_processer_test/nlu_processer_test.h"
#include <assert.h>
#include <cppunit/TestAssert.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "nlu_processer/nlu_processer.h"
#include "wordseg/word_segmenter.h"

using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, NLUProcesserTest);

CPPUNIT_TEST_SUITE_REGISTRATION(NLUProcesserTest);

NLUProcesserTest::NLUProcesserTest() {}

NLUProcesserTest::~NLUProcesserTest() {}

void NLUProcesserTest::setUp() {}

void NLUProcesserTest::tearDown() {}

void NLUProcesserTest::NLUProcesserTestNormalTest() {
  ws::AliTokenizerFactory* factory = new ws::AliTokenizerFactory();
  CPPUNIT_ASSERT(factory->Init("./conf/AliTokenizer.conf"));

  NLUProcesser nlu_processer;
  CPPUNIT_ASSERT(0 == nlu_processer.Init(factory, "./conf/nlu.conf"));

  std::string query = "找个超市";
  QpRequest qp_request;
  qp_request.set_query(query);
  NluResult nlu_result;
  CPPUNIT_ASSERT(0 == nlu_processer.Process(qp_request, &(nlu_result)));
  LOG(INFO, "nlu result is %s", nlu_result.Utf8DebugString().c_str());

  string session, context_str, context_cache_str, nlu_result_str;
  CPPUNIT_ASSERT(0 == nlu_processer.Process(
      session, context_str, query, context_cache_str, nlu_result_str));
  if (factory) {
    delete factory;
    factory = NULL;
  }
}

DIALOG_SEG_END_NAMESPACE(qp);
