#ifndef NLU_CLASSIFIER_FEATURE_GENERATOR_FACTORY_H_
#define NLU_CLASSIFIER_FEATURE_GENERATOR_FACTORY_H_

#include "../common_macros.h"
#include "../log.h"
#include "../classifier/classifier_config.h"
#include "../classifier/abstract_feature_generator.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class FeatureGeneratorFactory
{
public:
	FeatureGeneratorFactory() {}
	virtual ~FeatureGeneratorFactory() {}
	 
	virtual bool Init(const ClassifierConfig& config);

	virtual AbstractFeatureGenerator* CreateFeatureGenerator();

private:
	ClassifierConfig config_;
	DIALOG_SEG_LOG_DECLARE();
};  // FeatureGeneratorFactory

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_FEATURE_GENERATOR_FACTORY_H_
