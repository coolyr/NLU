#include "../classifier/simple_feature_generator.h"

#include <string>
#include <map>
#include <vector>

using std::map;
using std::string;
using std::vector;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, SimpleFeatureGenerator);

bool Comparator(const Term& a, const Term& b)
{
	return a.begin() < b.begin();
}

/*
message FeatureElement {
	required string key = 1;	//特征
	required uint32 count = 2;  //频率
}
message Feature {
	required string label = 1;
	repeated FeatureElement elements = 2;
}
*/
bool SimpleFeatureGenerator::GenerateFeature(const QpRequest& qp_request, const QpResult& qp_result, Feature* feature)
{
	/*

	类别y     特征x	      权重W      ===>>    特征f 【分类的过程就是查这个表的过程】
	__________________________
	7		官方_网站	0.477547
	14		信息_叫		0.032232
	13		loc_几		-0.012150
	___________________________
	*/
	if (NULL == feature)
	{
		return false;
	}
	vector<Term> term_vec;
	map<string, int> feature_map;
	for (int i = 0; i < qp_result.terms_size(); ++i) //单词特征   =>>  feature_map["天气"] = count("天气")
	{
		term_vec.push_back(qp_result.terms(i));
		feature_map[qp_result.terms(i).text()]++; //单词计数 +1
	}

	/*
	message Term {
		required string text = 1;		//切词
		required string pos = 2;		//词性
		required uint32 begin = 3;     //词的开始位置 【在原始query中的】，以字节为单位，左闭右开
		required uint32 end = 4;	   //词的结束位置 【在原始query中的】
		optional float weight = 5;     //term权重 【word_segmenter 未添加】
	}
	*/
	// "今天  天气  很  好"
	for (size_t i = 1; i < term_vec.size(); ++i) //词的bigram特征   =>   feature_map["今天天气"] = count("今天天气")
	{
		string bgram;
		if (term_vec[i - 1].end() == term_vec[i].begin())
		{
			bgram = term_vec[i - 1].text() + term_vec[i].text();
		}
		feature_map[bgram]++;//bigram特征计数
	}

	//    "刘德华/[per | singer]  唱  了  一首  忘情水/song"   ==>   per是CRF识别的，singer和song是通过词典识别的
	for (int i = 0; i < qp_result.ne_terms_size(); ++i) //实体词的bigram特征   =>   feature_map["singer_唱"] = count("singer_唱")
	{
		/*
		message NeTerm {
			required string ne_type = 1;	//实体类型    CRF：[per/poi/loc]    DIC： [singer/song/app/website]
			required string ne_text = 2;	//实体词 【CRF有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
			required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
			required uint32 end = 4;
		}
		*/
		string bgram_for_ne_term;
		//####################################【开始】 和 【结束】 特殊处理###########################################
		if (0 == qp_result.ne_terms(i).begin())//实体词是开始
		{
			// start
			bgram_for_ne_term = "S_" + qp_result.ne_terms(i).ne_type();		// "S_per"   "S_singer"
			feature_map[bgram_for_ne_term]++;
		}
		else if (qp_request.query().length() ==	qp_result.ne_terms(i).end()) //实体词是结束
		{
			// end
			bgram_for_ne_term = qp_result.ne_terms(i).ne_type() + "_E";		// "song_E"
			feature_map[bgram_for_ne_term]++;
		}
		//#################################################################################################

		//##########################################实体词和【前后词】的特征#################################
		vector<int> indexes_;
		const NeTerm& ne_term = qp_result.ne_terms(i);
		//当前实体词ne_term的 【前一个词】和 【后一个词】的词排序索引
		if (!FindMatch(term_vec, ne_term, &indexes_))
		{
			// failure  ??????有问题->当indexes_==NULL返回false
			LOG(INFO, "Failed to find matched Term for NeTerm!"); //切词 和 实体词 不匹配   ==>  [实体词取了两个词的中间部分]
			continue;
		}
		// found matched and update
		for (size_t i = 0; i < indexes_.size(); ++i)//实体词和【前后词】的特征
		{
			const Term&  term = term_vec[indexes_[i]];
			if (term.begin() == ne_term.end())//当前词是实体词的 【后一个词】
			{
				bgram_for_ne_term = ne_term.ne_type() + "_" + term.text();// "singer_唱"  "per_唱"
				feature_map[bgram_for_ne_term]++;
			}
			else if (term.end() == ne_term.begin())//当前词是实体词的 【前一个词】
			{
				bgram_for_ne_term = term.text() + "_" + ne_term.ne_type(); // "一首_song"
				feature_map[bgram_for_ne_term]++;
			}
			else
			{
				LOG(ERROR, "Match Error for term:%s, ne_term:%s", term.text().c_str(), ne_term.ne_type().c_str());
			}
		}//实体词和【前后词】的特征
	}//实体词的bigram特征   =>   feature_map["singer_唱"] = count("singer_唱")
	/*
	message FeatureElement {
		required string key = 1; //特征
		required uint32 count = 2; //频率
	}
	message Feature {
		required string label = 1;
		repeated FeatureElement elements = 2;
	}
	*/
	map<string, int>::iterator iter;
	for (iter = feature_map.begin(); iter != feature_map.end(); ++iter)
	{
		FeatureElement* element = feature->add_elements();
		element->set_key(iter->first); //特征
		element->set_count(iter->second); //count(feature)
	}
	return true;
}

/*
vector<int> indexes_;
const NeTerm& ne_term = qp_result.ne_terms(i);
if (!FindMatch(term_vec, ne_term, &indexes_))
	{continue;}
*/

/*
当前实体词ne_term的 【前一个词】和 【后一个词】的词排序索引

“A实体词B”    =》   index[0] = index[A], index[1] = index[B]
“实体词C”     =》   index[0] = index[B]
“A实体词”     =》   index[0] = index[A]
*/
bool SimpleFeatureGenerator::FindMatch(const vector<Term>& term_vec, const NeTerm& ne_term,	vector<int>* index)
{
	if (NULL == index)
	{
		return false;
	}
	index->resize(0);//设置长度为0
	for (size_t i = 0; i < term_vec.size(); ++i)
	{
		if (term_vec[i].end() == ne_term.begin() || term_vec[i].begin() == ne_term.end())
		{
			index->push_back(i); //当前实体词ne_term的 【前一个词】和 【后一个词】的词排序索引
		}
	}
	return true;
}

DIALOG_SEG_END_NAMESPACE(qp);
