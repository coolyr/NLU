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
			SEG_MODE = "INTERNET_CHN" ;    <!-- ���������ķִ� -->
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
	//<1> ����ִ���AliTokenizer
	AliTokenizer* tokenizer = factory_->GetAliTokenizer(seg_mode_.c_str());
	if (tokenizer == NULL)
	{
		LOG(ERROR, "get tokenizer faild");
		return -1;
	}
	//<2> �ִʽ��SegResult
	SegResult* seg_result = tokenizer->CreateSegResult();//�зֽ������
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

	//<3> �зֹ���
	const char* raw_query = qp_result->raw_query().c_str();//�з�query
	int len = strlen(raw_query); //����
	int ret = tokenizer->Segment(raw_query, len, UTF8, SEG_TOKEN_RETRIEVE, seg_result); //�з�[�������ȣ�SEG_TOKEN_RETRIEVE]
	uint32_t begin = 0;
	uint32_t end = begin;
	//<4>�����зֽ��
	if (ret == 0)
	{
		SegToken* token = NULL;
		for (SegResult::Iterator iter = seg_result->Begin(RETRIEVE_ITERATOR); iter != seg_result->End(RETRIEVE_ITERATOR); ++iter)//����������������:RETRIEVE_ITERATOR��	
		{
			if (IsExtendRetrieveToken(iter->tokenType))
			{
				continue;
			}
			//const char* pWord;      //ָ�򡾵�ǰ�дʡ��Ŀ�ʼ�ġ�ָ�롿�����ԡ�\0����β  //pointer to the word's beginning in input text, not ended with '\0'
			//uint16_t length;        //����ĳ��ȣ����ֽ�Ϊ [��λ] ����, word length in encoding bytes
			string cur_word((const char*)iter->pWord, iter->length); //��ǰ�д�
			end = begin + iter->length;

			string cur_pos("");//����POS
			if (seg_result->GetPosTagName(iter->posTagId))//��ȡ����id
			{
				cur_pos = seg_result->GetPosTagName(iter->posTagId);//����POS
			}
			else
			{
				cur_pos = "UNK"; //δ֪
			}
			/*
			message Term {
				required string text = 1;		//�д�
				required string pos = 2;		//����
				required uint32 begin = 3;     //�ʵĿ�ʼλ�� ����ԭʼquery�еġ������ֽ�Ϊ��λ������ҿ� 
				required uint32 end = 4;	   //�ʵĽ���λ�� ����ԭʼquery�еġ�
				optional float weight = 5;     //termȨ��
			}
			*/
			Term* term = qp_result->add_terms();
			term->set_text(cur_word);	//��ǰ �д�
			term->set_pos(cur_pos);		//����POS
			term->set_begin(begin);		//�ʵĿ�ʼλ�� ����ԭʼquery�еġ�
			term->set_end(end);			//�ʵĽ���λ�� ����ԭʼquery�еġ�
			begin = end;				//begin���ƣ� ָ����һ���ʵĿ�ʼ
		}
	}
	else
	{
		LOG(ERROR, "seg failed");
		return -1;
	}
	//<5> �ͷŽ������
	tokenizer->ReleaseSegResult(seg_result);
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
