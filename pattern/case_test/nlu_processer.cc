#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include "nlu_processer.h"
#include "classifier/classifier_config.h"
#include "classifier/classifier_factory.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, NLUProcesser);

using std::string;
NLUProcesser::~NLUProcesser() {
  if (word_segmenter_) {
    delete word_segmenter_;
    word_segmenter_ = NULL;
  }
  if (crf_model_) {
    delete crf_model_;
    crf_model_ = NULL;
  }
  if (dict_matcher_) {
    delete dict_matcher_;
    dict_matcher_ = NULL;
  }
  if (classifier_) {
    delete classifier_;
    classifier_ = NULL;
  }
  if (post_processer_) {
    delete post_processer_;
    post_processer_ = NULL;
  }
  if (pattern_engine_) {
    pattern_engine_->Destroy();
    delete pattern_engine_;
    pattern_engine_ = NULL;
  }
}

int NLUProcesser::Init(ws::AliTokenizerFactory* factory, const string& cfg_path) {
  word_segmenter_ = new WordSegmenter();
  int ret = word_segmenter_->Init(factory, cfg_path);
  if (ret != 0) {
    LOG(ERROR, "WordSegmenter init failed");
    return -1;
  }

  crf_model_ = new CRFModel();
  ret = crf_model_->Init(cfg_path);
  if (ret != 0) {
    LOG(ERROR, "CrfModel init failed");
    return -1;
  }

  dict_matcher_ = new DictMatcher();
  ret = dict_matcher_->Init(cfg_path);
  if (ret != 0) {
    LOG(ERROR, "DictMatcher init failed");
    return -1;
  }

  ClassifierConfig classifier_config;
  if (!classifier_config.Init(cfg_path)) {
    LOG(ERROR, "classifier config init failed");
    return -1;
  }
  ClassifierFactory classifier_factory;
  if (!classifier_factory.Init(classifier_config)) {
    LOG(ERROR, "classifier factory init failed");
    return -1;
  }
  classifier_ = classifier_factory.CreateClassifier();
  if (!classifier_) {
    LOG(ERROR, "classifier get failed");
    return -1;
  }
  if (!classifier_->Init(classifier_config)) {
    LOG(ERROR, "classifier init failed");
    return -1;
  }

  post_processer_ = new PostProcesser();
  ret = post_processer_->Init(cfg_path);
  if (ret != 0) {
    LOG(ERROR, "Post processer init failed");
    return -1;
  }

  pattern_engine_ = new PatternEngine();
  if (pattern_engine_->Init(cfg_path.c_str()) != 0) {
    LOG(ERROR, "pattern engine inti failed");
    return -1;
  }
  return 0;
}

int NLUProcesser::Process(const QpRequest& qp_request, NluResult* nlu_result) {
  std::ofstream ofs("./data_test/test_res", std::ofstream::app);

  QpResult* qp_result = new QpResult();
  qp_result->set_raw_query(qp_request.query());

  int ret = word_segmenter_->Process(qp_request, qp_result);
  if (ret != 0) {
    LOG(ERROR, "word segmenter process failed");
    return -1;
  }

  ret = crf_model_->Process(qp_request, qp_result);
  if (ret != 0) {
    LOG(ERROR, "CRF Model process failed");
    return -1;
  }

  ret = dict_matcher_->Process(qp_request, qp_result);
  if (ret != 0) {
    LOG(ERROR, "Dict matcher process failed");
    return -1;
  }

  if (!classifier_->Predict(qp_request, qp_result)) {
    LOG(ERROR, "classify failed");
    return -1;
  }

  ret = post_processer_->Process(qp_request, qp_result);
  if (ret != 0) {
    LOG(ERROR, "Post processer process failed");
    return -1;
  }
// for unit test
//  int fd = open("data_test/seri.res", O_CREAT|O_TRUNC|O_RDWR, 0644);
//  qp_result->SerializeToFileDescriptor(fd);
//  close(fd);

  ret = pattern_engine_->Process(qp_result, *nlu_result, false);
  if (ret != 0) {
    LOG(ERROR, "pattern process failed");
    return -1;
  }
  
  string raw_query = qp_result->raw_query();
  ofs << raw_query << ";;;";
  for (int i = 0; i < qp_result->domain_qp_results_size();i++) {
    DomainQpResult dqr = qp_result->domain_qp_results(i);
    string domain = dqr.domain();
    float score = dqr.domain_score();
    string ne_sen = dqr.ne_sentense();
    ofs << " class:" << domain << ";;;";
    ofs << " score:" << score << ";;;";
    ofs << " ne_sen:" << ne_sen << ";;;";
  }
  string seg = nlu_result->seg();
  ofs << " seg:" << seg << ";;;";
  if (nlu_result->domains_size() == 0) {
    ofs << std::endl;
  }
  for (int i = 0; i < nlu_result->domains_size(); i++) {
    for (int j = 0; j < nlu_result->domains(i).domain_intents_size(); j++) {
        const string intent = nlu_result->domains(i).domain_intents(j).intent_name();
        if (intent.empty()) {
          ofs << std::endl;
        } else {
          ofs << " intent:" << intent << std::endl;
        }
    }
  }

  LOG(INFO, "qp nlu_result   is: %s", qp_result->Utf8DebugString().c_str());
  if (qp_result != NULL) {
    delete qp_result;
    qp_result = NULL;
  }
  ofs.close();
  return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
