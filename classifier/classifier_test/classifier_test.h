#ifndef NLU_CLASSIFIER_CLASSIFIER_TEST_CLASSIFIER_TEST_H_
#define NLU_CLASSIFIER_CLASSIFIER_TEST_CLASSIFIER_TEST_H_

//#include <log.h>
//#include <common_macros.h>

#include "../../../common_macros.h"
#include "../../../log.h"
#include <string>

#include <cppunit/extensions/HelperMacros.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class ClassifierTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ClassifierTest);
	CPPUNIT_TEST(FeatureGeneratorTest);
	CPPUNIT_TEST(MaxEntropyClassifierNormalTest);
	CPPUNIT_TEST(MaxEntropyClassifierUnsupportedTest);
	CPPUNIT_TEST_SUITE_END();

public:
	ClassifierTest();
	~ClassifierTest();

public:
	void setUp();
	void tearDown();

private:
	DIALOG_SEG_LOG_DECLARE();

private:
	void FeatureGeneratorTest();
	void MaxEntropyClassifierNormalTest();
	void MaxEntropyClassifierUnsupportedTest();
};  // ClassifierTest

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_CLASSIFIER_TEST_CLASSIFIER_TEST_H_
