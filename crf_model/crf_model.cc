#include "crf_model.h"
#include "../reader/config_reader.h"
#include <iostream>

using std::string;
using std::vector;
using std::cout;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, CRFModel);

CRFModel::CRFModel()
{
	model_ = NULL;
}

CRFModel::~CRFModel()
{
	if (model_ != NULL)
	{
		delete model_;
		model_ = NULL;
	}
}

int CRFModel::Init(const string& cfg_path)
{
	/*
		crf_model : {
			MODEL_FILE = "./models/crf_0524.model" ;
		};
	*/
	ConfigReader reader(cfg_path);
	const string& model_path = reader.Get<string>("crf_model.MODEL_FILE");
	string params("");
	params.append("-m ");
	params.append(model_path);
	params.append(" -v3");
	// params =>  "-m ./models/crf_0524.model -v3"
	// ģ�Ͷ����ƣ�crf_0524.model
	model_ = CRFPP::createModel(params.c_str());//����ģ��
	if (model_ == NULL)
	{
		LOG(ERROR, "can not create crf model: %s", model_path.c_str());
		return -1;
	}
	return 0;
}
/*
ʶ���ʵ�壬��ӵ�qp_result.ne_term
*/
int CRFModel::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	vector<string> crf_input;//CRF����������Ϊ��λ��ȡfeature
	vector<int> term_index;
	/*
	message Term {
		required string text = 1;		//�д�
		required string pos = 2;		//����
		required uint32 begin = 3;		//�ʵĿ�ʼλ��,��ԭʼquery�е�. ���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ� 
		required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
		optional float weight = 5;		//termȨ�� [�д�ʱδ���]
	}
	*/
	/*
	������̨/˵/����/����/��/��ů/����
	��  0��  1�� 2�� 3�� 4,  5,  6��		<-   term_index
	*/
	for (int i = 0; i < qp_result->terms_size(); ++i)//����ÿ���дʣ�0��ʼ  
	{
		const Term& term = qp_result->terms(i);
		vector<string> characters;
		//characters �洢��ǰ�дʵ�ÿ���֡�utf-8���롿
		Split2chs(term.text(), characters);

		for (size_t j = 0; j < characters.size(); j++)//����ÿ����  -> ����Ϊ������ȡCRF Feature
		{
			//ֻ��һ���ֵ��д�
			if (characters.size() == 1)
			{
				string crf_item = characters[j] + "\t" + term.pos(); // "��\t����"
				crf_input.push_back(crf_item);			  			
				term_index.push_back(i);//��ʹ�õĴ��ǵڼ�����
				break;
			}
			//���ǴʵĿ�ʼ
			if (j == 0)
			{
				string crf_item = characters[j] + "\t" + term.pos() + "_B"; // "��\t���ݴ�_B"
				crf_input.push_back(crf_item);
				term_index.push_back(i);
				continue;
			}
			//���ǴʵĽ���
			if (j == characters.size() - 1)
			{
				string crf_item = characters[j] + "\t" + term.pos() + "_E"; // "��\t����_E"
				crf_input.push_back(crf_item);
				term_index.push_back(i);
				continue;
			}
			//�����м�
			string crf_item = characters[j] + "\t" + term.pos() + "_M";		//"��\t����_M"
			crf_input.push_back(crf_item);
			term_index.push_back(i);
		}//for(size_t j = 0; j < characters.size(); j++)//����ÿ����
	}//for (int i = 0; i < qp_result->terms_size(); ++i) ����ÿ���д�

	vector<string> tags;
	//����ά�ر��㷨����ʶʵ���,����洢��tags��
	int ret = Parse(crf_input, tags);//CRF����������Ϊ��λ��ȡfeature
	if (ret != 0)
	{
		LOG(ERROR, "crf parse fail");
		return -1;
	}
	//����ʵ���ע�Ľ����PEO:����, POI:λ��, LOC:�ص㡿
	// PEO_S : ��ʼ
	// PEO_M : �м�
	// PEO_E : ��β
	/*
	message NeTerm {
		required string ne_type = 1;
		required string ne_text = 2;
		required uint32 begin = 3;
		required uint32 end = 4;
	} 
	*/
	for (size_t i = 0; i < tags.size(); ++i)//������ǩ
	{
		//����
		if (tags[i].compare("PEO_S") == 0)
		{
			NeTerm* ne_term = qp_result->add_ne_terms();//ʵ��ʵ� proto
			size_t j = i + 1;
			//�ҵ���β�� ��i,j)ǰ�պ�
			while ((j < tags.size()) &&	((tags[j].compare("PEO_M") == 0) || (tags[j].compare("PEO_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			/*
				������ȡ�ǰ��֣�term_index�洢��ǰ�ֵ����ڴʵ���������(�дʺ�ĵڼ�����)
			*/
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)// ʲô��˼?? ����
				{
					continue;
				}
				pre_index = term_index[k];//��ǰ���������ڴʵ���������  
				ne_term_text += qp_result->terms(term_index[k]).text();//�ѵ�ǰ���֡��������ڵġ��ʡ����� ������ʵ�塯  -�� ��������������дʺ�ʵ�岻�غϣ�ʵ��ʶ��Ľ���������дʵ��м䲿�� A[B/CD]E��
			}
			ne_term->set_ne_text(ne_term_text); //ʵ��� ��CRF�п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
			ne_term->set_ne_type("per");	//ʵ�����
			ne_term->set_begin(qp_result->terms(term_index[i]).begin()); //ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf - 8 ����3���ֽ�]Ϊ��λ������ҿ�
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end()); //�ʵĽ���λ��,��ԭʼquery�е�.
			i = j - 1;// ?? i=j
			continue;
		}	//����
		if (tags[i].compare("POI_S") == 0)//POI����
		{
			NeTerm* ne_term = qp_result->add_ne_terms();
			size_t j = i + 1;
			while ((j < tags.size()) &&
				((tags[j].compare("POI_M") == 0) || (tags[j].compare("POI_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)
				{
					continue;
				}
				pre_index = term_index[k];
				ne_term_text += qp_result->terms(term_index[k]).text();
			}
			ne_term->set_ne_text(ne_term_text);
			ne_term->set_ne_type("poi");
			ne_term->set_begin(qp_result->terms(term_index[i]).begin());
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end());
			i = j - 1;
			continue;
		}//POI����
		if (tags[i].compare("LOC_S") == 0)//LOC����
		{
			NeTerm* ne_term = qp_result->add_ne_terms();
			size_t j = i + 1;
			while ((j < tags.size()) &&
				((tags[j].compare("LOC_M") == 0) || (tags[j].compare("LOC_E") == 0)))
			{
				j++;
			}
			int pre_index = -1;
			string ne_term_text("");
			for (size_t k = i; k < j; k++)
			{
				if (term_index[k] == pre_index)
				{
					continue;
				}
				pre_index = term_index[k];
				ne_term_text += qp_result->terms(term_index[k]).text();
			}
			ne_term->set_ne_text(ne_term_text);
			ne_term->set_ne_type("loc");
			ne_term->set_begin(qp_result->terms(term_index[i]).begin());
			ne_term->set_end(qp_result->terms(term_index[j - 1]).end());
			i = j - 1;
			continue;
		}//LOC����
	}
	return 0;
}

//Split2chs(term.text(), characters);
void CRFModel::Split2chs(const string& word, vector<string>& characters)
{
	string character;
	//utf-8�����ʽ�Ǳ䳤���룬ͨ��������ķ�Χȷ������ĳ��ȡ�[1~6]
	for (size_t i = 0, len = 0; i != word.length(); i += len)
	{
		unsigned char byte = (unsigned)word[i];
		if (byte >= 0xFC)
			len = 6;
		else if (byte >= 0xF8)
			len = 5;
		else if (byte >= 0xF0)
			len = 4;
		else if (byte >= 0xE0)
			len = 3;
		else if (byte >= 0xC0)
			len = 2;
		else
			len = 1;
		character = word.substr(i, len);//�г���ǰ��
		characters.push_back(character);
	}
}
/*
vector<string> tags;
int ret = Parse(crf_input, tags);
*/

//����ά�ر��㷨����ʶʵ���
int CRFModel::Parse(const vector<string>& sequence, vector<string>& tags)
{
	//<1>	��ȡ���ǩ��Tagger
	CRFPP::Tagger* tagger = model_->createTagger();
	if (tagger == NULL)
	{
		LOG(ERROR, "create crf tagger fail");
		return -1;
	}
	tags.clear();
	tagger->clear();
	//<2>	����������� ������Ϊ��λ��������
	for (size_t i = 0; i < sequence.size(); i++)
	{
		tagger->add(sequence[i].c_str());
	}
	//<3>	�������� - ά�ر�
	if (!tagger->parse())
	{
		LOG(ERROR, "parse error");
		return -1;
	}
	//<4>	���ÿ���ʵ����� [y2�����ʽ ��]
	for (size_t i = 0; i < tagger->size(); i++)
	{
		tags.push_back(tagger->y2(i));//��Ӵ����ǩ���ÿ���ֵı�ǩ���
	}
	//<5>	������ǩ��Tagger
	if (tagger != NULL)
	{
		delete tagger;
		tagger = NULL;
	}
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
