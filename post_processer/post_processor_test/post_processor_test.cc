#include "post_processer/post_processor_test/post_processor_test.h"
#include <assert.h>
#include <cppunit/TestAssert.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "post_processer/post_processer.h"

using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, PostProcessorTest);

CPPUNIT_TEST_SUITE_REGISTRATION(PostProcessorTest);

PostProcessorTest::PostProcessorTest() {}

PostProcessorTest::~PostProcessorTest() {}

void PostProcessorTest::setUp() {}

void PostProcessorTest::tearDown() {}

void PostProcessorTest::PostProcessorNormalTest() {
  PostProcesser post_processer;
  CPPUNIT_ASSERT(0 == post_processer.Init("./conf/nlu.conf"));
}

DIALOG_SEG_END_NAMESPACE(qp);
