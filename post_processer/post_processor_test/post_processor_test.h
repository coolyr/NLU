#ifndef NLU_POST_PROCESSOR_POST_PROCESSOR_TEST_POST_PROCESSOR_TEST_H_
#define NLU_POST_PROCESSOR_POST_PROCESSOR_TEST_POST_PROCESSOR_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PostProcessorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PostProcessorTest);
  CPPUNIT_TEST(PostProcessorNormalTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  PostProcessorTest();
  ~PostProcessorTest();

 public:
  void setUp();
  void tearDown();

 private:
  DIALOG_SEG_LOG_DECLARE();

 private:
  void PostProcessorNormalTest();
};  // PostProcessorTest

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_POST_PROCESSOR_POST_PROCESSOR_TEST_POST_PROCESSOR_TEST_H_
