#include "../classifier/feature_generator_factory.h"
#include "../classifier/simple_feature_generator.h"

#include <string>

using std::string;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, FeatureGeneratorFactory);

bool FeatureGeneratorFactory::Init(const ClassifierConfig& config)
{
	config_ = config;
	return true;
}

AbstractFeatureGenerator* FeatureGeneratorFactory::CreateFeatureGenerator()
{
	AbstractFeatureGenerator* feature_generator = NULL;
	if ("SimpleFeatureGenerator" == config_.GetFeatureGeneratorName())
	{
		feature_generator = new SimpleFeatureGenerator();
	}
	return feature_generator;
}

DIALOG_SEG_END_NAMESPACE(qp);
