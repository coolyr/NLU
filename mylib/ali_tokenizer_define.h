#ifndef ALI_TOKENIZER_DEFINE_H
#define ALI_TOKENIZER_DEFINE_H

#include <stdint.h>

namespace ws
{

typedef uint32_t PropertyType;

#ifndef PROPERTY_TYPE_CONSTANT
#define PROPERTY_TYPE_CONSTANT
static const PropertyType WS_PROPERTY_DEF = 0x00;      ///< default property
static const PropertyType WS_PROPERTY_NUM = 0x04;      ///< number, not including chinese number
static const PropertyType WS_PROPERTY_PER = 0x10;      ///< person name
static const PropertyType WS_PROPERTY_LET = 0x20;      ///< letters
static const PropertyType WS_PROPERTY_LETNUM = 0x40;   ///< letters and numbers
static const PropertyType WS_PROPERTY_PLA = 0x80;      ///< place name
static const PropertyType WS_PROPERTY_DATE = 0x0100;   ///< date 
static const PropertyType WS_PROPERTY_SOFTVERSION = 0x0200;   ///< soft version
static const PropertyType WS_PROPERTY_ORG_RULE = 0x0400;      ///< orgnaziton name
static const PropertyType WS_PROPERTY_CHINESE_NUM = 0x0800;   ///< chinese number
static const PropertyType WS_PROPERTY_TIME = 0x1000;          ///< time
static const PropertyType WS_PROPERTY_TEL = 0x2000;           ///< telephone number
static const PropertyType WS_PROPERTY_EMAIL = 0x4000;         ///< email address
static const PropertyType WS_PROPERTY_URL = 0x8000;           ///< URL 
static const PropertyType WS_PROPERTY_INTERPUNCTION = 0x010000;  ///< interpunction
static const PropertyType WS_PROPERTY_WHITE_SPACE = 0x020000;    ///< white space
static const PropertyType WS_PROPERTY_HIRAGANA = 0x040000;       ///< ping jia ming
static const PropertyType WS_PROPERTY_KATAKANA = 0x080000;       ///< pian jia ming
static const PropertyType WS_PROPERTY_ORDINAL_NUM = 0x100000;    ///< ordinal number
static const PropertyType WS_PROPERTY_PER_CHN = 0x200000;		///< chinese person name
static const PropertyType WS_PROPERTY_PER_ENG = 0x400000;		///< europe/american person name
static const PropertyType WS_PROPERTY_PER_JPN = 0x800000;		///< japanese person name
static const PropertyType WS_PROPERTY_IP = 0x1000000;			///< ip address

/*
 *Deprecated constant
 */
static const PropertyType WS_PROPERTY_PER_RULE = 0x08; ///< person name //Deprecated
static const PropertyType WS_PROPERTY_PER_ME = 0x10;   ///< person name //Deprecated
static const uint8_t WS_TAGID_DEF = 0x0; //Deprecated

#endif
/**
 * text encoding supported
 */
enum EnumEncodingSupported {UTF16 = 1, UTF8 = 2, GBK = 3};

/**
 * enum for list type 
 */
enum EnumListType {MAIN_LIST = 1, ACCESSORY_LIST = 2};

enum EnumIteratorType { SEMANTIC_ITERATOR = 1 , RETRIEVE_ITERATOR = 2 };

/*
 * Type of SegToken
 */
typedef uint32_t SegTokenType;
/*
 * constant definition of SegToken type
 * 
 */
static const SegTokenType SEG_TOKEN_SEMANTIC = 0x00010000; //语义单元
static const SegTokenType SEG_TOKEN_RETRIEVE = 0x00020000; //检索单元
static const SegTokenType SEG_TOKEN_PHRASE   = 0x00040000; //短语(预留给句法分析使用)

static const SegTokenType SEG_TOKEN_HEADWORD = 0x00000100; //中心词成分
static const SegTokenType SEG_TOKEN_MODIFIER = 0x00000200; //修饰词成分

static const SegTokenType SEG_TOKEN_SEMANTIC_MIN = SEG_TOKEN_SEMANTIC | 0x0001; //最小语义单元
static const SegTokenType SEG_TOKEN_SEMANTIC_MAX = SEG_TOKEN_SEMANTIC | 0x0002; //最大语义单元

static const SegTokenType SEG_TOKEN_RETRIEVE_BASIC  = SEG_TOKEN_RETRIEVE | 0x0001; //基本检索单元
static const SegTokenType SEG_TOKEN_RETRIEVE_EXTEND = SEG_TOKEN_RETRIEVE | 0x0002; //扩充检索单元
static const SegTokenType SEG_TOKEN_RETRIEVE_BASIC_SUB = SEG_TOKEN_RETRIEVE | 0x0004; //基本检索单元的子串
     

#define IsSemanticToken(segTokenType) ((segTokenType) & SEG_TOKEN_SEMANTIC)
#define IsRetrieveToken(segTokenType) ((segTokenType) & SEG_TOKEN_RETRIEVE)
#define IsBasicRetrieveToken(segTokenType) (((segTokenType) & SEG_TOKEN_RETRIEVE_BASIC) == SEG_TOKEN_RETRIEVE_BASIC)
#define IsExtendRetrieveToken(segTokenType) (((segTokenType) & SEG_TOKEN_RETRIEVE_EXTEND) == SEG_TOKEN_RETRIEVE_EXTEND)

/*
 * semantic type in knowledge base, such as "产品类型-简单"
 */
class SemanticTag 
{
public :
	SemanticTag(const int16_t id = 0, const float w = 0.0f);
	
