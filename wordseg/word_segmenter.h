#ifndef DM_QP_WORD_SEGMENTER_H_
#define DM_QP_WORD_SEGMENTER_H_

#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include "../mylib/ali_tokenizer.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class WordSegmenter
{
public:
	WordSegmenter() : factory_(NULL) {}
	~WordSegmenter() {}

	int Init(ws::AliTokenizerFactory* factory, const std::string& cfg_path);
	int Process(const QpRequest& qp_request, QpResult* qp_result);

private:
	ws::AliTokenizerFactory* factory_;//阿里分词器
	std::string seg_mode_; //INTERNET_CHN
	/*
		wordseg : {
			SEG_MODE = "INTERNET_CHN" ;    <!-- 互联网中文分词 -->
		};
	*/

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif //DM_QP_WORD_SEGMENTER_H_
