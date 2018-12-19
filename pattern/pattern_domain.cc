#include "pattern_domain.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);


DIALOG_SEG_LOG_SETUP(qp, PatternDomain);

PatternDomain::PatternDomain() {}
PatternDomain::~PatternDomain() {}

void PatternDomain::AdjustSlot(PatternResult& res)
{
	res.AdjustNersKV();		//ners_kv['singer'] = ners_pair['singer']    ners_kv �� mers_pair����һ��ô�� �������Ǻ�������DomainQpResult��������ʵ�塿
	res.AdjustSlotsKV();	//slots_kv['���ҡ�'] = slots_pair['��'] slots_kv = [<intent, play_instrument>, <domain, music>, <���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
}

DIALOG_SEG_END_NAMESPACE(qp);