	int16_t id;           // tag id 
	float weight;         // the weight of the type
}__attribute__ ((packed));


/*
 * SegToken represent each token in SegResult
 */
class SegToken
{
public:
    const char* pWord;      //pointer to the word's beginning in input text, not ended with '\0'
    uint16_t length;        //词语的长度，以字节为单位计算,word length in encoding bytes

    int32_t charOffset;     //词语在输入文本中的位置，以字为单位计算
    int32_t charCount;      //词语的长度，以字为单位计算

    SegTokenType tokenType;   //type of the SegToken token, 
                              //which could be SEG_TOKEN_SEMANTIC(语义单元), 
                              //               SEG_TOKEN_RETRIEVE(检索单元),
                              //               SEG_TOKEN_PHRASE(短语).

    uint8_t isDictEntry; //if token is in dict. 0: not in dict; 1: in dict 

    SegToken* pParentToken; //pointer to parent token, NULL if has no parent
                            //eg. For a SEG_TOKEN_RETRIEVE_BASIC token, 
                            //its parent token would be a SEG_TOKEN_SEMANTIC token.

    SegToken* pLeftSibling; //pointer to left sibling token, NULL if has no left sibling 
    SegToken* pRightSibling;//pointer to right sibling token, NULL if has no right sibling

    SegToken* pSubToken;    //pointer to sub-tokens, NULL if has no sub-tokens 
    uint8_t subTokenNum;    //number of sub-tokens, 0 if has no sub-tokens
                            //eg. For a SEG_TOKEN_SEMANTIC token: 菊花茶 
                            //its sub-tokens are: 菊花  茶
                            //its subTokenNum is 2

    SegToken* pRelatedToken;    //pointer to related tokens, such as SEG_TOKEN_RETRIEVE_EXTENTD token, eg. 花茶, 
                                //NULL if has no related tokens
    uint8_t relatedTokenNum;    //number of related tokens, 0 if has no related tokens


    float weight;             //Reserved. weight of the token, default 0.0f
    uint8_t posTagId;         //the id of POS tag 
                              //eg. For tagId: 46, its POS tag is "n"; tagId: 82, its POS tag is "v"

    PropertyType property;    //property type for dynamically recognized word
                              //eg. WS_PROPERTY_NUM: number
                              //    WS_PROPERTY_PER: person name
                              //    WS_PROPERTY_ORG_RULE: organization name
                              //    WS_PROPERTY_EMAIL: email address

    const SemanticTag* pSemanticTag;//semantic tags of the token 
    uint8_t semanticTagNum;   //number of semantic tags 
                              //eg. For 菊花茶, its semantic tags are "[|/产品类型-简单@6719.0 /基本词-中文@1909.0 |] "
                              //number of semantic tags is 2

    const uint8_t* pUsrVal;   //user value defined in dictionary, added during dictionary building. 
    uint16_t usrValLen;       //the length of user value defined in dictionary.

    uint64_t usrTag;       //user tag, set by user during run-time, 0 by default

    SegToken();            //default constructor
    virtual ~SegToken();           //destructor
    SegToken(const SegToken& token); //copy constructor
    SegToken& operator=(const SegToken& token); //assignment operator

protected:
    bool isShared;   //if the token is shared. The token should be freed if it's not shared.
};

}//namespace
#endif
