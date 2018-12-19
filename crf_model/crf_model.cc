#include "crf_model.h"
#include "../reader/config_reader.h"
#include <iostream>

using std::string;
using std::vector;
using std::cout;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, CRFModel);

CRFModel::CRFModel()
{
	model_ = NULL;
}

CRFModel::~CRFModel()
{
	if (model_ != NULL)
	{
		delete model_;
		model_ = NULL;
	}
}

int CRFModel::Init(const string& cfg_path)
{
	/*
		crf_model : {
			MODEL_FILE = "./models/crf_0524.model" ;
		};
	*/
	ConfigReader reader(cfg_path);
	const string& model_path = reader.Get<string>("crf_model.MODEL_FILE");
	string params("");
	params.append("-m ");
	params.append(model_path);
	params.append(" -v3");
	// params =>  "-m ./models/crf_0524.model -v3"
	// 模型二进制：crf_0524.model
	model_ = CRFPP::createModel(params.c_str());//创建模型
	if (model_ == NULL)
	{
		LOG(ERROR, "can not create crf model: %s", model_path.c_str());
		return -1;
	}
	return 0;
}
/*
识别出实体，添加到qp_result.ne_term
*/
int CRFModel::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	vector<string> crf_input;//CRF特征：以字为单位提取feature
	vector<int> term_index;
	/*
	message Term {
		required string text = 1;		//切词
		required string pos = 2;		//词性
		required uint32 begin = 3;		//词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开 
		required uint32 end = 4;		//词的结束位置,在原始query中的.
		optional float weight = 5;		//term权重 [切词时未填充]
	}
	*/
	/*
	【电视台/说/今天/天气/很/温暖/？】
	【  0，  1， 2， 3， 4,  5,  6】		<-   term_index
	*/
	for (int i = 0; i < qp_result->terms_size(); ++i)//遍历每个切词，0开始  
	{
		const Term& term = qp_result->terms(i);
		vector<string> characters;
		//characters 存储当前切词的每个字【utf-8编码】
		Split2chs(term.text(), characters);

		for (size_t j = 0; j < characters.size(); j++)//遍历每个字  -> 以字为特征提取CRF Feature
		{
			//只有一个字的切词
			if (characters.size() == 1)
			{
				string crf_item = characters[j] + "\t" + term.pos(); // "很\t副词"
				crf_input.push_back(crf_item);			  			
				term_index.push_back(i);//所使用的词是第几个？
				break;
			}
			//字是词的开始
			if (j == 0)
			{
				string crf_item = characters[j] + "\t" + term.pos() + "_B"; // "温\t形容词_B"
				crf_input.push_back(crf_item);
				term_index.push_back(i);
				continue;
			}
			//字是词的结束
			if (j == characters.size() - 1)
			{
				string crf_item = characters[j] + "\t" + term.pos() + "_E"; // "气\t名词_E"
				crf_input.push_back(crf_item);
				term_index.push_back(i);
				continue;
			}
			//字在中间
			string crf_item = characters[j] + "\t" + term.pos() + "_M";		//"视\t名词_M"
			crf_input.push_back(crf_item);
			term_index.push_back(i);
		}//for(size_t j = 0; j < characters.size(); j++)//遍历每个字
	}//for (int i = 0; i < qp_result->terms_size(); ++i) 遍历每个切词

	vector<string> tags;
	//运行维特比算法，标识实体词,结果存储在tags中
	int ret = Parse(crf_input, tags);//CRF特征：以字为单位提取feature
	if (ret != 0)
	{
		LOG(ERROR, "crf parse fail");
		return -1;
	}
	//解析实体标注的结果【PEO:人名, POI:位置, LOC:地点】
	// PEO_S : 开始
	// PEO_M : 中间
	// PEO_E : 结尾
	/*
	message NeTerm {
		required string ne_type = 1;
		required string ne_text = 2;
		required uint32 begin = 3;
		required uint32 end = 4;
	} 
	*/
	for (size_t i = 0; i < tags.size(); ++i)//遍历标签
	{
		//人名
		if (tags[i].compare("PEO_S") == 0)
		{
			NeTerm* ne_term = qp_result->add_ne_terms();//实体词的 proto
			size_t j = i + 1;
			//找到结尾处 【i,j)前闭后开
			while ((j < tags.size()) &&	((tags[j].compare("PEO_M") == 0) || (tags[j].compare("PEO_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			/*
				特征提取是按字，term_index存储当前字的所在词的排序索引(切词后的第几个词)
			*/
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)// 什么意思?? 【】
				{
					continue;
				}
				pre_index = term_index[k];//当前字特征所在词的排序索引  
				ne_term_text += qp_result->terms(term_index[k]).text();//把当前‘字’特征所在的‘词’加入 ‘命名实体’  -》 “如果检索粒度切词和实体不重合？实体识别的结果是两个切词的中间部分 A[B/CD]E”
			}
			ne_term->set_ne_text(ne_term_text); //实体词 【CRF有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
			ne_term->set_ne_type("per");	//实体类别
			ne_term->set_begin(qp_result->terms(term_index[i]).begin()); //实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end()); //词的结束位置,在原始query中的.
			i = j - 1;// ?? i=j
			continue;
		}	//人名
		if (tags[i].compare("POI_S") == 0)//POI类型
		{
			NeTerm* ne_term = qp_result->add_ne_terms();
			size_t j = i + 1;
			while ((j < tags.size()) &&
				((tags[j].compare("POI_M") == 0) || (tags[j].compare("POI_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)
				{
					continue;
				}
				pre_index = term_index[k];
				ne_term_text += qp_result->terms(term_index[k]).text();
			}
			ne_term->set_ne_text(ne_term_text);
			ne_term->set_ne_type("poi");
			ne_term->set_begin(qp_result->terms(term_index[i]).begin());
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end());
			i = j - 1;
			continue;
		}//POI类型
		if (tags[i].compare("LOC_S") == 0)//LOC类型
		{
			NeTerm* ne_term = qp_result->add_ne_terms();
			size_t j = i + 1;
			while ((j < tags.size()) &&
				((tags[j].compare("LOC_M") == 0) || (tags[j].compare("LOC_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)
				{
					continue;
				}
				pre_index = term_index[k];
				ne_term_text += qp_result->terms(term_index[k]).text();
			}
			ne_term->set_ne_text(ne_term_text);
			ne_term->set_ne_type("loc");
			ne_term->set_begin(qp_result->terms(term_index[i]).begin());
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end());
			i = j - 1;
			continue;
		}//LOC类型
	}
	return 0;
}

//Split2chs(term.text(), characters);
void CRFModel::Split2chs(const string& word, vector<string>& characters)
{
	string character;
	//utf-8编码格式是变长编码，通过检测编码的范围确定编码的长度。[1~6]
	for (size_t i = 0, len = 0; i != word.length(); i += len)
	{
		unsigned char byte = (unsigned)word[i];
		if (byte >= 0xFC)
			len = 6;
		else if (byte >= 0xF8)
			len = 5;
		else if (byte >= 0xF0)
			len = 4;
		else if (byte >= 0xE0)
			len = 3;
		else if (byte >= 0xC0)
			len = 2;
		else
			len = 1;
		character = word.substr(i, len);//切出当前字
		characters.push_back(character);
	}
}
/*
vector<string> tags;
int ret = Parse(crf_input, tags);
*/

//运行维特比算法，标识实体词
int CRFModel::Parse(const vector<string>& sequence, vector<string>& tags)
{
	//<1>	获取打标签器Tagger
	CRFPP::Tagger* tagger = model_->createTagger();
	if (tagger == NULL)
	{
		LOG(ERROR, "create crf tagger fail");
		return -1;
	}
	tags.clear();
	tagger->clear();
	//<2>	添加序列特征 【以字为单位的特征】
	for (size_t i = 0; i < sequence.size(); i++)
	{
		tagger->add(sequence[i].c_str());
	}
	//<3>	解析序列 - 维特比
	if (!tagger->parse())
	{
		LOG(ERROR, "parse error");
		return -1;
	}
	//<4>	添加每个词的特征 [y2输出格式 ？]
	for (size_t i = 0; i < tagger->size(); i++)
	{
		tags.push_back(tagger->y2(i));//添加打完标签后的每个字的标签结果
	}
	//<5>	析构标签器Tagger
	if (tagger != NULL)
	{
		delete tagger;
		tagger = NULL;
	}
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
