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
			����˹ _ _      1
			���� �� �˸�    1
			_ singer _      1
			���� �� e1      1
			�� �� һ        1
			�� �� ��        1
			�� ˫�� �ϳ�    5
			s2 �� С��      1
			���� �� ����    1
			�Ƽ� �� ��      1
	*/
	string line;
	vector<string> terms;
	vector<string> words;
	while (getline(infile, line))
	{
		// line = "���� �� �˸�    1"
		terms.clear();
		boost::split(terms, line, boost::is_any_of("\t"));
		if (terms.size() != 2)
		{
			LOG(WARN, "wrong format line: %s", line.c_str());
			continue;
		}
		string& trigram = terms[0];// trigram = "���� �� �˸�"
		int trigram_count = atoi(terms[1].c_str());// trigram_count = 1
		trigram_lm_.insert(make_pair(trigram, trigram_count));// <trigram,trigram_count> = <"���� �� �˸�", 1>
		words.clear();// ["����", "��", "�˸�"]
		boost::split(words, trigram, boost::is_any_of(" "));
		if (words.size() != 3)
		{
			LOG(WARN, "wrong format trigram: %s", trigram.c_str());
			continue;
		}
		int bigram_count = trigram_count; // bigram_count = trigram_count;
		string bigram = words[0] + " " + words[1];// bigram = "���� ��"
		if (bigram_lm_.find(bigram) == bigram_lm_.end())
		{
			bigram_lm_.insert(make_pair(bigram, bigram_count)); //<bigram, bigram_count>
		}
		else
		{
			bigram_lm_[bigram] += bigram_count; //ͳ�� cout(AB?)�Ĵ���[��ĸ]
		}
	}
	vocabulary_size_ = bigram_lm_.size(); // vocabulary_size_ = bigram�Ĵ�С
	min_word_freq_ = SMOOTH_FACTOR / vocabulary_size_; // ��СƵ��
	return 0;
}

// temp_terms =	["song", "��", "singer", "��", "����ˮ"]			 "song��singer������ˮ"    <==  "���������»��ĸ�"
// temp_terms =	["��", ���롱, "��", "singer", "��", "song"]		 "������singer��song"      <==  "���������»��ĸ�"
//lm_->ComputeScore(temp_terms, temp_score); //ÿ��<singer,song>ʵ����滻����� => ���������滻��һ�����
int LanguageModel::ComputeScore(const vector<std::string>& terms, float& score)
{
	map<string, int> ngrams;
	score = 0.0;
	/*
	ngrams = trigram ["s1 s2 ��"] = count("s1 s2 ��")
			 trigram ["s2 �� ��"] = count("s2 �� ��")
			 ...
			 ...
			 trigram ["�� song e1"] = count("�� song e1")
			 trigram ["song e1 e2"] = count("song e1 e2")
	*/
	if (GenerateNgram(terms, ngrams) == -1) //["s1", "s2", "��", ���롱, "��", "singer", "��", "song", "e1", "e2"]
	{
		return -1;
	}
	float num_words = 0;
	for (map<string, int>::const_iterator it = ngrams.begin(); it != ngrams.end(); it++)//����ÿ�� Ngram��trigram��
	{
		float ngram_score = 0.0;
		/*
			��ȡNgram�ĵ÷� "s2 �� ��"
		1 - ���bigram�����ڣ�ֱ��-10��
		2 - ����trigramƽ����ֱ�Ӽ���÷֡�score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR))
		*/
		if (GetNgramScore(it->first, ngram_score) == -1)//it->first = "s2 �� ��"
		{
			return -1;
		}
		score += ngram_score * (it->second); //���ܵ÷�log-> ���
		num_words = num_words + it->second; // trigram�ĸ���
	}//����ÿ�� Ngram
	return 0;
}

/*
����query��Ngram
terms = ["song", "��", "singer", "��", "����ˮ"]
terms = ["��", ���롱, "��", "singer", "��", "song"]
*/
int LanguageModel::GenerateNgram(const vector<string>& terms, map<string, int>& ngrams) const
{
	for (size_t i = 0; i < terms.size(); i++)//����ÿ���д� ["s1", "s2", "��", ���롱, "��", "singer", "��", "song", "e1", "e2"]
	{
		if (i == 0)//��һ�� �д�
		{
			const string& temp = "s1 s2 " + terms[i];// "s1 s2 ��"
			ngrams.insert(make_pair(temp, 1));
			continue;
		}
		if (i == 1)//�ڶ���	�д�
		{
			const string& temp = "s2 " + terms[0] + " " + terms[1];// ��s2 �� �롱
			ngrams.insert(make_pair(temp, 1));
			continue;
		}
		const string& temp = terms[i - 2] + " " + terms[i - 1] + " " + terms[i];// "�� �� singer"
		map<string, int>::iterator it = ngrams.find(temp);
		if (it == ngrams.end())
		{
			ngrams.insert(make_pair(temp, 1));
		}
		else
		{
			it->second++;
		}
	}//����ÿ���д�

	int len = terms.size();
	if (len > 1)
	{
		const string& temp1 = terms[len - 2] + " " + terms[len - 1] + " e1";// "�� song e1"
		ngrams.insert(make_pair(temp1, 1));
	}
	const string& temp2 = terms[len - 1] + " e1 e2";// "song e1 e2"
	ngrams.insert(make_pair(temp2, 1));
	return 0;
}
/*
��ȡNgram�ĵ÷� "s2 �� ��"
1 - ���bigram�����ڣ�ֱ��-10��
2 - ����trigramƽ����ֱ�Ӽ���÷֡�score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR))
*/
int LanguageModel::GetNgramScore(const string& ngram, float& score)
{
	vector<string> words;// ["s2", "��", "��"]
	boost::split(words, ngram, boost::is_any_of(" "));
	if (words.size() != 3)
	{
		LOG(ERROR, "wrong format ngram: %s", ngram.c_str());
		return -1;
	}
	string bigram = words[0] + " " + words[1]; // "s2 ��"
	map<string, int>::const_iterator first_iterator = bigram_lm_.find(bigram);//bigram
	map<string, int>::const_iterator second_iterator = trigram_lm_.find(ngram);//trigram
	if (first_iterator == bigram_lm_.end())//bigram û�� => trigramҲû��
	{
		score = -10; //Ĭ�� -10
		return 0;// ����
	}
	float bigram_cout = first_iterator->second;
	float trigram_cout = 0.0;
	if (second_iterator == trigram_lm_.end())//trigram û��
	{
		trigram_cout = min_word_freq_;// trigram������Сֵ =>  SMOOTH_FACTOR/vocabulary_size_  =>  600/len(bigram) = 5% = 0.05(delta)
	}
	else
	{
		trigram_cout = second_iterator->second + min_word_freq_;// ƽ��add delta�� ��ʵtrigram_cout + ��Сֵmin_word_freq_
	}
	score = log(trigram_cout / (bigram_cout + SMOOTH_FACTOR));//��ǰ terms��trigram�� �ĵ÷�,ȡlog��ʽ�����ͻ��������ʽ
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
