//author xhz105925
//date 2016/6/13

#include "pattern_request.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
using std::string;
using std::vector;

DIALOG_SEG_LOG_SETUP(qp, PatternRequest);

PatternRequest::PatternRequest() {}
PatternRequest::~PatternRequest() {}

/*
message QpResult {
	required string raw_query = 1;					//原始query = QpRequest.query
	optional string corrected_query = 2;			//PostProcesser处理完后，该字段还没有填充！？
	repeated Term terms = 3;						//检索粒度切词 【WordSegmenter类完成】
	repeated NeTerm ne_terms = 4;					//实体CRF[per, poi, loc]   Dic[singer, song, app, website]
	repeated DomainQpResult domain_qp_results = 5;	//MaxEnt分类结果
}
message DomainQpResult {
	required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
	optional float domain_score = 2;	//MaxEnt得分
	repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
	required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充 :"我想听/o刘德华/singer的/o忘情水/song(/o)"]
}
*/
void PatternRequest::InputInfo(QpResult& qp_result)
{
	if (qp_result.has_raw_query())//raw_query
	{
		SetRawQuery(qp_result.raw_query());
	}
	if (qp_result.has_corrected_query())//corrected_query
	{
		SetCorrectQuery(qp_result.corrected_query());
	}
	for (int i = 0; i < qp_result.terms_size(); i++)//切词
	{
		/*
		message Term {
			required string text = 1;		//切词
			required string pos = 2;		//词性
			required uint32 begin = 3;      //词的开始位置,在原始query中的. 以字节[utf-8 汉字3个字节]为单位，左闭右开 
			required uint32 end = 4;		//词的结束位置,在原始query中的.
			optional float weight = 5;      //term权重 [切词时未填充]
			}
		*/
		Term term = qp_result.terms(i);
		QueryTerm qterm;//无词性POS？
		qterm.SetText(term.text());
		qterm.SetBegin(term.begin());
		qterm.SetEnd(term.end());
		qterm.SetWeight(term.weight());
		AddTerm(qterm);
	}
	for (int i = 0; i < qp_result.ne_terms_size(); i++)//实体词
	{
		/*
		message NeTerm {
			required string ne_type = 1;	//实体类别
			required string ne_text = 2;	//实体词 【有可能比识别出来的实体词 ‘大’， 拼接实体词是按照词级别拼接的，不是按照字】
			required uint32 begin = 3;		//实体词的开始位置, 在原始query中的.以字节[utf - 8 汉字3个字节]为单位，左闭右开
			required uint32 end = 4;		//词的结束位置,在原始query中的.
		} 
		*/
		NeTerm neterm = qp_result.ne_terms(i);
		QueryNeTerm qne_term;
		qne_term.SetNeType(neterm.ne_type());
		qne_term.SetNeText(neterm.ne_text());
		qne_term.SetBegin(neterm.begin());
		qne_term.SetEnd(neterm.end());
		AddNeTerm(qne_term);
	}
	for (int i = 0; i < qp_result.domain_qp_results_size(); i++)//遍历分类结果【目前只有一个】
	{
		/*
		message DomainQpResult {
			required string domain = 1;			//域名 [ music]  ==> "我想刘德华/[per singer]的忘情水/song"
			optional float domain_score = 2;	//MaxEnt得分
			repeated NeTerm ne_terms = 3;		//MaxEnt未填充 [后处理器填充 : 刘德华/singer   忘情水/song]
			required string ne_sentense = 4;	//MaxEnt未填充 [后处理器填充 :"我想听/o刘德华/singer的/o忘情水/song(/o)"]
		}
		*/
		DomainQpResult dqr = qp_result.domain_qp_results(i);
		QueryDomainQpResult qdres;
		qdres.SetDomain(dqr.domain());
		qdres.SetDomainScore(dqr.domain_score());
		qdres.SetNeSentence(dqr.ne_sentense());
		QueryNeTerm qne_term;
		for (int j = 0; j < dqr.ne_terms_size(); j++)
		{
			qne_term.SetNeType(dqr.ne_terms(j).ne_type());
			qne_term.SetNeText(dqr.ne_terms(j).ne_text());
			qne_term.SetBegin(dqr.ne_terms(j).begin());
			qne_term.SetEnd(dqr.ne_terms(j).end());
			qdres.AddNeTerm(qne_term);
		}

		AddDomainResult(qdres);//【目前只有一个】
	}//遍历分类结果【目前只有一个】
}

