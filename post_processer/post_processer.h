#ifndef DM_QP_POST_PROCESSER_H
#define DM_QP_POST_PROCESSER_H

#include "../common_macros.h"
#include "../log.h"
#include "post_processer_base.h"
#include "map/map_post_processer.h"
#include "music/music_post_processer.h"
#include "telephone/telephone_post_processer.h"
#include <string>
#include <map>
#include "../proto/nlu_dm.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PostProcesser
{
public:
	PostProcesser() {}
	virtual ~PostProcesser();

	int Init(const std::string& cfg_path);
	int Uinit();

	int Process(const QpRequest& qp_request, QpResult* qp_result);

private:
	int RegisteProcesser(PostProcesserBase* post_processer);
	std::map<std::string, PostProcesserBase*> type2post_processer_; // map -> MapPostProcesser()  [map, music, telephone]ÎÞcalnedar£¿

private:
	DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif  //DM_QP_POST_PROCESSER_H
