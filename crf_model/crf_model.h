#ifndef DM_QP_CRF_MODEL_H
#define DM_QP_CRF_MODEL_H

#include "../mylib/crfpp.h"
#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include <string>
#include <vector>

DIALOG_SEG_BEGIN_NAMESPACE(qp);


class CRFModel
{
public:
	CRFModel();
	~CRFModel();

	int Init(const std::string& cfg_path);
	int Process(const QpRequest& qp_request, QpResult* qp_result);

private:
	int Parse(const std::vector<std::string>& sequence, std::vector<std::string>&tags);
	void Split2chs(const std::string& word, std::vector<std::string>& characters);

	CRFPP::Model* model_;

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif //DM_QP_CRF_MODEL_H
