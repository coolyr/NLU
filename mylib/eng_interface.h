#ifndef ENG_INTERFACE_H
#define ENG_INTERFACE_H

#include "ali_tokenizer_define.h"
#include "eng_define.h"

namespace ws
{
namespace eng
{

static const SegTokenType SEG_TOKEN_STEM = 0x00000400; //stem结果
static const SegTokenType SEG_TOKEN_DEFORM = 0x00000800; //词形还原结果
#define IsStemToken(segTokenType) ((segTokenType) & SEG_TOKEN_STEM)
#define IsDeformToken(segTokenType) ((segTokenType) & SEG_TOKEN_DEFORM)

class SegTokenENG : public SegToken
{
public:
    EngDeformType		deformTypeId;//id of type judged by word restore. Includes five values: ing, ed, s, er, est. Not implemented yet

public:
    SegTokenENG();
    virtual ~SegTokenENG();
    SegTokenENG(const SegTokenENG& token);
    SegTokenENG& operator=(const SegTokenENG& token);
};

}//namespace eng
}//namespace ws
#endif
