#ifndef NLU_CLASSIFIER_CLASSIFIER_FACTORY_H_
#define NLU_CLASSIFIER_CLASSIFIER_FACTORY_H_

#include "../common_macros.h"
#include "../log.h"
#include "../classifier/abstract_classifier.h"
#include "../classifier/classifier_config.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
class AbstractClassifier;

class ClassifierFactory
{
public:
	ClassifierFactory() {}
	virtual ~ClassifierFactory() {}

	virtual bool Init(const ClassifierConfig& config);

	virtual AbstractClassifier* CreateClassifier();

private:
	ClassifierConfig config_;
	DIALOG_SEG_LOG_DECLARE();
};  // ClassifierFactory

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_CLASSIFIER_FACTORY_H_
