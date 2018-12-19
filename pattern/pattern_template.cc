#include "pattern_template.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

using std::string;
using std::map;

DIALOG_SEG_LOG_SETUP(qp, PatternTemplate);

PatternTemplate::PatternTemplate() {}
PatternTemplate::~PatternTemplate() {}

void PatternTemplate::SetId(string id)
{
	id_ = id;
}

void PatternTemplate::SetDomain(string domain)
{
	domain_ = domain;
}

void PatternTemplate::AddSynxType(string synx, string type)
{
	map_synx_type_[synx] = type;
}

void PatternTemplate::AddSemantic(string key, string value)
{
	map_semantics_[key] = value;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//�Զ���
void PatternTemplate::AddSynxRule(string key, string value)
{
	map_synx_rule_[key] = value;
}

const string PatternTemplate::GetSynxRule(string synx)
{
	const string rule = map_synx_rule_[synx];
	return rule;
}

/*
	query	=	"��һ��2017��12��2���ޱ����Ļ�Ʊ"
	rule	=	"��һ��@<date>ȥ@<loc>�Ļ�Ʊ"
	prce	=	"^��һ��.*ȥ.*�Ļ�Ʊ$"
*/
bool PatternTemplate::GetSlotUseRule(std::string query, std::string rule, std::map<std::string, std::string> &slotKV)
{	
	//cout << "rule = " << rule << endl;
	//cout << "query = " << query << endl;

	std::vector<std::string> slots;
	std::vector<std::string> values;
	std::vector<std::string> otherWords;

	string ruleFirstPart, ruleSecondPart;
	//string queryFirstPart, querySecondPart;

	int count = 0;
	while (SplitOnFirst(rule, "@", ruleFirstPart, ruleSecondPart))
	{
		count++;
		//cout << count << "	ruleFirstPart = " << ruleFirstPart << ";	ruleSecondPart = " << ruleSecondPart << endl;
		//firstPart = "��һ��"
		//secondPart = "<date>ȥ@<loc>�Ļ�Ʊ"
		otherWords.push_back(ruleFirstPart);//"��һ��"	"ȥ"		
		int i = ruleSecondPart.find("<");
		int j = ruleSecondPart.find(">");
		if (i == -1 || j == -1)
		{
			//cout << "slot <> not find!" << i << j << endl;
			//cout << "ruleSecondPart = " << ruleSecondPart << endl;
			return false;
		}
		string slot = ruleSecondPart.substr(i + 1, j - i - 1);// date		loc
		slots.push_back(slot);
		//cout << "slot = " << slot << endl;
		rule = ruleSecondPart.substr(j + 1, ruleSecondPart.length() - j);//"ȥ@<loc>�Ļ�Ʊ"  "�Ļ�Ʊ"
		//cout << "rule = " << rule << endl;
	}
	//cout << "rulefirstPart = " << ruleFirstPart << endl;
	//cout << "##############################################" << endl;

	otherWords.push_back(ruleFirstPart);// "�Ļ�Ʊ"
	//otherWords : "��һ��"		"ȥ"		"�Ļ�Ʊ"
	//slots				   "date"   "loc"
	//for (int i = 0; i < otherWords.size(); i++)
	//{
	//	cout << "otherWords[" << i << "] = " << otherWords[i] << endl;
	//}

	if (otherWords.size() == 1)
	{
		return true;//û��@slot��Ϣ
	}
	//cout << "##############################################" << endl;
	for (int k = 1; k < otherWords.size(); k++)
	{
		//cout << "query = " << query << endl;
		//cout << "otherWords[" << k - 1 << "] = " << otherWords[k - 1] << "		otherWords[" << k << "] = " << otherWords[k] << endl;

		int i = query.find(otherWords[k - 1]);
		int j = query.find(otherWords[k]);
		if (i == -1 || j == -1)
		{
			//cout << "query not find otherword!\t" << otherWords[k - 1] << " : \t\t" << i << otherWords[k] << " : " << j << endl;
			return false;
		}
		i = i + otherWords[k - 1].length();
		string slotValue = query.substr(i, j - i);
		//cout << "i = " << i << "\tj = " << j << endl;
		//cout << "slot value:	" << slotValue << endl << endl;
		values.push_back(slotValue);
		//Ϊ�˷�ֹ�ظ����ֵ�otherWords[k-1],�Ѳ��õ�wordɾ����
		query = query.substr(i, query.length() - i);

	}
	if (values.size() != slots.size())
	{
		return false;
	}
	for (int i = 0; i < slots.size(); i++)
	{
		//slotKV.insert(map<string, string>::value_type(slots[i], values[i]));//insert�������ܲ�����ͬ��key
		slotKV[slots[i]] = values[i];//���Բ�����ͬkey,�Ḳ��ǰvalue
	}
	return true;
}


// A: splits the string in 2 parts, around and not including the first 
//    occurence of any of a set of specified characters. Returns true on success
//	���ַ������շָ��������ϡ��ֳ�������
bool PatternTemplate::SplitOnFirst(string sOriginal, char* pDividers, string& rsFirstPart, string& rsSecondPart)
{
	//cout << pDividers << endl;
	// find_first_of : �ǲ����ַ�����pDividers������һ���ַ����ֵ�λ��
	int iCharPos = sOriginal.find_first_of(pDividers);
	if (iCharPos == -1)
	{
		// û�ҵ��ָ���������ԭ��
		// if the character was not found
		rsFirstPart = sOriginal;
		rsSecondPart = "";
		return false;
	}
	else
	{
		// if the character was found
		// �ҵ��ָ���������һ�γ��ֵķָ����з��ַ�תΪ2����
		/*
		Parameters
			 pos	 -	 position of the first character to include
			count	 -	 length of the substring
		*/
		rsFirstPart = sOriginal.substr(0, iCharPos);
		rsSecondPart = sOriginal.substr(iCharPos + 1, sOriginal.length() - iCharPos);
		return true;
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

bool PatternTemplate::CheckDuplicate(std::string synx)
{
	if (map_synx_type_.find(synx) != map_synx_type_.end())
	{
		return true;
	}
	return false;
}
const string PatternTemplate::GetId() const
{
	return id_;
}

const string PatternTemplate::GetDomain() const
{
	return domain_;
}

const string PatternTemplate::GetSynxType(string synx)
{
	const string type = map_synx_type_[synx];
	return type;
}

const map<string, string> & PatternTemplate::GetMapSemantics() const
{
	return map_semantics_;
}
DIALOG_SEG_END_NAMESPACE(qp);

