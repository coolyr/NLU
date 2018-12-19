#include "../classifier/classifier_config.h"

#include <string>

#include "../reader/config_reader.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, ClassifierConfig);

using std::string;

const string ClassifierConfig::CLASSIFIER_NAME("classifier.CLASSIFIER_NAME");
const string ClassifierConfig::FEATURE_GENERATOR_NAME("classifier.FEATURE_GENERATOR_NAME");
const string ClassifierConfig::INPUT_MODEL_PATH("classifier.INPUT_MODEL_PATH");
const string ClassifierConfig::MAX_MIN_SCORE_RATIO("classifier.MAX_MIN_SCORE_RATIO");
const string ClassifierConfig::CLASS_ID_PATH("classifier.CLASS_ID_PATH");
const string ClassifierConfig::SUPPORT_DOMAIN("classifier.SUPPORT_DOMAIN");

bool ClassifierConfig::Init(const string& cfg_path)
{
	/*
		classifier: {
			CLASSIFIER_NAME = "MaxEntropyClassifier";
			FEATURE_GENERATOR_NAME= "SimpleFeatureGenerator";
			INPUT_MODEL_PATH = "./models/model";
			CLASS_ID_PATH = "./conf/class_id.txt";
			MAX_MIN_SCORE_RATIO = "9.0f";
			SUPPORT_DOMAIN = "map,calendar,music,telephone";
		};
	*/
	ConfigReader reader(cfg_path);

	classifier_name_ = reader.Get<string>(CLASSIFIER_NAME);
	feature_generator_name_ = reader.Get<string>(FEATURE_GENERATOR_NAME);
	input_model_path_ = reader.Get<string>(INPUT_MODEL_PATH);
	class_id_path_ = reader.Get<string>(CLASS_ID_PATH);
	support_domain_ = reader.Get<string>(SUPPORT_DOMAIN);
	max_min_score_ratio_ = reader.Get<float>(MAX_MIN_SCORE_RATIO);

	return true;
}  // Init

const string& ClassifierConfig::GetClassifierName()
{
	return classifier_name_;
}  // GetClassifierName

const string& ClassifierConfig::GetFeatureGeneratorName()
{
	return feature_generator_name_;
}  // GetFeatureGeneratorName

const string& ClassifierConfig::GetModelPath()
{
	return input_model_path_;
}  // GetModelPath

const string& ClassifierConfig::GetClassIDPath()
{
	return class_id_path_;
}  // GetClassIDPath

const string& ClassifierConfig::GetSupportDomain()
{
	return support_domain_;
}  // GetSupportDomain

const float ClassifierConfig::GetMaxMinRatio()
{
	return max_min_score_ratio_;
}  // GetMaxMinRatio

DIALOG_SEG_END_NAMESPACE(qp);
