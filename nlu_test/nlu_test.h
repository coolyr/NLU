#ifndef PATTERN_PATTERN_TEST_PATTERN_TEST_H_
#define PATTERN_PATTERN_TEST_PATTERN_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>


DIALOG_SEG_BEGIN_NAMESPACE(qp);

class NluTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(NluTest);
  CPPUNIT_TEST(NluUnitTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  NluTest();
  ~NluTest();
  void NluUnitTest();

 private:
  DIALOG_SEG_LOG_DECLARE();

}; 

DIALOG_SEG_END_NAMESPACE(qp);

#endif  
