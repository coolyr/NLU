//author xhz105925
//date 2016/6/20
//
#ifndef DM_QP_DOMAIN_MAP_H_
#define DM_QP_DOMAIN_MAP_H_

#include "pattern_domain.h"
#include "../common_macros.h"
#include "../log.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
class DomainMap : public PatternDomain
{
public:
	DomainMap();
	virtual ~DomainMap();
	virtual void AdjustSlot(PatternResult& res);
private:
	DIALOG_SEG_LOG_DECLARE();

};



DIALOG_SEG_END_NAMESPACE(qp);
#endif
