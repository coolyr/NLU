//author xhz105925
//date 2016/5/31

#ifndef DM_QP_PATTERN_TEMPLATE_H_
#define DM_QP_PATTERN_TEMPLATE_H_

#include <string>
#include <map>
#include "../common_macros.h"
#include "../log.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternTemplate
{
public:
	PatternTemplate();
	~PatternTemplate();
	void SetId(std::string id);
	void SetDomain(std::string domain);
	bool CheckDuplicate(std::string synx);
	void AddSynxType(std::string synx, std::string type);
	void AddSemantic(std::string key, std::string value);
	const std::string GetId() const;
	const std::string GetDomain() const;
	const std::string GetSynxType(std::string synx);
	const std::map<std::string, std::string> & GetMapSemantics() const;


	//<pattern type="EXACT">^(【我】)?(【想】)?【导航】$</pattern>
	std::map<std::string, std::string> map_synx_type_;	// <synx, type>	=>		< "^(【我】)?(【想】)?【导航】$", "EXACT">

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//自定义 map_synx_rule_
	std::map<std::string, std::string> map_synx_rule_;	// <synx, rule>	=>		<"^订一张.*去.*的机票$", "^订一张@<date>去@<loc>的机票$">
	void AddSynxRule(std::string synx, std::string rule);
	const std::string GetSynxRule(std::string synx);
	bool GetSlotUseRule(std::string query, std::string rule, std::map<std::string, std::string> &slotKV);
	bool SplitOnFirst(std::string sOriginal, char* pDividers, std::string& rsFirstPart, std::string& rsSecondPart);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
private:
	DIALOG_SEG_LOG_DECLARE();
	std::string id_;									//<template id="ROUTE">
	std::string domain_;								//<templates domainId="map">
	std::map<std::string, std::string> map_semantics_;  //<key, value> likewise  <intent, route> or <start, poi> 
	/*
	<semantics>
		<domain>music</domain>
        <intent>play_instrument</intent>
	</semantics>

	<domain, music>
	<intent, play_instrument>
	*/
};

DIALOG_SEG_END_NAMESPACE(qp);
#endif // END OF DM_OP_PATTERN_TEMPLATE_H_
