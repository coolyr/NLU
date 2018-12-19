#include "../../classifier/classifier_test/classifier_test.h"
#include <assert.h>
#include <cppunit/TestAssert.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "../../classifier/classifier_factory.h"
#include "../../classifier/feature_generator_factory.h"

using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, ClassifierTest);

CPPUNIT_TEST_SUITE_REGISTRATION(ClassifierTest);

ClassifierTest::ClassifierTest() {}

ClassifierTest::~ClassifierTest() {}

void ClassifierTest::setUp() {}

void ClassifierTest::tearDown() {}

void ClassifierTest::FeatureGeneratorTest()
{
	ClassifierConfig config;
	string cfg_path = "./conf/classifier_test.conf";
	CPPUNIT_ASSERT(true == config.Init(cfg_path));

	FeatureGeneratorFactory  feature_generator_factory;
	CPPUNIT_ASSERT(true == feature_generator_factory.Init(config));

	AbstractFeatureGenerator* feature_generator = NULL;
	feature_generator = feature_generator_factory.CreateFeatureGenerator();
	CPPUNIT_ASSERT(NULL != feature_generator);

	QpRequest request;
	QpResult result;
	Feature feature;
	//request.set_query("给我查现在是几点");
	request.set_query("农历多少号");
	Term* term = result.add_terms();
	string text1 = "农历";
	term->set_text(text1);
	term->set_begin(0);
	term->set_end(text1.size());

	term = result.add_terms();
	string text2 = "多少";
	term->set_text(text2);
	term->set_begin(text1.size());
	term->set_end(text1.size() + text2.size());

	term = result.add_terms();
	string text3 = "号";
	term->set_text(text3);
	term->set_begin(text1.size() + text2.size());
	term->set_end(text1.size() + text2.size() + text3.size());

	NeTerm* ne_term = result.add_ne_terms();
	string ne_text = "app";
	ne_term->set_ne_type(ne_text);
	ne_term->set_begin(0);
	ne_term->set_end(text1.size());

	CPPUNIT_ASSERT(false == feature_generator->GenerateFeature(
		request, result, NULL));
	CPPUNIT_ASSERT(true == feature_generator->GenerateFeature(
		request, result, &feature));
	CPPUNIT_ASSERT(7 == feature.elements_size());
	for (int i = 0; i < feature.elements_size(); ++i)
	{
		LOG(INFO, "idx:%d, value:%s, count:%ud",
			i, feature.elements(i).key().c_str(), feature.elements(i).count());
	}

	if (NULL != feature_generator)
	{
		delete feature_generator;
		feature_generator = NULL;
	}
}

void ClassifierTest::MaxEntropyClassifierNormalTest()
{
	ClassifierConfig config;
	string cfg_path = "./conf/classifier_test.conf";
	CPPUNIT_ASSERT(true == config.Init(cfg_path));
	ClassifierFactory  classifier_factory;
	CPPUNIT_ASSERT(true == classifier_factory.Init(config));

	AbstractClassifier* classifier = NULL;
	classifier = classifier_factory.CreateClassifier();
	CPPUNIT_ASSERT(NULL != classifier);


	CPPUNIT_ASSERT(true == classifier->Init(config));

	QpRequest request;
	QpResult result;
	//request.set_query("给我查现在是几点");
	request.set_query("农历多少号");
	Term* term = result.add_terms();
	string text1 = "农历";
	term->set_text(text1);
	term->set_begin(0);
	term->set_end(text1.size());

	term = result.add_terms();
	string text2 = "多少";
	term->set_text(text2);
	term->set_begin(text1.size());
	term->set_end(text1.size() + text2.size());

	term = result.add_terms();
	string text3 = "号";
	term->set_text(text3);
	term->set_begin(text1.size() + text2.size());
	term->set_end(text1.size() + text2.size() + text3.size());

	NeTerm* ne_term = result.add_ne_terms();
	string ne_text = "app";
	ne_term->set_ne_type(ne_text);
	ne_term->set_begin(0);
	ne_term->set_end(text1.size());

	string expected_domain = "calendar";
	CPPUNIT_ASSERT(false == classifier->Predict(request, NULL));
	CPPUNIT_ASSERT(true == classifier->Predict(request, &result));
	CPPUNIT_ASSERT(result.domain_qp_results_size() > 0);
	CPPUNIT_ASSERT(0 == expected_domain.compare(
		result.domain_qp_results(0).domain()));
	for (int i = 0; i < result.domain_qp_results_size(); ++i)
	{
		LOG(INFO, "id:%d, domain:%s, score:%f",
			i, result.domain_qp_results(i).domain().c_str(),
			result.domain_qp_results(i).domain_score());
	}

	if (NULL != classifier)
	{
		delete classifier;
		classifier = NULL;
	}
}

void ClassifierTest::MaxEntropyClassifierUnsupportedTest()
{
	ClassifierConfig config;
	string cfg_path = "./conf/classifier_test.conf";
	CPPUNIT_ASSERT(true == config.Init(cfg_path));
	ClassifierFactory  classifier_factory;
	CPPUNIT_ASSERT(true == classifier_factory.Init(config));

	AbstractClassifier* classifier = NULL;
	classifier = classifier_factory.CreateClassifier();
	CPPUNIT_ASSERT(NULL != classifier);


	CPPUNIT_ASSERT(true == classifier->Init(config));

	QpRequest request;
	QpResult result;
	//request.set_query("给我查现在是几点");
	request.set_query("明天冷吗");
	Term* term = result.add_terms();
	string text1 = "明天";
	term->set_text(text1);
	term->set_begin(0);
	term->set_end(text1.size());

	term = result.add_terms();
	string text2 = "冷";
	term->set_text(text2);
	term->set_begin(text1.size());
	term->set_end(text1.size() + text2.size());

	term = result.add_terms();
	string text3 = "吗";
	term->set_text(text3);
	term->set_begin(text1.size() + text2.size());
	term->set_end(text1.size() + text2.size() + text3.size());

	NeTerm* ne_term = result.add_ne_terms();
	string ne_text = "time";
	ne_term->set_ne_type(ne_text);
	ne_term->set_begin(0);
	ne_term->set_end(text1.size());

	string expected_domain = "other";
	CPPUNIT_ASSERT(false == classifier->Predict(request, NULL));
	CPPUNIT_ASSERT(true == classifier->Predict(request, &result));
	CPPUNIT_ASSERT(result.domain_qp_results_size() > 0);
	//CPPUNIT_ASSERT(0 == expected_domain.compare(
	//                    result.domain_qp_results(0).domain()));
	for (int i = 0; i < result.domain_qp_results_size(); ++i)
	{
		LOG(INFO, "id:%d, domain:%s, score:%f",
			i, result.domain_qp_results(i).domain().c_str(),
			result.domain_qp_results(i).domain_score());
	}

	if (NULL != classifier)
	{
		delete classifier;
		classifier = NULL;
	}
}

DIALOG_SEG_END_NAMESPACE(qp);
