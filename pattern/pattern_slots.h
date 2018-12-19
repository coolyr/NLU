//author xhz105925
//date 2016/5/31

#ifndef DM_QP_PATTERN_SLOTS_H_
#define DM_QP_PATTERN_SLOTS_H_

#include <string>
#include <map>
#include <vector>
#include "../common_macros.h"
#include "../log.h"
#include "../mylib/pcre++.h"
#include "pattern_slot.h"


DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternSlots
{
public:
	PatternSlots();
	~PatternSlots();
	void SetDomainName(std::string domain_name);
	const std::string GetDomainName() const;
	int LoadSlot(const char * xml_path);
	const std::map<std::string, pcrepp::Pcre*>* GetSynxPcre() const;
	const std::map<std::string, PatternSlot>* GetSynx2Slot() const;
	void Destroy();
private:
	DIALOG_SEG_LOG_DECLARE();
	std::string domain_;
	std::map<std::string, PatternSlot> synx2slot_;
	std::map<std::string, pcrepp::Pcre*> synx2pcre_;

}; // END OF CLASS PATTERNSLOTS

DIALOG_SEG_END_NAMESPACE(qp);
#endif
