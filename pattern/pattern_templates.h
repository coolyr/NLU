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
	//	�Զ���	map_synx_rule_
	const std::map<std::string, std::string>* GetMapSynxRule() const;	//�Զ���
	int AddTemplatesByRule(const std::string domain, const std::string intent, const std::set<std::string> rules);
	std::string GenerateSynxFromRule(std::string rule);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	int LoadTemplates(const char* xml_path);
	void Destroy();


private:
	DIALOG_SEG_LOG_DECLARE();
	void GenIndexMaps();

	std::string domain_name_;									// domain��
	std::vector<PatternTemplate> templates_;					// List[ patternTemplate1, ...]
	std::map<std::string, PatternTemplate> map_synx_template_;	// synx		"^(���ҡ�)?(���롿)?��������$" ->  PatternTemplate
	std::map<std::string, pcrepp::Pcre*> map_synx_pcre_;		// synx		"^(���ҡ�)?(���롿)?��������$" ->  Pcre(synx)
	std::set<std::string> template_id_;							// set("ROUTE", "ROUTE_ARRIVAL",..)   <template id="ROUTE">  <template id="ROUTE_ARRIVAL">

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//	�Զ���	map_synx_rule_
	//	<pattern type = "EXACT" rule="��һ��@<date>ȥ@<loc>�Ļ�Ʊ">^��һ��.*ȥ.*�Ļ�Ʊ$< / pattern>
	std::map<std::string, std::string> map_synx_rule_;			// synx		"^��һ��ȥ.*�Ļ�Ʊ$"	-> "^��һ��ȥ@<loc>�Ļ�Ʊ$"
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
}; //end class PatternTemplates


DIALOG_SEG_END_NAMESPACE(qp);

#endif 

