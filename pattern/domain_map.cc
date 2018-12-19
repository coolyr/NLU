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
	"我想/song 听 刘德华/[singer,per] 的 歌" => "我想 听 刘德华/singer 的 歌"

	slots_pair 里面是非实体		=>		[<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
	ners_pair  里面是实体		=>		<singer, 刘德华> 【里面是后处理器的DomainQpResult解析出的实体】

	synx_token=seg_token_query= "【我】 【要】 【听】 【singer】 【的】 【song】"
	slots_kv 添加意图对象 <intent, play_instrument> <domain, music>
	########################################################################################################################
	map类
	slots_kv添加 <"start", "五道口">, <"arrival", "百度大厦">
	*/

	const vector<pair<string, string> > slots = res.GetSlotPairs();	//slots_pair
	const vector<pair<string, string> > ners = res.GetNerPairs();	//ners_pair
	set<string> slot_set;
	for (vector<pair<string, string> >::const_iterator it = ners.begin(); it != ners.end(); it++)//ners_pair [<loc,五道口>, <poi,电影院>]
	{
		//代码？？
		string intent = res.GetSlotByKey("intent");	//slots_kv  意图对象 <intent, play_instrument> <domain, music>
		if (!intent.compare("nearby"))// 'nearby'
		{

		}
		else if (!intent.compare("route_start_arrival"))//"route_start_arrival" <description>从百度大厦到上地地铁站</description>
		{
			//ners_pair里有'loc'
			if (!it->first.compare("loc") && slot_set.find("loc") == slot_set.end())
			{
				//slots_kv_;//意图对象 <intent, play_instrument> <domain, music>
				res.AddSlotsKV("start", it->second);//slots_kv添加 <"start", "五道口">
				slot_set.insert("loc");//'loc'
			}
			else if (!it->first.compare("loc") && slot_set.find("loc") != slot_set.end())
			{
				//默认第一个start
				res.AddSlotsKV("arrival", it->second);// slots_kv添加 <"arrival", "五道口">
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
	}//ners_pair [<loc,北京>, <poi,五道口>]

	res.AdjustNersKV();
	res.AdjustSlotsKV();
}


DIALOG_SEG_END_NAMESPACE(qp);


