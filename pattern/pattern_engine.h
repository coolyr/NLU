//author xhz105925
//date 2016/5/31

#ifndef DM_QP_PATTERNENGINE_H_
#define DM_QP_PATTERNENGINE_H_

#include <string>
#include <vector>
#include <set>
#include <map>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"
#include "../proto/nlu_dm.pb.h"
#include "../mylib/pcre++.h"
#include "pattern_slots.h"
#include "pattern_templates.h"
#include "pattern_result.h"
#include "pattern_request.h"
#include "pattern_domain.h"
#include "domain_map.h"
#include "domain_music.h"
#include "domain_call.h"
DIALOG_SEG_BEGIN_NAMESPACE(qp);

class PatternEngine
{
public:
	PatternEngine();
	~PatternEngine();
	bool Matched(const std::string expr, const std::string stuff);
	std::vector<std::string> GetSubstr(const std::string expr, const std::string stuff);
	std::vector<std::pair<size_t, size_t> > GetSubstrPos(const std::string expr, const std::string stuff);
	int Init(const char* conf_path);
	int Process(QpResult* qp_result, NluResult& nlu_result, bool is_exact);
	void AddSupportDomain(std::string domain);
	void AddRecommendDomain(std::string domain);

	PatternSlots* GetSlotsByDomain(std::string domain);
	void ClearRecommendDomain();
	void ClearResults();
	void GenMapDomainTemplates(std::string domain, PatternTemplates* patt_tmp);
	int Configure(const char* conf_path);
	void Destroy();
	void InputInfo(QpResult& qp_result);
	void ReConcatByNer(PatternRequest& request, std::vector<PatternResult>& results);
	void NormallizeBySlot(PatternResult& result, std::string& synx_token);
	void MatchTemplate(PatternResult& result, std::string& synx_token);
	void FormateNluResult(std::vector<PatternResult>& patt_results, NluResult& nlu_result);
	int DirectMatch(std::string synx, std::vector<PatternResult>& results);
	int TemplatePriority(PatternResult& result, const std::string type, const size_t weight);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	int ModifyTemplatesByRule(const std::string domain, const std::string intent, const std::set<std::string> rules);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


private:
	DIALOG_SEG_LOG_DECLARE();
	std::map<std::string, PatternSlots*> domains_slots_;						// <4>  map -> PatternSlots
	std::set<std::string> support_domains_;										// <1>  [map,music,telepthon,(calendar沒有)]
	std::vector<std::string> recommend_domains_;								// ？	无用
	std::map<std::string, std::set<PatternTemplates*> > map_domain_templates_;	// <5>  <map  -> set<PatternTemplates*>> 一个领域可以有多套模板
	std::map<std::string, std::string> xml_paths_;								// <3>  map -> 模板路径
	std::vector<PatternResult> results_;										// <6>  处理结果对象PatternResult
	std::map<std::string, PatternDomain*> domain_ptr_;							// <2>  map -> DomainMap()
}; //end class PatternTemplates		


DIALOG_SEG_END_NAMESPACE(qp);

#endif //END DM_QP_PATTERN_ENGINE_H_

