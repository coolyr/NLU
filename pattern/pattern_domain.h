//author xhz105925
//date 2016/6/20
//
#ifndef DM_QP_PATTERNDOMAIN_H_
#define DM_QP_PATTERNDOMAIN_H_

#include "../common_macros.h"
#include "../log.h"
#include "pattern_result.h"
DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternDomain
{
public:
	PatternDomain();
	virtual ~PatternDomain();
	virtual void AdjustSlot(PatternResult& res);
	std::string domain_;
private:
	DIALOG_SEG_LOG_DECLARE();

};


DIALOG_SEG_END_NAMESPACE(qp);

#endif
