#include "domain_call.h"
#include <map>
#include <set>
#include <vector>
DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;
using std::set;
using std::vector;
using std::pair;

DIALOG_SEG_LOG_SETUP(qp, DomainCall);

DomainCall::DomainCall() {}
DomainCall::~DomainCall() {}

void DomainCall::AdjustSlot(PatternResult& res)
{
	/*
	"我想/song 听 刘德华/[singer,per] 的 歌" => "我想 听 刘德华/singer 的 歌"

	slots_pair 里面是非实体？		=>	[<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
	ners_pair  里面是实体		=>	<type, text> ： <singer, 刘德华> 【里面是后分类器的DomainQpResult解析出的实体】
	*/
	const vector<pair<string, string> > slots = res.GetSlotPairs();
	const vector<pair<string, string> > ners = res.GetNerPairs();
	for (vector<pair<string, string> >::const_iterator it = ners.begin(); it != ners.end(); it++)
	{
		set<string> slot_set;
		string intent = res.GetSlotByKey("intent");//slots_kv  意图对象 <intent, play_instrument> <domain, music>
		LOG(INFO, "slots=========call %s", it->first.c_str());// singer
	}

	res.AdjustNersKV();
	res.AdjustSlotsKV();


}


DIALOG_SEG_END_NAMESPACE(qp);


