#include "word_segmenter.h"
#include "../reader/config_reader.h"
#include <string>
#include "../mylib/ali_tokenizer.h"
#include "../mylib/ali_tokenizer_define.h"


using std::string;
using namespace ws;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, WordSegmenter);

int WordSegmenter::Init(ws::AliTokenizerFactory* factory, const string& cfg_path)
{
	factory_ = factory;
	ConfigReader reader(cfg_path.c_str());
	/*
		wordseg : {
			SEG_MODE = "INTERNET_CHN" ;    <!-- 互联网中文分词 -->
		};
	*/
	seg_mode_ = reader.Get<string>("wordseg.SEG_MODE");
	if (seg_mode_.empty())
	{
		LOG(ERROR, "seg mode has not been set");
		return -1;
	}
	return 0;
}

//QpResult.raw_query = QpRequest.query
int WordSegmenter::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	if (factory_ == NULL)
	{
		LOG(ERROR, "factory has not been initialized");
		return -1;
	}
	//<1> 阿里分词器AliTokenizer
	AliTokenizer* tokenizer = factory_->GetAliTokenizer(seg_mode_.c_str());
	if (tokenizer == NULL)
	{
		LOG(ERROR, "get tokenizer faild");
		return -1;
	}
	//<2> 分词结果SegResult
	SegResult* seg_result = tokenizer->CreateSegResult();//切分结果容器
	if (!seg_result)
	{
		LOG(ERROR, "seg result created failed");
		return -1;
	}
	//QpResult.raw_query = QpRequest.query
	if (!qp_result->has_raw_query())
	{
		LOG(ERROR, "qp result has no raw query");
		return -1;
	}

	//<3> 切分过程
	const char* raw_query = qp_result->raw_query().c_str();//切分query
	int len = strlen(raw_query); //长度
	int ret = tokenizer->Segment(raw_query, len, UTF8, SEG_TOKEN_RETRIEVE, seg_result); //切分[检索粒度：SEG_TOKEN_RETRIEVE]
	uint32_t begin = 0;
	uint32_t end = begin;
	//<4>解析切分结果
	if (ret == 0)
	{
		SegToken* token = NULL;
		for (SegResult::Iterator iter = seg_result->Begin(RETRIEVE_ITERATOR); iter != seg_result->End(RETRIEVE_ITERATOR); ++iter)//迭代器【检索粒度:RETRIEVE_ITERATOR】	
		{
			if (IsExtendRetrieveToken(iter->tokenType))
			{
				continue;
			}
			//const char* pWord;      //指向【当前切词】的开始的【指针】，不以‘\0’结尾  //pointer to the word's beginning in input text, not ended with '\0'
			//uint16_t length;        //词语的长度，以字节为 [单位] 计算, word length in encoding bytes
			string cur_word((const char*)iter->pWord, iter->length); //当前切词
			end = begin + iter->length;

			string cur_pos("");//词性POS
			if (seg_result->GetPosTagName(iter->posTagId))//获取词性id
			{
				cur_pos = seg_result->GetPosTagName(iter->posTagId);//词性POS
			}
			else
			{
				cur_pos = "UNK"; //未知
			}
			/*
			message Term {
				required string text = 1;		//切词
				required string pos = 2;		//词性
				required uint32 begin = 3;     //词的开始位置 【在原始query中的】，以字节为单位，左闭右开 
				required uint32 end = 4;	   //词的结束位置 【在原始query中的】
				optional float weight = 5;     //term权重
			}
			*/
			Term* term = qp_result->add_terms();
			term->set_text(cur_word);	//当前 切词
			term->set_pos(cur_pos);		//词性POS
			term->set_begin(begin);		//词的开始位置 【在原始query中的】
			term->set_end(end);			//词的结束位置 【在原始query中的】
			begin = end;				//begin后移， 指向下一个词的开始
		}
	}
	else
	{
		LOG(ERROR, "seg failed");
		return -1;
	}
	//<5> 释放解析结果
	tokenizer->ReleaseSegResult(seg_result);
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
