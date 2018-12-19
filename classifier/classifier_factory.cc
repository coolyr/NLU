#include "../classifier/classifier_factory.h"

#include <string>

#include "../classifier/maxentropy_classifier.h"

using std::string;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, ClassifierFactory);

bool ClassifierFactory::Init(const ClassifierConfig& config)
{
	config_ = config;
	return true;
}

AbstractClassifier* ClassifierFactory::CreateClassifier()
{
	AbstractClassifier* classifier = NULL;
	if ("MaxEntropyClassifier" == config_.GetClassifierName())
	{
		classifier = new MaxEntropyClassifier();
	}
	return classifier;
}

DIALOG_SEG_END_NAMESPACE(qp);
