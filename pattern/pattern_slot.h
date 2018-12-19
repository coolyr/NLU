//author xhz105925
//date 2016/5/31

#ifndef DM_QP_PATTERN_SLOT_H_
#define DM_QP_PATTERN_SLOT_H_

#include <string>
#include "../common_macros.h"
#include "../log.h"


DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternSlot
{
public:
	PatternSlot();
	~PatternSlot();
	void SetName(std::string name);
	void SetDomain(std::string domain);
	void SetWeight(std::string weight);
	void SetSynx(std::string synx);
	void SetSynxPrefix(std::string prefix);
	const std::string GetName() const;
	const std::string GetDomain() const;
	const std::string GetWeight() const;
	const std::string GetSynx() const;
	const std::string GetSynxPrefix() const;

private:
	DIALOG_SEG_LOG_DECLARE();
	//<slot name="������" weight="high" synx="(ǰ��|��)"/>
	//<slot name = "��ȥ��" weight = "high" synx = "(ȥ|��|��|��|��|ֱ��|ǰ��|go|����|�н�|ǰ��|�ִ�)" / >
	std::string name_;			// name =��ȥ��
	std::string domain_;		// domain = <templates domainId="map">
	std::string weight_;		// weight = weight = "high"
	std::string synx_;			// synx = "(ȥ|��|��|��|��|ֱ��|ǰ��|go|����|�н�|ǰ��|�ִ�)"
	std::string synx_prefix_;	// synx_prefix = "?"
};

DIALOG_SEG_END_NAMESPACE(qp);
#endif // END OF DM_OP_PATTERN_SLOT_H_
