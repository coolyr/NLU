#include "pattern_domain.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);


DIALOG_SEG_LOG_SETUP(qp, PatternDomain);

PatternDomain::PatternDomain() {}
PatternDomain::~PatternDomain() {}

void PatternDomain::AdjustSlot(PatternResult& res)
{
	res.AdjustNersKV();		//ners_kv['singer'] = ners_pair['singer']    ners_kv 和 mers_pair不是一样么？ 【里面是后处理器的DomainQpResult解析出的实体】
	res.AdjustSlotsKV();	//slots_kv['【我】'] = slots_pair['我'] slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
}

DIALOG_SEG_END_NAMESPACE(qp);



