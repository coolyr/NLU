//author xhz105925
//date 2016/5/31

#ifndef DM_QP_PATTERN_RESULT_H_
#define DM_QP_PATTERN_RESULT_H_

#include <string>
#include <utility>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include "../mylib/pcre++.h"
//#include "../pcre.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternResult
{
public:
	PatternResult();
	~PatternResult();

	void SetRawQuery(std::string query);
	void SetSegQuery(std::string seg_query);
	void SetNerQuery(std::string ner_token_query);
	void SetSegTokenQuery(std::string seg_token_query);
	void SetDomain(std::string domain);
	void SetPatternType(std::string type);

	const std::string GetRawQuery() const;
	const std::string GetSegQuery() const;
	const std::string GetNerQuery() const;
	const std::string GetSegTokenQuery() const;
	const std::string GetDomain() const;

	void SetScore(float score);
	void SetIntentScore(float score);
	void SetHasNer(bool is);
	void SetSlotNum(size_t num);
	void ScoreIntent();
	void SetWeight(size_t weight);


	float GetScore();
	float GetIntentScore();
	bool HasNer();
	size_t GetSlotNum();
	const std::string GetPatternType() const;
	size_t GetWeight() const;
	void CleanSlotsKV();

	void AddNersKV(std::string key, std::string value);
	void AddSlotsKV(std::string key, std::string value);
	void AddNersPair(std::string key, std::string value);
	void AddSlotsPair(std::string key, std::string value);
	const std::string GetSlotByKey(std::string slot_name);
	const std::map<std::string, std::string>* GetSlotMap() const;
	const std::map<std::string, std::string>* GetNersMap() const;
	void AdjustNersKV();
	void AdjustSlotsKV();
	const std::vector<std::pair<std::string, std::string> >& GetSlotPairs() const;
	const std::vector<std::pair<std::string, std::string> >& GetNerPairs() const;
	void DelNersKV(const std::string key);
	void DelSlotsKV(const std::string key);

private:
	DIALOG_SEG_LOG_DECLARE();
	float score_;					//<4>	[����]�÷�
	float intent_score_;			//<14>	��ͼ�÷�  ����ģ��ƥ���� [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
	bool has_ner_;					//<5>	�Ƿ��� ʵ���
	int slot_num_;					//<15>	ֱ��ƥ��������
	size_t weight_;					//<12>	����Ȩ��[Ĭ�ϳ�ʼ��0]
	std::string domain_;			//<3>	����
	std::string raw_query_;			//<1>	ԭʼquery  raw_query
	std::string seg_query_;			//<2>	�ո�������д�  seg_sen = "���� ȥ �����"
	std::string seg_token_query_;	//<6>	"�� �� �� singer �� song"  ������ʵ���ʱ==> �ո�������д�  seg_token_query = seg_sen = "���� ȥ �����"��
	std::string ner_token_query_;	//<7>	ne_sentense => MaxEntδ��� [��������� :"������/o���»�/singer��/o����ˮ/song(/o)"]
	std::string pattern_type_;		//<11>	ƥ�䵽�������type  = [EXACT, FUZZY, LOCAL]
	std::string intention_;
	std::map<std::string, std::string> slots_kv_;//<13>		��ͼ���� <intent, play_instrument> <domain, music>  
	/*
	PatternDomain => AdjustSlot������
	music �� telepthon ���ͣ�
			��ӣ�	slots_kv['���ҡ�'] = slots_pair['��'] 
					slots_kv = [<intent, play_instrument>, <domain, music>, <���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>]
	map����:
			
	*/
	std::vector<std::pair<std::string, std::string> > slots_pair_;	//<10> slots_pair ������ص��Ƿ�ʵ�壿 => ��һ����� <Normalize Slot, Slot>
	//=> [<���ҡ�, ��>, <��Ҫ��, ��>, <����������>, <���ġ�����>] ��ע�⣺��ʵ������ҡ���ȥ�������ŵġ�
	std::map<std::string, std::string> ners_kv_;					//<8> ��������õ�DomainQpResult�е�ʵ�� <type, text>  => <singer, ���»�>
	std::vector<std::pair<std::string, std::string> > ners_pair_;	//<9> ��������õ�DomainQpResult�е�ʵ�� <type, text>  => <singer, ���»�>

}; //end class PatternResult

/*
<semantics>
	<domain>music</domain>
	<intent>play_instrument</intent>
</semantics>

	<domain, music>
	<intent, play_instrument>
*/
DIALOG_SEG_END_NAMESPACE(qp);

#endif 

