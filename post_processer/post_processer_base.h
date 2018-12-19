#ifndef DM_QP_POST_PROCESSER_BASE_H
#define DM_QP_POST_PROCESSER_BASE_H

#include "../proto/qp.pb.h"
#include "../common_macros.h"
#include "../log.h"
#include <string>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PostProcesserBase
{
public:
	PostProcesserBase() {}
	virtual ~PostProcesserBase() {}

	virtual int PostProcess(const QpRequest& qp_request, QpResult* qp_result) = 0;//虚函数，必须重载实现
	virtual std::string GetDomain() = 0;
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_QP_POST_PROCESSER_BASE_H