const string PatternRequest::GetRawQuery() const
{
	return raw_query_;
}
const string PatternRequest::GetCorrectedQuery() const
{
	return corrected_query_;
}
const vector<QueryTerm>* PatternRequest::GetQueryTerm() const
{
	return &terms_;
}
const vector<QueryNeTerm>* PatternRequest::GetQueryNeTerm() const
{
	return &ne_terms_;
}
const vector<QueryDomainQpResult>* PatternRequest::GetDomainResult() const
{
	return &domain_results_;
}
void PatternRequest::SetRawQuery(string query)
{
	raw_query_ = query;
}
void PatternRequest::SetCorrectQuery(string query)
{
	corrected_query_ = query;
}
void PatternRequest::AddTerm(QueryTerm term)
{
	terms_.push_back(term);
}
void PatternRequest::AddNeTerm(QueryNeTerm ne_term)
{
	ne_terms_.push_back(ne_term);
}
void PatternRequest::AddDomainResult(QueryDomainQpResult domain_result)
{
	domain_results_.push_back(domain_result);
}


QueryTerm::QueryTerm() {}
QueryTerm::~QueryTerm() {}
const string QueryTerm::GetText() const
{
	return text_;
}
const int QueryTerm::GetBegin() const
{
	return begin_;
}
const int QueryTerm::GetEnd() const
{
	return end_;
}
const float QueryTerm::GetWeight() const
{
	return weight_;
}
void QueryTerm::SetText(string text)
{
	text_ = text;
}
void QueryTerm::SetBegin(int begin)
{
	begin_ = begin;
}
void QueryTerm::SetEnd(int end)
{
	end_ = end;
}
void QueryTerm::SetWeight(float weight)
{
	weight_ = weight;
}

QueryNeTerm::QueryNeTerm() {}
QueryNeTerm::~QueryNeTerm() {}
const string QueryNeTerm::GetNeType() const
{
	return ne_type_;
}
const string QueryNeTerm::GetNeText() const
{
	return ne_text_;
}
const int QueryNeTerm::GetBegin() const
{
	return begin_;
}
const int QueryNeTerm::GetEnd() const
{
	return end_;
}
void QueryNeTerm::SetNeType(string type)
{
	ne_type_ = type;
}
void QueryNeTerm::SetNeText(string text)
{
	ne_text_ = text;
}
void QueryNeTerm::SetBegin(int begin)
{
	begin_ = begin;
}
void QueryNeTerm::SetEnd(int end)
{
	end_ = end;
}

QueryDomainQpResult::QueryDomainQpResult() {}
QueryDomainQpResult::~QueryDomainQpResult() {}
const string QueryDomainQpResult::GetDomain() const
{
	return domain_;
}
const float QueryDomainQpResult::GetDomainScore() const
{
	return domain_score_;
}
const vector<QueryNeTerm>* QueryDomainQpResult::GetNeTerms() const
{
	return &ne_terms_;
}
const string QueryDomainQpResult::GetNeSentence() const
{
	return ne_sentence_;
}
void QueryDomainQpResult::SetNeSentence(string ne_sentence)
{
	ne_sentence_ = ne_sentence;
}
void QueryDomainQpResult::SetDomain(string domain)
{
	domain_ = domain;
}
void QueryDomainQpResult::SetDomainScore(float score)
{
	domain_score_ = score;
}
void QueryDomainQpResult::AddNeTerm(QueryNeTerm ne_term)
{
	ne_terms_.push_back(ne_term);
}
DIALOG_SEG_END_NAMESPACE(qp);
