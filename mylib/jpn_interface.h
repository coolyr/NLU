#ifndef JPN_INTERFACE_H
#define JPN_INTERFACE_H

#include "ali_tokenizer_define.h"
#include "ali_tokenizer.h"
#include "jpn_define.h"

namespace ws
{
namespace jpn
{

static const SegTokenType SEG_TOKEN_ORI_TOKEN = 0x00000400; //原型词
#define IsOriToken(segTokenType) ((segTokenType) & SEG_TOKEN_ORI_TOKEN)

class SegTokenJPN : public SegToken
{
public:
    JpnDeformType	deformTypeId; //id of deform type, which decides what deformation method can be used in this word
    JpnDeformMtd 	deformMtdId;//id of deformation method, which is like stem expanding. see jpn_define.h for deformation values.
    const char* pPron;//pronunciation of this word
    uint32_t    pronLen;//count by characters
    const char* pTone;//pronunciation with tone of this word
    uint32_t    toneLen;//count by characters

public:
    SegTokenJPN();
    virtual ~SegTokenJPN();
    SegTokenJPN(const SegTokenJPN& token);
    SegTokenJPN& operator=(const SegTokenJPN& token);
};

class Utility
{
public:
	static const char* GetDeformTypeName(AliTokenizer* pTokenizer, JpnDeformType deformTypeId);
	static const char* GetDeformMtdName(AliTokenizer* pTokenizer, JpnDeformMtd deformMtdId);
	static JpnDeformType GetDeformTypeId(AliTokenizer* pTokenizer, const char* deformTypeName);
	static JpnDeformMtd GetDeformMtdId(AliTokenizer* pTokenizer, const char* deformMtdName);

};
}//namespace jpn
}//namespace ws
#endif
