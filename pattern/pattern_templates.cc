//author xhz105925
//date 2016/6/2

#include "pattern_templates.h"
#include <algorithm>
#include <exception>
#include "../mylib/tinyxml.h"
#include "../mylib/crfpp.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;
using std::vector;
using pcrepp::Pcre;

DIALOG_SEG_LOG_SETUP(qp, PatternTemplates);

PatternTemplates::PatternTemplates() {}

PatternTemplates::~PatternTemplates() {}

void PatternTemplates::SetDomainName(string name)
{
	domain_name_ = name;
}

string PatternTemplates::GetDomainName()
{
	return domain_name_;
}

const PatternTemplate PatternTemplates::GetTemplate(string synx)
{
	return map_synx_template_[synx];
}

const map<string, Pcre*>* PatternTemplates::GetMapSynxPcre() const
{
	return &map_synx_pcre_;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	自定义	map_synx_rule_
const map<string, string>* PatternTemplates::GetMapSynxRule() const
{
	return &map_synx_rule_;
}

// 根据rule生成synx
// rule => "订一张@<date>去@<loc>的机票"
// synx => "^订一张.*去.*的机票$"
std::string PatternTemplates::GenerateSynxFromRule(std::string rule)
{
	string synx = rule;
	int posAt = 0;
	int posArrow = 0;
	while (((posAt = synx.find("@", posAt)) != -1) && ((posArrow = synx.find(">")) != -1))
	{
		string sSlot = rule.substr(posAt, posArrow - posAt + 1);
		synx.replace(posAt, sSlot.length(), ".*");
	}
	synx = "^" + synx + "$";
	return synx;
}
int PatternTemplates::AddTemplatesByRule(const std::string domain, const std::string intent, const std::set<string> rules)
{
	string id = intent;
	transform(id.begin(), id.end(), id.begin(), toupper); //转换为大写: #include <algorithm>
	if (template_id_.find(id) != template_id_.end())
	{
		LOG(INFO, "There are duplicate template id: %s", id.c_str());
		//如果存在相同意图的template=>不应该重新创建	  [1-重新创建覆盖掉 2-修改已有的]
	}
	PatternTemplate tmplate;
	tmplate.SetId(id);			// <1> template::id
	template_id_.insert(id);	// <2> Templates::template_id
	tmplate.SetDomain(domain);	// <3> template::domain
	std::set<string> synx_set;
	for (std::set<string>::iterator iter_rule = rules.begin(); iter_rule != rules.end(); iter_rule++)
	{
		string rule = (*iter_rule);					//rule = "订一张@<date>去@<loc>的机票"
		string synx = GenerateSynxFromRule(rule);	// synx = "^订一张.*去.*的机票$"
		if (map_synx_rule_.find(synx) != map_synx_rule_.end())
		{
			LOG(INFO, "There are duplicate in map_synx_rule_ synx:%s", synx.c_str());
		}
		map_synx_rule_[synx] = rule;//会被覆盖掉！	<4>	Templates::map_synx_rule_

		Pcre* pcre = new Pcre(synx);
		if (map_synx_pcre_.find(synx) != map_synx_pcre_.end())
		{
			LOG(INFO, "There are duplicate in map_synx_pcre_ synx:%s", synx.c_str());
		}
		map_synx_pcre_[synx] = pcre;//会被覆盖掉！	<5>	Templates::map_synx_pcre_

		bool check_point = tmplate.CheckDuplicate(synx);
		if (check_point)
		{
			LOG(INFO, "duplicate synx !!!!!!!!!!! %s", synx.c_str());
			continue; //自定义的不同重复synx -> continue
		}
		tmplate.AddSynxType(string(synx), string("EXACT")); // <6> template::map_synx_type_
		tmplate.AddSynxRule(string(synx), string(rule));	// <7> template::map_synx_rule_
		synx_set.insert(synx);
	}
	tmplate.AddSemantic(string("intent"), string(intent));  // <8> template::map_semantics_
	tmplate.AddSemantic(string("domain"), string(domain));
	for (std::set<string>::iterator iter_synx = synx_set.begin(); iter_synx != synx_set.end(); iter_synx++)
	{
		string synx = (*iter_synx);
		if (map_synx_template_.find(synx) != map_synx_template_.end()) 
		{
			LOG(INFO, "There are duplicate map_synx_template_ : %s", id.c_str());
			//如果存在相同意图的template=>不应该重新创建	  [1-重新创建覆盖掉 2-修改已有的]
		}
		map_synx_template_[synx] = tmplate;//覆盖掉！！		// <9> Templates::map_synx_template_
	}
	//templates_.push_back(tmplate);//Bug:vector会越加越多

	return 0;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

int PatternTemplates::LoadTemplates(const char* xml_path)
{
	TiXmlDocument *pdoc = new TiXmlDocument();
	if (NULL == pdoc)
	{
		return -1;
	}
	bool loaded = pdoc->LoadFile(xml_path);
	if (loaded)
	{
		LOG(INFO, "Load template file %s %s", xml_path, "SUCCESS");
	}
	else
	{
		LOG(INFO, "Load template file %s %s", xml_path, "FAILED");
		if (pdoc)
		{
			delete pdoc;
		}
		return -1;
	}
	TiXmlHandle handler(pdoc);
	TiXmlHandle root = handler.FirstChild("templates");//<templates domainId="map">
	string domain(root.ToElement()->Attribute("domainId"));
	SetDomainName(domain);//<templates domainId="map">
	for (TiXmlNode* template_node = handler.FirstChild("templates").FirstChild("template").ToNode(); template_node != NULL; template_node = template_node->NextSibling())
	{
		/*
		<template id="ROUTE">
			<description>导航</description>
			<pattern type="EXACT">^(【我】)?(【想】)?【导航】$</pattern>
			<semantics>
				<domain>map</domain>
				<intent>route</intent>
			</semantics>
		</template>
		*/
		if (string(template_node->Value()).compare(string("template")))
		{
			continue;
		}
		PatternTemplate tmplate;
		const char* id = template_node->ToElement()->Attribute("id"); // <template id="ROUTE">
		if (id)
		{
			if (template_id_.find(id) != template_id_.end())
			{
				LOG(INFO, "There are duplicate template id: %s", id);
				continue;
			}
			tmplate.SetId(string(id));	//<template id="ROUTE">
			template_id_.insert(id);	//<template id="ROUTE">
		}
		else
		{
			LOG(INFO, "There is invalid template, id is NULL%s", "");
			continue;
		}
		for (TiXmlElement* child = template_node->FirstChild("pattern")->ToElement(); child != NULL; child = child->NextSiblingElement())
		{
			//<pattern type="EXACT">^(【我】)?(【想】)?【导航】$</pattern>
			tmplate.SetDomain(domain);					//<templates domainId="map">
			const char* type = child->Attribute("type");//<pattern type="EXACT"
			const char* synx = child->GetText();			// "^(【我】)?(【想】)?【导航】$"
			if (type && synx)
			{
				bool check_point = tmplate.CheckDuplicate(synx); //std::map<std::string, std::string> map_synx_type_; // <synx, type>
				if (check_point)
				{
					LOG(INFO, "duplicate synx !!!!!!!!!!! %s", synx);
					continue;
				}
				tmplate.AddSynxType(string(synx), string(type));//std::map<std::string, std::string> map_synx_type_; // <synx, type> => <"^(【我】)?(【想】)?【导航】$", EXACT>
				if (string(child->Value()).compare("pattern")) //这句执行不到！！ <semantics>
				{
					continue;
				}
			}
			else if (type != NULL && synx == NULL)
			{
				LOG(INFO, "There is invalid template synx:%s ", synx);
				continue;
			}
			else if (type == NULL && synx != NULL)
			{
				LOG(INFO, "There is invalid template type:%s ", type);
				continue;
			}
			else// type == NULL && synx == NULL
			{
				continue;
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			//自定义 rule    <synx, rule>	=>		<"^订一张.*去.*的机票$", "^订一张@<date>去@<loc>的机票$">
			const char* rule = child->Attribute("rule");
			if (rule)
			{
				tmplate.AddSynxRule(string(synx), string(rule));
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		}
		for (TiXmlNode* child = template_node->FirstChild("semantics"); child != NULL; child = child->NextSibling())
		{
			for (TiXmlElement* subchild = child->FirstChild()->ToElement(); subchild != NULL; subchild = subchild->NextSiblingElement())
			{
				/*
				 <semantics>
					<domain>map</domain>
					<intent>route</intent>
				</semantics>
				*/
				const char* semantic_elem = subchild->Value();	//domain | intent
				const char* text = subchild->GetText();			// map	 | route
				if (semantic_elem && text)
				{
					tmplate.AddSemantic(semantic_elem, string(text)); // <domain, map> | <intent, route> ????有问题？？？
				}
				else
				{
					LOG(INFO, "There is invalid template semantic, value or text is NULL%s", "");
					continue;
				}
			}
		}
		templates_.push_back(tmplate); //templates_
	}
	/*
	主要填充：
	std::map<std::string, PatternTemplate> map_synx_template_;	// synx		"^(【我】)?(【想】)?【导航】$" ->  PatternTemplate
	std::map<std::string, pcrepp::Pcre*> map_synx_pcre_;		// synx		"^(【我】)?(【想】)?【导航】$" ->  Pcre(synx)
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	std::map<std::string, std::string> map_synx_rule_;			// synx		"^订一张去.*的机票$"	-> "^订一张去@<loc>的机票$"
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	*/
	GenIndexMaps();
	if (pdoc)
	{
		delete pdoc;
	}
	return 0;
}

void PatternTemplates::Destroy()
{
	for (map<string, Pcre*>::iterator it = map_synx_pcre_.begin(); it != map_synx_pcre_.end(); it++)
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
	}
}

/*
主要填充：
std::map<std::string, PatternTemplate> map_synx_template_;	// synx		"^(【我】)?(【想】)?【导航】$" ->  PatternTemplate
std::map<std::string, pcrepp::Pcre*> map_synx_pcre_;		// synx		"^(【我】)?(【想】)?【导航】$" ->  Pcre(synx)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
std::map<std::string, std::string> map_synx_rule_;			// synx		"^订一张去.*的机票$"	-> "^订一张去@<loc>的机票$"
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
void PatternTemplates::GenIndexMaps()
{
	//templates_  //vector<PatternTemplate> 
	for (size_t i = 0; i < templates_.size(); i++)
	{
		//map_synx_type_; // <synx, type>  =>  < "^(【我】)?(【想】)?【导航】$", "EXACT">
		for (map<string, string>::iterator it = templates_[i].map_synx_type_.begin(); it != templates_[i].map_synx_type_.end(); it++)//// <synx, type>    =>  < "^(【我】)?(【想】)?【导航】$", "EXACT">
		{
			string synx = it->first; //synx   "^(【我】)?(【想】)?【导航】$"
			map_synx_template_[synx] = templates_[i]; // synx "^(【我】)?(【想】)?【导航】$" ->  PatternTemplate
			try
			{
				Pcre* pcre = new Pcre(synx); // synx   "^(【我】)?(【想】)?【导航】$"
				if (map_synx_pcre_.find(synx) != map_synx_pcre_.end())
				{
					LOG(INFO, "There are duplicate synx:%s", synx.c_str());
				}
				map_synx_pcre_[synx] = pcre;// synx   "^(【我】)?(【想】)?【导航】$" -> Pcre(synx)
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				//自定义 rule    <synx, rule>	=>		<"^订一张.*去.*的机票$", "^订一张@<date>去@<loc>的机票$">
				if (templates_[i].map_synx_rule_.find(synx) != templates_[i].map_synx_rule_.end())
				{
					if (map_synx_rule_.find(synx) != map_synx_rule_.end())
					{
						LOG(INFO, "There are duplicate synx-rule:%s", synx.c_str);
					}
					map_synx_rule_[synx] = templates_[i].map_synx_rule_[synx];//##复制到总模板Templates
				}
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			}
			catch (Pcre::exception &e)
			{
				LOG(INFO, "Exception for compiling synx:%s", synx.c_str());
				continue;
			}
		}
	}
}

DIALOG_SEG_END_NAMESPACE(qp);


