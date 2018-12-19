#ifndef DM_QP_MUSIC_POST_PROCESSER_H
#define DM_QP_MUSIC_POST_PROCESSER_H

#include "../../post_processer/post_processer_base.h"
#include "language_model.h"
#include <string>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class MusicPostProcesser : public PostProcesserBase
{
public:
	MusicPostProcesser() : lm_(NULL) {}
	virtual ~MusicPostProcesser()
	{
		if (lm_ != NULL)
		{
			delete lm_;
			lm_ = NULL;
		}
	}

	int Init(const std::string& cfg_path);
	int Uinit();
	virtual int PostProcess(const QpRequest& qp_request, QpResult* qp_result);
	virtual std::string GetDomain()
	{
		return "music";
	}

private:
	int Violate(const NeTerm& ne_termi, const NeTerm& ne_termj);

	LanguageModel* lm_;

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_QP_MUSIC_POST_PROCESSER_
