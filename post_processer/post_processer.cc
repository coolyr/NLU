#include "post_processer.h"

#include <iostream>

using std::string;
using std::map;

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, PostProcesser);

PostProcesser::~PostProcesser()
{
	map<string, PostProcesserBase*>::iterator it = type2post_processer_.begin();
	for (; it != type2post_processer_.end(); ++it)
	{
		delete it->second;
	}
	type2post_processer_.clear();
}

int PostProcesser::Init(const string& cfg_path)
{
	MapPostProcesser* map_post_processer = new MapPostProcesser();//map������
	if (map_post_processer->Init(cfg_path) != 0) //��
	{
		return -1;
	}
	if (RegisteProcesser(map_post_processer) == -1)
	{
		return -1;
	}

	MusicPostProcesser* music_post_processer = new MusicPostProcesser();//music������
	if (music_post_processer->Init(cfg_path) != 0)//����LanguageModel
	{
		return -1;
	}
	if (RegisteProcesser(music_post_processer) == -1)
	{
		return -1;
	}

	TelephonePostProcesser* telephone_post_processer = new TelephonePostProcesser();//telepthon������
	if (telephone_post_processer->Init(cfg_path) != 0)//��
	{
		return -1;
	}
	if (RegisteProcesser(telephone_post_processer) == -1)
	{
		return -1;
	}

	return 0;
}

int PostProcesser::Uinit()
{
	return 0;
}
/*
message DomainQpResult {
	required string domain = 1;			//���� [map, calendar, music, telephone�� other]
	optional float domain_score = 2;	//MaxEnt�Ե�ǰ���÷�
	repeated NeTerm ne_terms = 3;		//MaxEntδ���
	required string ne_sentense = 4;	//MaxEntδ���
}
*/
int PostProcesser::Process(const QpRequest& qp_request, QpResult* qp_result)
{
	for (int i = 0; i < qp_result->domain_qp_results_size(); ++i)//��������ط��� ��Ŀǰֻ��һ�����
	{
		const string& domain = qp_result->domain_qp_results(i).domain();// [map, music, calendar, telepthon, other]
		map<string, PostProcesserBase*>::const_iterator it = type2post_processer_.find(domain);//��map, music, calendar�� (calendar��postProcesser??)
		if (it != type2post_processer_.end())
		{
			PostProcesserBase* post_processer = it->second;
			post_processer->PostProcess(qp_request, qp_result);//���������� ���õ���ʵ�壬��ð���Ӧ��DomainQpResult���󴫽�ȥ����Ϊ�����ֱ�����һ������ط��ࡿ
		}
	}//��������ط��� ��Ŀǰֻ��һ�����
	return 0;
}

int PostProcesser::RegisteProcesser(PostProcesserBase* post_processer)
{
	string domain = post_processer->GetDomain();
	if (type2post_processer_.find(domain) != type2post_processer_.end())
	{
		return -1;
	}
	type2post_processer_.insert(std::make_pair(domain, post_processer));
	return 0;
}

DIALOG_SEG_END_NAMESPACE(qp);
