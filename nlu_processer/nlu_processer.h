#ifndef DM_NLU_PROCESSER_H
#define DM_NLU_PROCESSER_H

#include "../proto/qp.pb.h"
#include "../proto/nlu_dm.pb.h"
#include <set>

namespace ws
{
	class AliTokenizerFactory;
}

namespace alog
{
	class Logger;
}

namespace dialog
{
	namespace qp
	{

		class WordSegmenter;
		class CRFModel;
		class DictMatcher;
		class AbstractClassifier;
		class PostProcesser;
		class PatternEngine;

		class NLUProcesser
		{
		public:
			NLUProcesser();
			~NLUProcesser();

			int Init(ws::AliTokenizerFactory* factory, const std::string& cfg_path);

			int Process(const QpRequest& qp_request, NluResult* nlu_result);
			int Process(const std::string session_str, const std::string context_str, const std::string qp_request_str, std::string& context_cache_str, std::string& nlu_result_str);
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			int AddRules(const std::string domain, const std::string intent, const std::set<std::string> rules);
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			
		private:
			WordSegmenter* word_segmenter_;
			CRFModel* crf_model_;
			DictMatcher* dict_matcher_;
			AbstractClassifier* classifier_;
			PostProcesser* post_processer_;
			PatternEngine* pattern_engine_;

		private:
			static alog::Logger* _logger;
		};

	}
}

#endif  //DM_NLU_PROCESSER_H
