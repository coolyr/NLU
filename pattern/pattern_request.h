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
		required string raw_query = 1;					//原始query = QpRequest.query
		optional string corrected_query = 2;			//PostProcesser处理完后，该字段还没有填充！？
		repeated Term terms = 3;						//检索粒度切词 【WordSegmenter类完成】
		repeated NeTerm ne_terms = 4;					//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
		repeated DomainQpResult domain_qp_results = 5;	//MaxEnt分类结果
}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string raw_query_;								//原始query = qp_result.raw_query
	std::string corrected_query_;						//？？无值
	std::vector<QueryTerm> terms_;						//切词Term 列表
	std::vector<QueryNeTerm> ne_terms_;					//实体词NeTerm列表
	std::vector<QueryDomainQpResult> domain_results_;	//分类结果DomainQpResult 列表
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
		required string text = 1;		//切词
		required string pos = 2;		//词性?
		required uint32 begin = 3;      //词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开
		required uint32 end = 4;		//词的结束位置,在原始query中的.
		optional float weight = 5;      //term权重 [切词时未填充]
	}
	*/
	DIALOG_SEG_LOG_DECLARE();
	std::string text_;  //无词性？pos
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
		required string ne_type = 1;	//实体类别
		required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
		required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
		required uint32 end = 4;		//词的结束位置,在原始query中的.
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
		required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
		optional float domain_score = 2;	//MaxEnt得分
		repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
		required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充 : "我想听/o刘德华/singer的/o忘情水/song(/o)"]
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
