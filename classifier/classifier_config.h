#ifndef NLU_CLASSIFIER_CLASSIFIER_CONFIG_H_
#define NLU_CLASSIFIER_CLASSIFIER_CONFIG_H_

#include <string>

#include "../log.h"
#include "../common_macros.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class ClassifierConfig
{
public:
	ClassifierConfig() : max_min_score_ratio_(1.0f) {}

	~ClassifierConfig() {}

public:
	bool Init(const std::string& cfg_path);

	const std::string& GetClassifierName();

	const std::string& GetFeatureGeneratorName();

	const std::string& GetModelPath();

	const std::string& GetClassIDPath();

	const std::string& GetSupportDomain();

	const float GetMaxMinRatio();

private:
	static const std::string CLASSIFIER_NAME;
	static const std::string FEATURE_GENERATOR_NAME;
	static const std::string INPUT_MODEL_PATH;
	static const std::string CLASS_ID_PATH;
	static const std::string SUPPORT_DOMAIN;
	static const std::string MAX_MIN_SCORE_RATIO;

private:
	std::string classifier_name_;			// classifier name [MaxEntropyClassifier]
	std::string feature_generator_name_;	// feature generator name [SimpleFeatureGenerator]
	std::string input_model_path_;			// model path [./models/model]
	std::string class_id_path_;				// class id path [./conf/class_id.txt]
	std::string support_domain_;			// supported domain [ ¡°map,calendar,music,telephone¡± ]
	float max_min_score_ratio_;				// max min score ratio [9.0f]

private:
	DIALOG_SEG_LOG_DECLARE();
};  // ClassifierConfig

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // NLU_CLASSIFIER_CLASSIFIER_CONFIG_H_

