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
	float score_;					//<4>	[分类]得分
	float intent_score_;			//<14>	意图得分  根据模板匹配结果 [<EXACT : 1>, <LOCAL : 0.8>, <FUZZY, 0.7>]
	bool has_ner_;					//<5>	是否由 实体词
	int slot_num_;					//<15>	直接匹配中设置
	size_t weight_;					//<12>	设置权重[默认初始化0]
	std::string domain_;			//<3>	域名
	std::string raw_query_;			//<1>	原始query  raw_query
	std::string seg_query_;			//<2>	空格隔开的切词  seg_sen = "导航 去 五道口"
	std::string seg_token_query_;	//<6>	"我 想 听 singer 的 song"  【当无实体词时==> 空格隔开的切词  seg_token_query = seg_sen = "导航 去 五道口"】
	std::string ner_token_query_;	//<7>	ne_sentense => MaxEnt未填充 [后处理器填充 :"我想听/o刘德华/singer的/o忘情水/song(/o)"]
	std::string pattern_type_;		//<11>	匹配到的正则的type  = [EXACT, FUZZY, LOCAL]
	std::string intention_;
	std::map<std::string, std::string> slots_kv_;//<13>		意图对象 <intent, play_instrument> <domain, music>  
	/*
	PatternDomain => AdjustSlot操作后
	music 和 telepthon 类型：
			添加：	slots_kv['【我】'] = slots_pair['我'] 
					slots_kv = [<intent, play_instrument>, <domain, music>, <【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>]
	map类型:
			
	*/
	std::vector<std::pair<std::string, std::string> > slots_pair_;	//<10> slots_pair 里面加载的是非实体？ => 归一化后的 <Normalize Slot, Slot>
	//=> [<【我】, 我>, <【要】, 想>, <【听】，听>, <【的】，的>] 【注意：真实情况【我】是去掉中括号的】
	std::map<std::string, std::string> ners_kv_;					//<8> 后处理器获得的DomainQpResult中的实体 <type, text>  => <singer, 刘德华>
	std::vector<std::pair<std::string, std::string> > ners_pair_;	//<9> 后处理器获得的DomainQpResult中的实体 <type, text>  => <singer, 刘德华>

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

