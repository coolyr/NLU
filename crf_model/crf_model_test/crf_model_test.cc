#include "crf_model/crf_model_test/crf_model_test.h"
#include <assert.h>
#include <cppunit/TestAssert.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "crf_model/crf_model.h"

using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, CRFModelTest);

CPPUNIT_TEST_SUITE_REGISTRATION(CRFModelTest);

CRFModelTest::CRFModelTest() {}

CRFModelTest::~CRFModelTest() {}

void CRFModelTest::setUp() {}

void CRFModelTest::tearDown() {}

void CRFModelTest::CRFTest() {
  CRFModel crf_model;
  CPPUNIT_ASSERT(0 != crf_model.LoadModel("./models/crf_0524.model"));
}

DIALOG_SEG_END_NAMESPACE(qp);
