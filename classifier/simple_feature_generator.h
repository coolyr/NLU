#ifndef NLU_CLASSIFIER_SIMPLE_FEATURE_GENERATOR_H_
#define NLU_CLASSIFIER_SIMPLE_FEATURE_GENERATOR_H_

#include <vector>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/classifier.pb.h"
#include "../proto/qp.pb.h"
#include "../classifier/abstract_feature_generator.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class SimpleFeatureGenerator : public AbstractFeatureGenerator
{
public:
	SimpleFeatureGenerator() {}
	virtual ~SimpleFeatureGenerator() {}

	virtual bool GenerateFeature(const QpRequest& qp_request, const QpResult& qp_result, Feature* feature);
private:
	bool FindMatch(const std::vector<Term>& term_vec, const NeTerm& ne_term, std::vector<int>* index);

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_SIMPLE_FEATURE_GENERATOR_H_
