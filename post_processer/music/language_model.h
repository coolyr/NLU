#ifndef DM_QP_LANGUAGE_MODEL_H
#define DM_QP_LANGUAGE_MODEL_H

#include <string>
#include <vector>
#include <map>
#include "../../common_macros.h"
#include "../../log.h"
#include "../../mylib/StringUtil.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class LanguageModel
{
public:
	int Init(const std::string& path);
	int ComputeScore(const std::vector<std::string>& terms, float& score);

private:
	int GenerateNgram(const std::vector<std::string>& terms, std::map<std::string, int>& ngrams) const;
	int GetNgramScore(const std::string& ngram, float& score);

	std::map<std::string, int> bigram_lm_;	// <bigram, count>
	std::map<std::string, int> trigram_lm_; // <trigram,count>
	float vocabulary_size_;					// bigram_lm_.size();
	float min_word_freq_;					//  = SMOOTH_FACTOR / vocabulary_size_   =>  600/len(bigram) = 5% = 0.05(delta)
	const static float SMOOTH_FACTOR = 600;

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif //DM_QP_LANGUAGE_MODEL_H
