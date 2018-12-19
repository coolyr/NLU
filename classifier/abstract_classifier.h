#ifndef NLU_CLASSIFIER_ABSTRACT_CLASSIFIER_H_
#define NLU_CLASSIFIER_ABSTRACT_CLASSIFIER_H_

#include "../common_macros.h"
#include "../log.h"
#include "../proto/classifier.pb.h"
#include "../proto/qp.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
class ClassifierConfig;

class AbstractClassifier
{
public:
	AbstractClassifier() {}
	virtual ~AbstractClassifier() {}

	// Interfaces
	virtual bool Init(const ClassifierConfig& cfg) = 0;
	virtual bool Predict(const QpRequest& qp_request, QpResult* qp_result) = 0;
};  // AbstractClassifier

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_ABSTRACT_CLASSIFIER_H_

