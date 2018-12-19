#ifndef DM_QP_TELEPHONE_POST_PROCESSER_H
#define DM_QP_TELEPHONE_POST_PROCESSER_H

#include "../../post_processer/post_processer_base.h"
#include "../../proto/qp.pb.h"
#include <string>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class TelephonePostProcesser : public PostProcesserBase
{
public:
	TelephonePostProcesser() {}
	virtual ~TelephonePostProcesser() {}

	int Init(const std::string& cfg_path);
	int Uinit();
	virtual int PostProcess(const QpRequest& qp_request, QpResult* qp_result);
	virtual std::string GetDomain()
	{
		return "telephone";
	}

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_QP_TELEPHONE_POST_PROCESSER_
