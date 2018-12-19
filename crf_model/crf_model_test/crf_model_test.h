#ifndef NLU_CRF_MODEL_CRF_MODEL_TEST_CRF_MODEL_TEST_H_
#define NLU_CRF_MODEL_CRF_MODEL_TEST_CRF_MODEL_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class CRFModelTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(CRFModelTest);
  CPPUNIT_TEST(CRFTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  CRFModelTest();
  ~CRFModelTest();

 public:
  void setUp();
  void tearDown();

 private:
  DIALOG_SEG_LOG_DECLARE();

 private:
  void CRFTest();
};  // CRFModelTest

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CRF_MODEL_CRF_MODEL_TEST_CRF_MODEL_TEST_H_
