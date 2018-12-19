//author xhz105925
//date 2016/6/2

#ifndef DM_QP_PATTERN_STRUCT_H_
#define DM_QP_PATTERN_STRUCT_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include "pattern_template.h"
#include "../mylib/pcre++.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternTemplates
{
public:

	PatternTemplates();
	~PatternTemplates();
	void SetDomainName(std::string name);
	std::string GetDomainName();
	const PatternTemplate GetTemplate(std::string synx);
	const std::map<std::string, pcrepp::Pcre*>* GetMapSynxPcre() const;
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//	自定义	map_synx_rule_
	const std::map<std::string, std::string>* GetMapSynxRule() const;	//自定义
	int AddTemplatesByRule(const std::string domain, const std::string intent, const std::set<std::string> rules);
	std::string GenerateSynxFromRule(std::string rule);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	int LoadTemplates(const char* xml_path);
	void Destroy();


private:
	DIALOG_SEG_LOG_DECLARE();
	void GenIndexMaps();

	std::string domain_name_;									// domain名
	std::vector<PatternTemplate> templates_;					// List[ patternTemplate1, ...]
	std::map<std::string, PatternTemplate> map_synx_template_;	// synx		"^(【我】)?(【想】)?【导航】$" ->  PatternTemplate
	std::map<std::string, pcrepp::Pcre*> map_synx_pcre_;		// synx		"^(【我】)?(【想】)?【导航】$" ->  Pcre(synx)
	std::set<std::string> template_id_;							// set("ROUTE", "ROUTE_ARRIVAL",..)   <template id="ROUTE">  <template id="ROUTE_ARRIVAL">

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//	自定义	map_synx_rule_
	//	<pattern type = "EXACT" rule="订一张@<date>去@<loc>的机票">^订一张.*去.*的机票$< / pattern>
	std::map<std::string, std::string> map_synx_rule_;			// synx		"^订一张去.*的机票$"	-> "^订一张去@<loc>的机票$"
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
}; //end class PatternTemplates


DIALOG_SEG_END_NAMESPACE(qp);

#endif 

