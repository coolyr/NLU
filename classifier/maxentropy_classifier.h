#ifndef NLU_CLASSIFIER_MAXENTROPY_CLASSIFIER_H_
#define NLU_CLASSIFIER_MAXENTROPY_CLASSIFIER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "../common_macros.h"
#include "../log.h"

#include "../proto/classifier.pb.h"
#include "../proto/qp.pb.h"
#include "../mylib/maxent.h"
#include "../classifier/abstract_classifier.h"
#include "../classifier/abstract_feature_generator.h"
#include "../classifier/classifier_config.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class MaxEntropyClassifier : public AbstractClassifier
{
public:
	MaxEntropyClassifier() :feature_generator_(NULL), max_min_score_ratio_(1.0f) {}
	virtual ~MaxEntropyClassifier()
	{
		if (NULL != feature_generator_)
		{
			delete feature_generator_;
		}
	}

	virtual bool Init(const ClassifierConfig& config);

	virtual bool Predict(const QpRequest& qp_request, QpResult* qp_result);

private:
	bool GenerateFeature(const QpRequest& qp_request,	const QpResult& qp_result,	Feature* feature);

	bool LoadClassIDFile(const std::string& file_path);

private:
	ClassifierConfig config_;
	ME_Model model_;
	AbstractFeatureGenerator* feature_generator_;

	float max_min_score_ratio_;							//最高和最低得分的比率
	std::set<std::string> support_domain_set_;			//支持的domain  [map,calendar,music,telephone]
	std::map<std::string, std::string> class_id_map_;	//分类器训练时的支持类别

private:
	static const int PER_LINEBUFF_SIZE;
	static const float OTHER_DOMAIN_SCORE;				//其它domain的分 = 1.0f;
	static const std::string OTHER_DOMAIN;				//其它domain = "other"
	static const std::string CLASS_ID_SEPEARTOR;		//分类和id的分隔符
	static const std::string SUPPORT_DOMAIN_SEPEARTOR;	//支持Domain的分隔符

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_MAXENTROPY_CLASSIFIER_H_
