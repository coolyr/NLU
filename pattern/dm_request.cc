//author xhz105925
//date 2016/6/13
#include "dm_request.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
using std::string;
using std::vector;

DIALOG_SEG_LOG_SETUP(qp, DmRequest);

DmRequest::DmRequest() {}
DmRequest::~DmRequest() {}
const string DmRequest::GetQuery() const
{
	return query_;
}
const string DmRequest::GetSeg() const
{
	return seg_;
}
const vector<DmDomain>* DmRequest::GetDomains() const
{
	return &domains_;
}
void DmRequest::SetQuery(string query)
{
	query_ = query;
}
void DmRequest::SetSeg(string seg)
{
	seg_ = seg;
}
void DmRequest::AddDomain(DmDomain domain)
{
	domains_.push_back(domain);
}
/*
message NluResult {
	required string query = 1;
	optional string seg = 2;
	repeated Domain domains = 3;  //领域 【list】
}
*/
void DmRequest::OutputInfo(NluResult& nlu_result)
{
	nlu_result.set_query(GetQuery());					//原始query  raw_query
	nlu_result.set_seg(GetSeg());						//空格隔开的切词  seg_sen = "导航 去 五道口"
	Domain* domain = nlu_result.add_domains();
	const vector<DmDomain>* dm_domain = GetDomains();	//domain域对象， 可以有多个【list】
	for (vector<DmDomain>::const_iterator it = dm_domain->begin(); it != dm_domain->end(); it++)//domains【与 MaxEnt分类结果DomainQpResult对应，目前只有一个】 
	{
		domain->set_domain_name(it->GetName());					//域名
		domain->set_domain_score(it->GetDomainScore());			//[分类]得分
		const vector<DmIntent>* dm_intents = it->GetIntents();	//意图对象【list】
		for (vector<DmIntent>::const_iterator ii = dm_intents->begin(); ii != dm_intents->end(); ii++)//意图对象【list】
		{
			Intent* intent = domain->add_domain_intents();
			intent->set_intent_name(ii->GetName());		//意图名play_instrument     slots_kv[intent] = play_instrument
			intent->set_intent_score(ii->GetScore());	//意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
			const DmSlot* dm_slots = ii->GetSlot();		//itent里slot唯一
			size_t kcnt = dm_slots->GetKeys()->size();
			if (kcnt != dm_slots->GetValues()->size())
			{
				LOG(INFO, "Different size of Keys and values");
			}
			else
			{
				Slot* slot = intent->mutable_intent_slots();
				for (size_t cnt = 0; cnt < kcnt; cnt++)//遍历所有的kv
				{
					/*
						slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
						ners_kv = <singer, 刘德华>
					*/
					slot->add_slot_key(dm_slots->GetKey(cnt));
					slot->add_slot_value(dm_slots->GetValue(cnt));
				}
			}
		}//意图对象【list】
	}//domains
}

DIALOG_SEG_LOG_SETUP(qp, DmDomain);

DmDomain::DmDomain() {};
DmDomain::~DmDomain() {};

const string DmDomain::GetName() const
{
	return domain_name_;
}
const float DmDomain::GetDomainScore() const
{
	return domain_score_;
}
const vector<DmIntent>* DmDomain::GetIntents() const
{
	return &domain_intents_;
}
void DmDomain::SetName(string name)
{
	domain_name_ = name;
}
void DmDomain::SetDomainScore(float score)
{
	domain_score_ = score;
}
void DmDomain::AddIntent(DmIntent intent)
{
	domain_intents_.push_back(intent);
}

DIALOG_SEG_LOG_SETUP(qp, DmSlot);

DmSlot::DmSlot() {};
DmSlot::~DmSlot() {};

const vector<string>* DmSlot::GetKeys() const
{
	return &slot_keys_;
}
const vector<string>* DmSlot::GetValues() const
{
	return &slot_values_;
}
const string DmSlot::GetKey(int i) const
{
	return slot_keys_[i];
}
const string DmSlot::GetValue(int i) const
{
	return slot_values_[i];
}
void DmSlot::AddKey(string key)
{
	slot_keys_.push_back(key);
}
void DmSlot::AddValue(string value)
{
	slot_values_.push_back(value);
}
void DmSlot::AddKeyValue(string key, string value)
{
	AddKey(key);
	AddValue(value);
}

DIALOG_SEG_LOG_SETUP(qp, DmIntent);

DmIntent::DmIntent() {}
DmIntent::~DmIntent() {}

const string DmIntent::GetName() const
{
	return intent_name_;
}
const float DmIntent::GetScore() const
{
	return intent_score_;
}
const DmSlot* DmIntent::GetSlot() const
{
	return &intent_slot_;
}
void DmIntent::SetName(string name)
{
	intent_name_ = name;
}
void DmIntent::SetScore(float score)
{
	intent_score_ = score;
}
void DmIntent::SetSlot(DmSlot slot)
{
	intent_slot_ = slot;
}

DIALOG_SEG_END_NAMESPACE(qp);
