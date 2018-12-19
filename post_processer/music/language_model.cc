#include "language_model.h"
//#include <boost/algorithm/string.hpp>
#include "../../mylib/StringUtil.h"
#include "../../common_macros.h"
#include "../../log.h"

#include <iostream>
#include <fstream>
#include <math.h>

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, LanguageModel);

using std::vector;
using std::string;
using std::ifstream;
using std::map;

int LanguageModel::Init(const string& path)//./models/music.model
{
	ifstream infile(path.c_str());
	vocabulary_size_ = 0;
	if (!infile)
	{
		LOG(ERROR, "can not open music lm file");
		return -1;
	}

	/*
			萨克斯 _ _      1
			想听 点 伤感    1
			_ singer _      1
			听得 歌 e1      1
			就 放 一        1
			、 劲 暴        1
			的 双人 合唱    5
			s2 听 小熊      1
			困难 的 负面    1
			推荐 几 个      1
	*/
	string line;
	vector<string> terms;
	vector<string> words;
	while (getline(infile, line))
	{
		// line = "想听 点 伤感    1"
		terms.clear();
		boost::split(terms, line, boost::is_any_of("\t"));
		if (terms.size() != 2)
		{
			LOG(WARN, "wrong format line: %s", line.c_str());
			continue;
		}
		string& trigram = terms[0];// trigram = "想听 点 伤感"
		int trigram_count = atoi(terms[1].c_str());// trigram_count = 1
		trigram_lm_.insert(make_pair(trigram, trigram_count));// <trigram,trigram_count> = <"想听 点 伤感", 1>
		words.clear();// ["想听", "点", "伤感"]
		boost::split(words, trigram, boost::is_any_of(" "));
		if (words.size() != 3)
		{
			LOG(WARN, "wrong format trigram: %s", trigram.c_str());
			continue;
		}
		int bigram_count = trigram_count; // bigram_count = trigram_count;
		string bigram = words[0] + " " + words[1];// bigram = "想听 点"
		if (bigram_lm_.find(bigram) == bigram_lm_.end())
		{
			bigram_lm_.insert(make_pair(bigram, bigram_count)); //<bigram, bigram_count>
		}
		else
		{
			bigram_lm_[bigram] += bigram_count; //统计 cout(AB?)的次数[分母]
		}
	}
	vocabulary_size_ = bigram_lm_.size(); // vocabulary_size_ = bigram的大小
	min_word_freq_ = SMOOTH_FACTOR / vocabulary_size_; // 最小频率
	return 0;
}

// temp_terms =	["song", "听", "singer", "的", "忘情水"]			 "song听singer的忘情水"    <==  "我想听刘德华的歌"
// temp_terms =	["我", “想”, "听", "singer", "的", "song"]		 "我想听singer的song"      <==  "我想听刘德华的歌"
//lm_->ComputeScore(temp_terms, temp_score); //每个<singer,song>实体对替换后计算 => 不是所有替换后一起计算
int LanguageModel::ComputeScore(const vector<std::string>& terms, float& score)
{
	map<string, int> ngrams;
	score = 0.0;
	/*
	ngrams = trigram ["s1 s2 我"] = count("s1 s2 我")
			 trigram ["s2 我 想"] = count("s2 我 想")
			 ...
			 ...
			 trigram ["的 song e1"] = count("的 song e1")
			 trigram ["song e1 e2"] = count("song e1 e2")
	*/
	if (GenerateNgram(terms, ngrams) == -1) //["s1", "s2", "我", “想”, "听", "singer", "的", "song", "e1", "e2"]
	{
		return -1;
	}
	float num_words = 0;
	for (map<string, int>::const_iterator it = ngrams.begin(); it != ngrams.end(); it++)//遍历每个 Ngram【trigram】
	{
		float ngram_score = 0.0;
		/*
			获取Ngram的得分 "s2 我 想"
		1 - 如果bigram不存在，直接-10分
		2 - 否则trigram平滑后直接计算得分。score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR))
		*/
		if (GetNgramScore(it->first, ngram_score) == -1)//it->first = "s2 我 想"
		{
			return -1;
		}
		score += ngram_score * (it->second); //求总得分log-> 相加
		num_words = num_words + it->second; // trigram的个数
	}//遍历每个 Ngram
	return 0;
}

/*
生成query的Ngram
terms = ["song", "听", "singer", "的", "忘情水"]
terms = ["我", “想”, "听", "singer", "的", "song"]
*/
int LanguageModel::GenerateNgram(const vector<string>& terms, map<string, int>& ngrams) const
{
	for (size_t i = 0; i < terms.size(); i++)//遍历每个切词 ["s1", "s2", "我", “想”, "听", "singer", "的", "song", "e1", "e2"]
	{
		if (i == 0)//第一个 切词
		{
			const string& temp = "s1 s2 " + terms[i];// "s1 s2 我"
			ngrams.insert(make_pair(temp, 1));
			continue;
		}
		if (i == 1)//第二个	切词
		{
			const string& temp = "s2 " + terms[0] + " " + terms[1];// “s2 我 想”
			ngrams.insert(make_pair(temp, 1));
			continue;
		}
		const string& temp = terms[i - 2] + " " + terms[i - 1] + " " + terms[i];// "想 听 singer"
		map<string, int>::iterator it = ngrams.find(temp);
		if (it == ngrams.end())
		{
			ngrams.insert(make_pair(temp, 1));
		}
		else
		{
			it->second++;
		}
	}//遍历每个切词

	int len = terms.size();
	if (len > 1)
	{
		const string& temp1 = terms[len - 2] + " " + terms[len - 1] + " e1";// "的 song e1"
		ngrams.insert(make_pair(temp1, 1));
	}
	const string& temp2 = terms[len - 1] + " e1 e2";// "song e1 e2"
	ngrams.insert(make_pair(temp2, 1));
	return 0;
}
/*
获取Ngram的得分 "s2 我 想"
1 - 如果bigram不存在，直接-10分
2 - 否则trigram平滑后直接计算得分。score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR))
*/
int LanguageModel::GetNgramScore(const string& ngram, float& score)
{
	vector<string> words;// ["s2", "我", "想"]
	boost::split(words, ngram, boost::is_any_of(" "));
	if (words.size() != 3)
	{
		LOG(ERROR, "wrong format ngram: %s", ngram.c_str());
		return -1;
	}
	string bigram = words[0] + " " + words[1]; // "s2 我"
	map<string, int>::const_iterator first_iterator = bigram_lm_.find(bigram);//bigram
	map<string, int>::const_iterator second_iterator = trigram_lm_.find(ngram);//trigram
	if (first_iterator == bigram_lm_.end())//bigram 没有 => trigram也没有
	{
		score = -10; //默认 -10
		return 0;// 返回
	}
	float bigram_cout = first_iterator->second;
	float trigram_cout = 0.0;
	if (second_iterator == trigram_lm_.end())//trigram 没有
	{
		trigram_cout = min_word_freq_;// trigram设置最小值 =>  SMOOTH_FACTOR/vocabulary_size_  =>  600/len(bigram) = 5% = 0.05(delta)
	}
	else
	{
		trigram_cout = second_iterator->second + min_word_freq_;// 平滑add delta： 真实trigram_cout + 最小值min_word_freq_
	}
	score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR));//当前 terms【trigram】 的得分,取log形式，最后就会变成相加形式
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
