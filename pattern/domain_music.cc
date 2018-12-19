#include "domain_music.h"
#include <map>
#include <set>
#include <vector>
DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;
using std::set;
using std::vector;
using std::pair;

DIALOG_SEG_LOG_SETUP(qp, DomainMusic);

DomainMusic::DomainMusic() {}
DomainMusic::~DomainMusic() {}

void DomainMusic::AdjustSlot(PatternResult& res)
{
	/*
	"����/song �� ���»�/[singer,per] �� ��" => "���� �� ���»�/singer �� ��"

	slots_pair �����Ƿ�ʵ��		=>		[<���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
	ners_pair  ������ʵ��		=>		<singer, ���»�> �������Ǻ�������DomainQpResult��������ʵ�塿

	synx_token=seg_token_query= "���ҡ� ��Ҫ�� ������ ��singer�� ���ġ� ��song��"
	slots_kv �����ͼ���� <intent, play_instrument> <domain, music>
	*/
	const vector<pair<string, string> > slots = res.GetSlotPairs();
	const vector<pair<string, string> > ners = res.GetNerPairs();
	for (vector<pair<string, string> >::const_iterator it = ners.begin(); it != ners.end(); it++)
	{
		set<string> slot_set;
		string intent = res.GetSlotByKey("intent");//slots_kv  ��ͼ���� <intent, play_instrument> <domain, music>
		LOG(INFO, "slots=========music  %s", it->first.c_str());// singer

	}

	res.AdjustNersKV();
	res.AdjustSlotsKV();


}


DIALOG_SEG_END_NAMESPACE(qp);


