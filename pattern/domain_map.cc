#include "domain_map.h"
#include <map>
#include <set>
#include <vector>
DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;
using std::set;
using std::vector;
using std::pair;

DIALOG_SEG_LOG_SETUP(qp, DomainMap);

DomainMap::DomainMap() {}
DomainMap::~DomainMap() {}

void DomainMap::AdjustSlot(PatternResult& res)
{
	/*
	"����/song �� ���»�/[singer,per] �� ��" => "���� �� ���»�/singer �� ��"

	slots_pair �����Ƿ�ʵ��		=>		[<���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
	ners_pair  ������ʵ��		=>		<singer, ���»�> �������Ǻ�������DomainQpResult��������ʵ�塿

	synx_token=seg_token_query= "���ҡ� ��Ҫ�� ������ ��singer�� ���ġ� ��song��"
	slots_kv �����ͼ���� <intent, play_instrument> <domain, music>
	########################################################################################################################
	map��
	slots_kv��� <"start", "�����">, <"arrival", "�ٶȴ���">
	*/

	const vector<pair<string, string> > slots = res.GetSlotPairs();	//slots_pair
	const vector<pair<string, string> > ners = res.GetNerPairs();	//ners_pair
	set<string> slot_set;
	for (vector<pair<string, string> >::const_iterator it = ners.begin(); it != ners.end(); it++)//ners_pair [<loc,�����>, <poi,��ӰԺ>]
	{
		//���룿��
		string intent = res.GetSlotByKey("intent");	//slots_kv  ��ͼ���� <intent, play_instrument> <domain, music>
		if (!intent.compare("nearby"))// 'nearby'
		{

		}
		else if (!intent.compare("route_start_arrival"))//"route_start_arrival" <description>�Ӱٶȴ��õ��ϵص���վ</description>
		{
			//ners_pair����'loc'
			if (!it->first.compare("loc") && slot_set.find("loc") == slot_set.end())
			{
				//slots_kv_;//��ͼ���� <intent, play_instrument> <domain, music>
				res.AddSlotsKV("start", it->second);//slots_kv��� <"start", "�����">
				slot_set.insert("loc");//'loc'
			}
			else if (!it->first.compare("loc") && slot_set.find("loc") != slot_set.end())
			{
				//Ĭ�ϵ�һ��start
				res.AddSlotsKV("arrival", it->second);// slots_kv��� <"arrival", "�����">
			}
		}
		else if (!intent.compare("201"))// "201"
		{
			if (!it->first.compare("loc") && slot_set.find("loc") == slot_set.end() && slot_set.find("poi") == slot_set.end())
			{
				res.AddSlotsKV("start", it->second);
				slot_set.insert("loc");
			}
			else if (!it->first.compare("loc") && (slot_set.find("loc") != slot_set.end() || slot_set.find("poi") != slot_set.end()))
			{
				res.AddSlotsKV("arrival", it->second);
			}
			else if (!it->first.compare("poi") && slot_set.find("poi") == slot_set.end() && slot_set.find("loc") == slot_set.end())
			{
				res.AddSlotsKV("start", it->second);
				slot_set.insert("poi");
			}
			else if (!it->first.compare("poi") && (slot_set.find("loc") != slot_set.end() || slot_set.find("poi") != slot_set.end()))
			{
				res.AddSlotsKV("arrival", it->second);
			}
		}
		else if (!intent.compare("203") || !intent.compare("202"))// "202" || "203"
		{
			if (!it->first.compare("loc"))
			{
				res.AddSlotsKV("arrival", it->second);
			}
			else if (!it->first.compare("poi"))
			{
				res.AddSlotsKV("arrival", it->second);
			}
		}
	}//ners_pair [<loc,����>, <poi,�����>]

	res.AdjustNersKV();
	res.AdjustSlotsKV();
}


DIALOG_SEG_END_NAMESPACE(qp);


