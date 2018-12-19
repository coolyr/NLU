#ifndef NLU_PROCESSER_NLU_PROCESSER_TEST_NLU_PROCESSER_TEST_H_
#define NLU_PROCESSER_NLU_PROCESSER_TEST_NLU_PROCESSER_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class NLUProcesserTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(NLUProcesserTest);
  CPPUNIT_TEST(NLUProcesserTestNormalTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  NLUProcesserTest();
  ~NLUProcesserTest();

 public:
  void setUp();
  void tearDown();

 private:
  DIALOG_SEG_LOG_DECLARE();

 private:
  void NLUProcesserTestNormalTest();
};  // NLUProcesserTest

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_PROCESSER_NLU_PROCESSER_TEST_NLU_PROCESSER_TEST_H_
