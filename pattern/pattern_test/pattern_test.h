#ifndef PATTERN_PATTERN_TEST_PATTERN_TEST_H_
#define PATTERN_PATTERN_TEST_PATTERN_TEST_H_

#include <log.h>
#include <common_macros.h>

#include <string>

#include <cppunit/extensions/HelperMacros.h>


DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PatternTest);
  CPPUNIT_TEST(PattUnitTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  PatternTest();
  ~PatternTest();
  void PattUnitTest();

 private:
  DIALOG_SEG_LOG_DECLARE();

}; 

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // PATTERN_PATTERN_TEST_PATTERN_TEST_H_
