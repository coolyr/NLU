//author xhz105925
//date 2016/5/31

#include "pattern_result.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;
using std::vector;
using std::pair;

DIALOG_SEG_LOG_SETUP(qp, PatternResult);

PatternResult::PatternResult() :weight_(0) {}
PatternResult::~PatternResult() {}

void PatternResult::SetRawQuery(string query)
{
	raw_query_ = query;
}

void PatternResult::SetSegQuery(string seg_query)
{
	seg_query_ = seg_query;
}

void PatternResult::SetNerQuery(string ner_token_query)
{
	ner_token_query_ = ner_token_query;
}

void PatternResult::SetSegTokenQuery(string seg_token_query)
{
	seg_token_query_ = seg_token_query;
}

void PatternResult::SetDomain(string domain)
{
	domain_ = domain;
}

const string PatternResult::GetDomain() const
{
	return domain_;
}

const string PatternResult::GetRawQuery() const
{
	return raw_query_;
}

const string PatternResult::GetSegQuery() const
{
	return seg_query_;
}

const string PatternResult::GetNerQuery() const
{
	return ner_token_query_;
}

const string PatternResult::GetSegTokenQuery() const
{
	return seg_token_query_;
}

void PatternResult::SetScore(float score)
{
	score_ = score;
}

void PatternResult::SetIntentScore(float score)
{
	intent_score_ = score;
}
void PatternResult::SetHasNer(bool is)
{
	has_ner_ = is;
}

void PatternResult::SetSlotNum(size_t num)
{
	slot_num_ = num;
}

float PatternResult::GetScore()
{
	return score_;
}

float PatternResult::GetIntentScore()
{
	return intent_score_;
}
bool PatternResult::HasNer()
{
	return has_ner_;
}

size_t PatternResult::GetSlotNum()
{
	return slot_num_;
}

const string PatternResult::GetPatternType() const
{
	return pattern_type_;
}

void PatternResult::AddNersKV(string key, string value)
{
	ners_kv_[key] = value;
}

void PatternResult::AddSlotsKV(string key, string value)
{
	slots_kv_[key] = value;
}

void PatternResult::CleanSlotsKV()
{
	slots_kv_.clear();
}

void PatternResult::DelNersKV(const string key)
{
	if (ners_kv_.find(key) != ners_kv_.end())
	{
		ners_kv_.erase(key);
	}
}

void PatternResult::DelSlotsKV(const string key)
{
	if (slots_kv_.find(key) != slots_kv_.end())
	{
		slots_kv_.erase(key);
	}
}

void PatternResult::SetPatternType(string type)
{
	pattern_type_ = type;
}
const string PatternResult::GetSlotByKey(string slot_name)
{
	return slots_kv_[slot_name];
}
const map<string, string>* PatternResult::GetSlotMap() const
{
	return &slots_kv_;
}
const map<string, string>* PatternResult::GetNersMap() const
{
	return &ners_kv_;
}
void PatternResult::ScoreIntent()
{
	if (!pattern_type_.compare("EXACT"))
	{
		SetIntentScore(1.0);
	}
	else if (!pattern_type_.compare("FUZZY"))
	{
		SetIntentScore(0.7);
	}
	else if (!pattern_type_.compare("LOCAL"))
	{
		SetIntentScore(0.8);
	}
	else
	{
		SetIntentScore(0.0);
	}

}
void PatternResult::AddNersPair(string key, string value)
{
	ners_pair_.push_back(make_pair(key, value));
}

void PatternResult::AddSlotsPair(string key, string value)
{
	slots_pair_.push_back(make_pair(key, value));
}

//ners_kv['singer'] = ners_pair['singer']
void PatternResult::AdjustNersKV()
{
	//ners_pair  里面是实体		=>	<type, text> : <singer, 刘德华> 【里面是后处理器的DomainQpResult解析出的实体】
	for (vector<pair<string, string> >::iterator it = ners_pair_.begin(); it != ners_pair_.end(); it++)
	{
		// ners_kv  里面是实体   =>   <type, text> : <singer, 刘德华>
		if (ners_kv_.find(it->first) == ners_kv_.end())// 当 ners_kv中没有 singer
		{
			ners_kv_[it->first] = it->second;// ners_kv['singer'] = ners_pair['singer']  ??  ners_kv 和 mers_pair不是一样么？
		}
		else
		{
			LOG(INFO, "there is more than one duplicate slot:%s", it->first.c_str());
		}
	}
}

//slots_kv['【我】'] = slots_pair['【我】']
void PatternResult::AdjustSlotsKV()
{
	//slots_pair  slots_pair 里面加载的是非实体？  => [<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
	for (vector<pair<string, string> >::iterator it = slots_pair_.begin(); it != slots_pair_.end(); it++)
	{
		//slots_kv 意图对象 <intent, play_instrument> <domain, music>
		if (slots_kv_.find(it->first) == slots_kv_.end())
		{
			slots_kv_[it->first] = it->second; //slots_kv['【我】'] = slots_pair['【我】']
		}// slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		else
		{
			LOG(INFO, "there is more than one duplicate ne-slot:%s", it->first.c_str());
		}
	}
}

const vector<pair<string, string> >& PatternResult::GetSlotPairs() const
{
	return slots_pair_;
}

const vector<pair<string, string> >& PatternResult::GetNerPairs() const
{
	return ners_pair_;
}
void PatternResult::SetWeight(size_t weight)
{
	weight_ = weight;
}
size_t PatternResult::GetWeight() const
{
	return weight_;
}
DIALOG_SEG_END_NAMESPACE(qp);

