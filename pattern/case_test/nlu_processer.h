#ifndef DM_NLU_PROCESSER_H
#define DM_NLU_PROCESSER_H

#include "common_macros.h"
#include "log.h"
#include "proto/qp.pb.h"
#include "proto/nlu_dm.pb.h"
#include "wordseg/word_segmenter.h"
#include "crf_model/crf_model.h"
#include "dict_matcher/dict_matcher.h"
#include "classifier/abstract_classifier.h"
#include "post_processer/post_processer.h"
#include "pattern/pattern_engine.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class NLUProcesser {
 public:
  NLUProcesser()
      : word_segmenter_(NULL),
        crf_model_(NULL),
        dict_matcher_(NULL),
        classifier_(NULL),
        post_processer_(NULL),
        pattern_engine_(NULL) {}
  ~NLUProcesser();

  int Init(ws::AliTokenizerFactory* factory, const std::string& cfg_path);

  int Process(const QpRequest& qp_request, NluResult* nlu_result);

 private:
  WordSegmenter* word_segmenter_;
  CRFModel* crf_model_;
  DictMatcher* dict_matcher_;
  AbstractClassifier* classifier_;
  PostProcesser* post_processer_;
  PatternEngine* pattern_engine_;

 private:
  DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_NLU_PROCESSER_H
