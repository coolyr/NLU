//author xhz105925
//date 2016/5/31

#include "pattern_slot.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, PatternSlot);

using std::string;

PatternSlot::PatternSlot() {}
PatternSlot::~PatternSlot() {}

void PatternSlot::SetName(string name)
{
	name_ = name;
}

void PatternSlot::SetDomain(string domain)
{
	domain_ = domain;
}

void PatternSlot::SetWeight(string weight)
{
	weight_ = weight;
}

void PatternSlot::SetSynx(string synx)
{
	synx_ = synx;
}

void PatternSlot::SetSynxPrefix(string prefix)
{
	synx_prefix_ = prefix;
}

const string PatternSlot::GetName() const
{
	return name_;
}

const string PatternSlot::GetDomain() const
{
	return domain_;
}

const string PatternSlot::GetWeight() const
{
	return weight_;
}

const string PatternSlot::GetSynx() const
{
	return synx_;
}

const string PatternSlot::GetSynxPrefix() const
{
	return synx_prefix_;
}

DIALOG_SEG_END_NAMESPACE(qp);
