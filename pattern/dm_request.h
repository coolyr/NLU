//author xhz105925
//date 2016/6/13
#ifndef DM_QP_DM_REQUEST_H_
#define DM_QP_DM_REQUSET_H_
#include <string>
#include <vector>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/nlu_dm.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
//class DmSlot;
class DmDomain;
class DmIntent;
class DmRequest
{
	/*
	message NluResult {
		required string query = 1;
		optional string seg = 2;
		repeated Domain domains = 3;  //领域 【list】
}
	*/
public:
	DmRequest();
	~DmRequest();
	const std::string GetQuery() const;
	const std::string GetSeg() const;
	const std::vector<DmDomain>* GetDomains() const;
	void SetQuery(std::string query);
	void SetSeg(std::string seg);
	void AddDomain(DmDomain domain);
	void OutputInfo(NluResult& nlu_result);
private:
	DIALOG_SEG_LOG_DECLARE();
	std::string query_;				//原始query  raw_query
	std::string seg_;				//空格隔开的切词  seg_sen = "导航 去 五道口"
	std::vector<DmDomain> domains_;	//domain域对象， 可以有多个【list】
};
class DmDomain
{
	/*
	message Domain {
		required string domain_name = 1;	//领域
		optional float domain_score = 2;	//[分类]得分
		repeated Intent domain_intents = 3;	//意图 【list】
	}
	*/
public:
	DmDomain();
	~DmDomain();
	const std::string GetName() const;
	const float GetDomainScore() const;
	const std::vector<DmIntent>* GetIntents() const;
	void SetName(std::string name);
	void SetDomainScore(float score);
	void AddIntent(DmIntent intent);
private:
	DIALOG_SEG_LOG_DECLARE();
	std::string domain_name_;				//域名
	float domain_score_;					//[分类]得分
	std::vector<DmIntent> domain_intents_;	//意图对象【list】
	/*
	message Intent {
		required string intent_name = 1;  //意图名 play_instrument     slots_kv[intent] = play_instrument
		optional float intent_score = 2;  //意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
		required Slot intent_slots = 3;   //slot信息
	}
	*/
};
class DmSlot
{
	/*
	message Slot {
		//slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		//ners_kv = <singer, 刘德华>
		repeated string slot_key = 1;	// slot key
		repeated string slot_value = 2; // slot value
	}	 
	*/
public:
	DmSlot();
	~DmSlot();
	const std::vector<std::string>* GetKeys() const;
	const std::vector<std::string>* GetValues() const;
	const std::string GetKey(int i) const;
	const std::string GetValue(int i) const;
	void AddKey(std::string key);
	void AddValue(std::string value);
	void AddKeyValue(std::string key, std::string value);
private:
	/*
	music 和 telepon类
		slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		ners_kv = <singer, 刘德华>
	map类
		slots_kv添加 <"start", "五道口">, <"arrival", "百度大厦">
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::vector<std::string> slot_keys_;	// 【我】，	singer
	std::vector<std::string> slot_values_;	//	我，		刘德华
};
class DmIntent
{
	/*
	message Intent {
		required string intent_name = 1;  //意图名
		optional float intent_score = 2;  //意图得分
		required Slot intent_slots = 3;   //slot信息  【list】
	}
	*/
public:
	DmIntent();
	~DmIntent();
	const std::string GetName() const;
	const float GetScore() const;
	const DmSlot* GetSlot() const;
	void SetName(std::string name);
	void SetScore(float score);
	void SetSlot(DmSlot slot);
private:
	DIALOG_SEG_LOG_DECLARE();
	std::string intent_name_;	//意图名play_instrument     slots_kv[intent] = play_instrument
	DmSlot intent_slot_;		//itent里slot唯一
	float intent_score_;		//意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
	/*
	message Slot {
		//slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
		//ners_kv = <singer, 刘德华>
		repeated string slot_key = 1;	// slot key
		repeated string slot_value = 2; // slot value
	}
	*/
};


DIALOG_SEG_END_NAMESPACE(qp);

#endif
