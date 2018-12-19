//author xhz105925
//date 2016/5/31

#include "pattern_slots.h"
#include <exception>
#include "../mylib/tinyxml.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, PatternSlots);

using std::string;
using std::vector;
using std::map;
using pcrepp::Pcre;

PatternSlots::PatternSlots() {}

PatternSlots::~PatternSlots() {}

void PatternSlots::SetDomainName(string name)
{
	domain_ = name;
}

int PatternSlots::LoadSlot(const char * xml_path)
{
	TiXmlDocument *pdoc = new TiXmlDocument();
	if (NULL == pdoc)
	{
		return -1;
	}

	bool loaded = pdoc->LoadFile(xml_path);
	if (loaded)
	{
		LOG(INFO, "Load slot file %s %s", xml_path, "SUCCESS");
	}
	else
	{
		LOG(INFO, "Load slot file %s %s", xml_path, "FAILED");
		if (pdoc)
		{
			delete pdoc;
		}
		return -1;
	}
	TiXmlHandle handler(pdoc);
	TiXmlHandle root = handler.FirstChild("templates");
	string domain(root.ToElement()->Attribute("domainId"));// <templates domainId="map">
	for (TiXmlElement* child = root.FirstChild("slots").FirstChild("slot").ToElement(); child != NULL; child = child->NextSiblingElement())//<slot name="【往】" weight="high" synx="(前往|往)"/>
	{
		//<slot name = "【往】" weight="high" synx="(前往|往)"/>
		//<slot name = "【去】" weight = "high" synx = "(去|到|至|走|回|直达|前往|go|出发|行进|前行|抵达)" / >
		PatternSlot slot;
		slot.SetDomain(domain);// <templates domainId="map">
		const char* name = child->Attribute("name");
		if (name)
		{
			slot.SetName(string(name));
		}
		else
		{
			LOG(INFO, "There is invalid slot, name attribution is lost: %s", "");
			continue;
		}

		const char* weight = child->Attribute("weight");
		if (weight)
		{
			slot.SetWeight(string(weight));
		}
		else
		{
			slot.SetWeight(string("low"));
		}

		const char* synx = child->Attribute("synx");
		if (synx)
		{
			if (synx[0] == '?' && strlen(synx) > 1) //? -> prefix
			{
				synx++;
				slot.SetSynxPrefix("?");// synx_prefix = "?"
			}
			else if (synx[0] == '?' && strlen(synx) == 1)
			{
				LOG(INFO, "slot name: %s, synx:NULL", child->Attribute("name"));
				continue;
			}
			slot.SetSynx(string(synx));
		}
		else
		{
			slot.SetSynx(string(""));
			LOG(INFO, "slot name: %s, synx:NULL", child->Attribute("name"));
			continue;
		}

		synx2slot_[synx] = slot;	// map<std::string, PatternSlot>
		try
		{
			Pcre* pcre = new Pcre(synx);
			synx2pcre_[synx] = pcre;// map<std::string, pcrepp::Pcre*>
		}
		catch (Pcre::exception &e)
		{
			LOG(INFO, "Exception for compiling synx:%s", synx);
			continue;
		}
	}

	if (pdoc)
	{
		delete pdoc;
	}
	return 0;
}

void PatternSlots::Destroy()
{
	for (map<std::string, Pcre*>::iterator it = synx2pcre_.begin(); it != synx2pcre_.end(); it++)
	{
		if (it->second != NULL)
		{
			delete(it->second);
		}
	}
}

const map<string, Pcre*>* PatternSlots::GetSynxPcre() const
{
	return &synx2pcre_;
}

const map<string, PatternSlot>* PatternSlots::GetSynx2Slot() const
{
	return &synx2slot_;
}

DIALOG_SEG_END_NAMESPACE(qp);
