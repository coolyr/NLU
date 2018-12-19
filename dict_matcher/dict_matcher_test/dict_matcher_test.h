#ifndef NLU_DICT_MATCHER_DICT_MATCHER_TEST_DICT_MATCHER_TEST_H_
#define NLU_DICT_MATCHER_DICT_MATCHER_TEST_DICT_MATCHER_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class DictMatcherTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DictMatcherTest);
  CPPUNIT_TEST(DictMatcherNormalTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  DictMatcherTest();
  ~DictMatcherTest();

 public:
  void setUp();
  void tearDown();

 private:
  DIALOG_SEG_LOG_DECLARE();

 private:
  void DictMatcherNormalTest();
};  // DictMatcherTest

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_DICT_MATCHER_DICT_MATCHER_TEST_DICT_MATCHER_TEST_H_
