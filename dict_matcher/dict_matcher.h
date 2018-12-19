#ifndef DM_QP_DICT_MATCHER_H
#define DM_QP_DICT_MATCHER_H

#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include <string>
#include <vector>
#include <map>

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class NeInfor
{
public:
	std::string type_;				//ʵ������
	std::string normal_text_;		//DictMatcher��δ�����ֶ�
};

class DictMatcher
{
public:
	DictMatcher() { ne_dict_ = NULL; }
	~DictMatcher()
	{
		if (ne_dict_)
		{
			delete ne_dict_;
			ne_dict_ = NULL;
		}
	}

	int Init(const std::string& cfg_path);
	int Process(const QpRequest& qp_request, QpResult* qa_result);

private:
	std::map<std::string, std::vector<NeInfor> >* ne_dict_;

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_QP_DICT_MATCHER_H
