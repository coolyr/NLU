#include "dict_matcher/dict_matcher_test/dict_matcher_test.h"
#include <assert.h>
#include <cppunit/TestAssert.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "dict_matcher/dict_matcher.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, DictMatcherTest);

CPPUNIT_TEST_SUITE_REGISTRATION(DictMatcherTest);

DictMatcherTest::DictMatcherTest() {}

DictMatcherTest::~DictMatcherTest() {}

void DictMatcherTest::setUp() {}

void DictMatcherTest::tearDown() {}

void DictMatcherTest::DictMatcherNormalTest() {
  DictMatcher dict_matcher;
  CPPUNIT_ASSERT(0 == dict_matcher.Init("conf/dict.txt"));
}

DIALOG_SEG_END_NAMESPACE(qp);
