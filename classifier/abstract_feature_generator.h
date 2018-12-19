#ifndef NLU_CLASSIFIER_ABSTRACT_FEATURE_GENERATOR_H_
#define NLU_CLASSIFIER_ABSTRACT_FEATURE_GENERATOR_H_

#include "../common_macros.h"
#include "../log.h"
#include "../proto/classifier.pb.h"
#include "../proto/qp.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class AbstractFeatureGenerator
{
public:
	AbstractFeatureGenerator() {}
	virtual ~AbstractFeatureGenerator() {}

	// Interfaces
	virtual bool GenerateFeature(const QpRequest& qp_request, const QpResult& qp_result, Feature* feature) = 0;
};  // AbstractFeatureGenerator

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_ABSTRACT_FEATURE_GENERATOR_H_

