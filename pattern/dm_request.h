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
		repeated Domain domains = 3;  //���� ��list��
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
	std::string query_;				//ԭʼquery  raw_query
	std::string seg_;				//�ո�������д�  seg_sen = "���� ȥ �����"
	std::vector<DmDomain> domains_;	//domain����� �����ж����list��
};
class DmDomain
{
	/*
	message Domain {
		required string domain_name = 1;	//����
		optional float domain_score = 2;	//[����]�÷�
		repeated Intent domain_intents = 3;	//��ͼ ��list��
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
	std::string domain_name_;				//����
	float domain_score_;					//[����]�÷�
	std::vector<DmIntent> domain_intents_;	//��ͼ����list��
	/*
	message Intent {
		required string intent_name = 1;  //��ͼ�� play_instrument     slots_kv[intent] = play_instrument
		optional float intent_score = 2;  //��ͼ�÷�  ����ģ��ƥ���� [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
		required Slot intent_slots = 3;   //slot��Ϣ
	}
	*/
};
class DmSlot
{
	/*
	message Slot {
		//slots_kv = [<intent, play_instrument>, <domain, music>, <���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
		//ners_kv = <singer, ���»�>
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
	music �� telepon��
		slots_kv = [<intent, play_instrument>, <domain, music>, <���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
		ners_kv = <singer, ���»�>
	map��
		slots_kv��� <"start", "�����">, <"arrival", "�ٶȴ���">
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::vector<std::string> slot_keys_;	// ���ҡ���	singer
	std::vector<std::string> slot_values_;	//	�ң�		���»�
};
class DmIntent
{
	/*
	message Intent {
		required string intent_name = 1;  //��ͼ��
		optional float intent_score = 2;  //��ͼ�÷�
		required Slot intent_slots = 3;   //slot��Ϣ  ��list��
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
	std::string intent_name_;	//��ͼ��play_instrument     slots_kv[intent] = play_instrument
	DmSlot intent_slot_;		//itent��slotΨһ
	float intent_score_;		//��ͼ�÷�  ����ģ��ƥ���� [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
	/*
	message Slot {
		//slots_kv = [<intent, play_instrument>, <domain, music>, <���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
		//ners_kv = <singer, ���»�>
		repeated string slot_key = 1;	// slot key
		repeated string slot_value = 2; // slot value
	}
	*/
};


DIALOG_SEG_END_NAMESPACE(qp);

#endif
