//author xhz105925
//date 2016/6/13
#ifndef DM_QP_PATTERN_REQUEST_H_
#define DM_QP_PATTERN_REQUEST_H_
#include <string>
#include "../common_macros.h"
#include "../log.h"
#include "../proto/qp.pb.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
class QueryTerm;
class QueryNeTerm;
class QueryDomainQpResult;
class PatternRequest
{
public:
	PatternRequest();
	~PatternRequest();
	void InputInfo(QpResult& qp_result);
	const std::string GetRawQuery() const;
	const std::string GetCorrectedQuery() const;
	const std::vector<QueryTerm>* GetQueryTerm() const;
	const std::vector<QueryNeTerm>* GetQueryNeTerm() const;
	const std::vector<QueryDomainQpResult>* GetDomainResult() const;

	void SetRawQuery(std::string query);
	void SetCorrectQuery(std::string query);
	void AddTerm(QueryTerm term);
	void AddNeTerm(QueryNeTerm ne_term);
	void AddDomainResult(QueryDomainQpResult domain_result);

private:
	/*
	message QpResult {
		required string raw_query = 1;					//ԭʼquery = QpRequest.query
		optional string corrected_query = 2;			//PostProcesser������󣬸��ֶλ�û����䣡��
		repeated Term terms = 3;						//���������д� ��WordSegmenter����ɡ�
		repeated NeTerm ne_terms = 4;					//ʵ��CRF[per, poi, loc]   Dic[singer, song, app, website]
		repeated DomainQpResult domain_qp_results = 5;	//MaxEnt������
}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string raw_query_;								//ԭʼquery = qp_result.raw_query
	std::string corrected_query_;						//������ֵ
	std::vector<QueryTerm> terms_;						//�д�Term �б�
	std::vector<QueryNeTerm> ne_terms_;					//ʵ���NeTerm�б�
	std::vector<QueryDomainQpResult> domain_results_;	//������DomainQpResult �б�
};

class QueryTerm
{
public:
	QueryTerm();
	~QueryTerm();
	const std::string GetText() const;
	const int GetBegin() const;
	const int GetEnd() const;
	const float GetWeight() const;
	void SetText(std::string text);
	void SetBegin(int begin);
	void SetEnd(int end);
	void SetWeight(float weight);
private:
	/*
	message Term {
		required string text = 1;		//�д�
		required string pos = 2;		//����?
		required uint32 begin = 3;      //�ʵĿ�ʼλ��,��ԭʼquery�е�. ���ֽ�[utf-8 ����3���ֽ�]Ϊ��λ������ҿ�
		required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
		optional float weight = 5;      //termȨ�� [�д�ʱδ���]
	}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string text_;  //�޴��ԣ�pos
	int begin_;
	int end_;
	float weight_;
};

class QueryNeTerm
{
public:
	QueryNeTerm();
	~QueryNeTerm();
	const std::string GetNeType() const;
	const std::string GetNeText() const;
	const int GetBegin() const;
	const int GetEnd() const;
	void SetNeType(std::string type);
	void SetNeText(std::string text);
	void SetBegin(int begin);
	void SetEnd(int end);
private:
	/*
	message NeTerm{
		required string ne_type = 1;	//ʵ�����
		required string ne_text = 2;	//ʵ��� ���п��ܱ�ʶ�������ʵ��� ���󡯣� ƴ��ʵ����ǰ��մʼ���ƴ�ӵģ����ǰ����֡�
		required uint32 begin = 3;		//ʵ��ʵĿ�ʼλ��, ��ԭʼquery�е�.���ֽ�[utf - 8 ����3���ֽ�]Ϊ��λ������ҿ�
		required uint32 end = 4;		//�ʵĽ���λ��,��ԭʼquery�е�.
	}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string ne_type_;
	std::string ne_text_;
	int begin_;
	int end_;
};

class QueryDomainQpResult
{
public:
	QueryDomainQpResult();
	~QueryDomainQpResult();
	const std::string GetDomain() const;
	const float GetDomainScore() const;
	const std::vector<QueryNeTerm>* GetNeTerms() const;
	const std::string GetNeSentence() const;
	void SetDomain(std::string domain);
	void SetDomainScore(float score);
	void SetNeSentence(std::string ne_sentence);
	void AddNeTerm(QueryNeTerm ne_term);
private:
	/*
	message DomainQpResult {
		required string domain = 1;			//���� [ music]  ==> "�������»�/[per singer]������ˮ/song"
		optional float domain_score = 2;	//MaxEnt�÷�
		repeated NeTerm ne_terms = 3;		//MaxEntδ��� [��������� : ���»�/singer   ����ˮ/song]
		required string ne_sentense = 4;	//MaxEntδ��� [��������� : "������/o���»�/singer��/o����ˮ/song(/o)"]
	}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string domain_;
	float domain_score_;
	std::vector<QueryNeTerm> ne_terms_;
	std::string ne_sentence_;
};


DIALOG_SEG_END_NAMESPACE(qp);

#endif
