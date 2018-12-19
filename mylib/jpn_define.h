#ifndef JPN_DEFINE_H
#define JPN_DEFINE_H

namespace ws
{
namespace jpn
{

//pos tag
static const uint8_t JPN_POS_DEF = 0;//Default Unknown
static const uint8_t JPN_POS_FILLING = 1;//フィラー,*,*,* (填料做填充作用)
static const uint8_t JPN_POS_EXCLAMATION = 2;//感動詞,*,*,*
static const uint8_t JPN_POS_SYMBOL_ALPHA = 3;//記号,アルファベット,*,*
static const uint8_t JPN_POS_SYMBOL_COMMON = 4;//記号,一般,*,*
static const uint8_t JPN_POS_SYMBOL_LBRACKET = 5;//記号,括弧開,*,*
static const uint8_t JPN_POS_SYMBOL_RBRACKET = 6;//記号,括弧閉,*,*
static const uint8_t JPN_POS_SYMBOL_STOP = 7;//記号,句点,*,*
static const uint8_t JPN_POS_SYMBOL_SPACE = 8;//記号,空白,*,*
static const uint8_t JPN_POS_SYMBOL_PAUSE = 9;//記号,読点,*,*(顿号)
static const uint8_t JPN_POS_ADJ_IDV = 10;//形容詞,自立,*,*
static const uint8_t JPN_POS_ADJ_END = 11;//形容詞,接尾,*,*
static const uint8_t JPN_POS_ADJ_NIDV = 12;//形容詞,非自立,*,*
static const uint8_t JPN_POS_AUX_REL_COMMON = 13;//助詞,格助詞,一般,*
static const uint8_t JPN_POS_AUX_REL_REF = 14;//助詞,格助詞,引用,*
static const uint8_t JPN_POS_AUX_REL_CONT = 15;//助詞,格助詞,連語,*
static const uint8_t JPN_POS_AUX_HINT = 16;//助詞,係助詞,*,*(提示性的)
static const uint8_t JPN_POS_AUX_END = 17;//助詞,終助詞,*,*
static const uint8_t JPN_POS_AUX_JOINT = 18;//助詞,接続助詞,*,*
static const uint8_t JPN_POS_AUX_SPE = 19;//助詞,特殊,*,*
static const uint8_t JPN_POS_AUX_ADV = 20;//助詞,副詞化,*,*
static const uint8_t JPN_POS_AUX_ADVAUX = 21;//助詞,副助詞,*,*
static const uint8_t JPN_POS_AUX_THREE = 22;//助詞,副助詞／並立助詞／終助詞,*,*
static const uint8_t JPN_POS_AUX_APPOSE = 23;//助詞,並立助詞,*,*
static const uint8_t JPN_POS_AUX_CONT = 24;//助詞,連体化,*,*
static const uint8_t JPN_POS_VU = 25;//助動詞,*,*,*
static const uint8_t JPN_POS_JOINT_OTHER = 26;//接続詞,*,*,*
static const uint8_t JPN_POS_JOINT_ADJ = 27;//接頭詞,形容詞接続,*,*
static const uint8_t JPN_POS_JOINT_NUM = 28;//接頭詞,数接続,*,*
static const uint8_t JPN_POS_JOINT_V = 29;//接頭詞,動詞接続,*,*
static const uint8_t JPN_POS_JOINT_N = 30;//接頭詞,名詞接続,*,*
static const uint8_t JPN_POS_V_IDV = 31;//動詞,自立,*,*
static const uint8_t JPN_POS_V_END = 32;//動詞,接尾,*,*
static const uint8_t JPN_POS_V_NIDV = 33;//動詞,非自立,*,*
static const uint8_t JPN_POS_ADV_COMMON = 34;//副詞,一般,*,*
static const uint8_t JPN_POS_ADV_AUX_JOINT = 35;//副詞,助詞類接続,*,*
static const uint8_t JPN_POS_N_SA = 36;//名詞,サ変接続,*,*
static const uint8_t JPN_POS_N_ADJ = 37;//名詞,ナイ形容詞語幹,*,*
static const uint8_t JPN_POS_N_COMMON = 38;//名詞,一般,*,*
static const uint8_t JPN_POS_N_REF = 39;//名詞,引用文字列,*,*
static const uint8_t JPN_POS_N_ADJV = 40;//名詞,形容動詞語幹,*,*
static const uint8_t JPN_POS_N_INH_COMMON = 41;//名詞,固有名詞,一般,*
static const uint8_t JPN_POS_N_INH_NAME = 42;//名詞,固有名詞,人名,一般
static const uint8_t JPN_POS_N_INH_SUR = 43;//名詞,固有名詞,人名,姓
static const uint8_t JPN_POS_N_INH_FML = 44;//名詞,固有名詞,人名,名
static const uint8_t JPN_POS_N_INH_ORG = 45;//名詞,固有名詞,組織,*
static const uint8_t JPN_POS_N_INH_PLA = 46;//名詞,固有名詞,地域,一般
static const uint8_t JPN_POS_N_INH_COUNTRY = 47;//名詞,固有名詞,地域,国
static const uint8_t JPN_POS_N_NUM = 48;//名詞,数,*,*
static const uint8_t JPN_POS_N_JOINT = 49;//名詞,接続詞的,*,*
static const uint8_t JPN_POS_N_END_SA = 50;//名詞,接尾,サ変接続,*
static const uint8_t JPN_POS_N_END_COMMON = 51;//名詞,接尾,一般,*
static const uint8_t JPN_POS_N_END_ADJV = 52;//名詞,接尾,形容動詞語幹,*
static const uint8_t JPN_POS_N_END_AUXNUM = 53;//名詞,接尾,助数詞,*
static const uint8_t JPN_POS_N_END_AUXV = 54;//名詞,接尾,助動詞語幹,*
static const uint8_t JPN_POS_N_END_NAME = 55;//名詞,接尾,人名,*
static const uint8_t JPN_POS_N_END_PLA = 56;//名詞,接尾,地域,*
static const uint8_t JPN_POS_N_END_SPE = 57;//名詞,接尾,特殊,*
static const uint8_t JPN_POS_N_END_ADV = 58;//名詞,接尾,副詞可能,*
static const uint8_t JPN_POS_N_ACT_COMMON = 59;//名詞,代名詞,一般,*
static const uint8_t JPN_POS_N_ACT_BREF = 60;//名詞,代名詞,縮約,*
static const uint8_t JPN_POS_N_V_NIDV = 61;//名詞,動詞非自立的,*,*
static const uint8_t JPN_POS_N_SPE_AUXV = 62;//名詞,特殊,助動詞語幹,*
static const uint8_t JPN_POS_N_NIDV_COMMON = 63;//名詞,非自立,一般,*
static const uint8_t JPN_POS_N_NIDV_ADJV = 64;//名詞,非自立,形容動詞語幹,*
static const uint8_t JPN_POS_N_NIDV_AUXV = 65;//名詞,非自立,助動詞語幹,*
static const uint8_t JPN_POS_N_NIDV_ADV = 66;//名詞,非自立,副詞可能,*
static const uint8_t JPN_POS_N_ADV = 67;//名詞,副詞可能,*,*
static const uint8_t JPN_POS_CONT = 68;//連体詞,*,*,*
static const uint8_t JPN_POS_AUX_MIDDLE = 69;//その他,間投,*,* (插入语作感叹用)


//deform type
enum JpnDeformType
{
DEFORM_TYPE_DEF = 0,//Default Unknown
KA_KATA = 1,//カ変 クル
KA_CHN = 2,//カ変 来ル
SA_SILU_SUFFIX = 3,//サ変 −スル
SA_ZILU = 4,//サ変 −ズル
SA_SILU = 5,//サ変 スル
RA = 6,//ラ変
SECT1 = 7,//一段
SECT1_GIVE = 8,//一段 クレル
SECT1_GET = 9,//一段 得ル
UP_SECT2_DA = 10,//上二 ダ行
UP_SECT2_HA = 11,//上二 ハ行
DOWN_SECT2_KA = 12,//下二 カ行
DOWN_SECT2_GA = 13,//下二 ガ行
DOWN_SECT2_TA = 14,//下二 タ行
DOWN_SECT2_DA = 15,//下二 ダ行
DOWN_SECT2_HA = 16,//下二 ハ行
DOWN_SECT2_MA = 17,//下二 マ行
DOWN_SECT2_GET = 18,//下二 得
STATIC = 19,//不変化型
SECT5_V_I = 20,//五段 カ行イ音便
SECT5_V_TT = 21,//五段 カ行促音便
SECT5_V_TT_UKU = 22,//五段 カ行促音便ユク
SECT5_GA = 23,//五段 ガ行
SECT5_SA = 24,//五段 サ行
SECT5_TA = 25,//五段 タ行
SECT5_NA = 26,//五段 ナ行
SECT5_BA = 27,//五段 バ行
SECT5_MA = 28,//五段 マ行
SECT5_LA = 29,//五段 ラ行
SECT5_LA_ALU = 30,//五段 ラ行アル
SECT5_LA_SPE = 31,//五段 ラ行特殊
SECT5_WA_U = 32,//五段 ワ行ウ音便
SECT5_WA_TT = 33,//五段 ワ行促音便
SECT4_SA = 34,//四段 サ行
SECT4_TA = 35,//四段 タ行
SECT4_HA = 36,//四段 ハ行
SECT4_BA = 37,//四段 バ行
ADJ_AWUO = 38,//形容詞 アウオ段
ADJ_II = 39,//形容詞 イイ
ADJ_I = 40,//形容詞 イ段
OLD_KI = 41,//文語 キ
OLD_KERI = 42,//文語 ケリ
OLD_GOTOSI = 43,//文語 ゴトシ
OLD_NARI = 44,//文語 ナリ
OLD_BESI = 45,//文語 ベシ
OLD_MAJI = 46,//文語 マジ
OLD_RI = 47,//文語 リ
OLD_RU = 48,//文語 ル
SPE_JYA = 49,//特殊 ジャ
SPE_TA = 50,//特殊 タ
SPE_TAI = 51,//特殊 タイ
SPE_DA = 52,//特殊 ダ
SPE_DESU = 53,//特殊 デス
SPE_NAI = 54,//特殊 ナイ
SPE_NU = 55,//特殊 ヌ
SPE_MASU = 56,//特殊 マス
SPE_YA = 57,//特殊 ヤ
DEFORM_TYPE_OTHER = 58//*
};

//deform method
enum JpnDeformMtd
{
DEFORM_MTD_DEF = 0,//Default Unknown
GARU_JOINT = 1,//ガル接続
SUPPOSE = 2,//仮定形
SUPPOSE_BRIEF1 = 3,//仮定縮約1
SUPPOSE_BRIEF2 = 4,//仮定縮約2
N_JOINT_COMMON = 5,//体言接続
N_JOINT_SPE = 6,//体言接続特殊
N_JOINT_SPE2 = 7,//体言接続特殊2
CMD = 8,//命令e
CMD_I = 9,//命令i
CMD_RO = 10,//命令ro
CMD_YO = 11,//命令yo
BASIC = 12,//基本形
BASIC_TT = 13,//基本形-促音便
OLD_BASIC = 14,//文語基本形
IF_WU_JOINT = 15,//未然ウ接続
IF_LU_JOINT = 16,//未然ヌ接続
IF_RERU_JOINT = 17,//未然レル接続
IF_BASIC = 18,//未然形
IF_SPE = 19,//未然特殊
MODERN_BASIC = 20,//現代基本形
CONN_GOZAI_JOINT = 21,//連用ゴザイ接続
CONN_TA_JOINT = 22,//連用タ接続
CONN_TE_JOINT = 23,//連用テ接続
CONN_DE_JOINT = 24,//連用デ接続
CONN_NI_JOINT = 25,//連用ニ接続
CONN = 26,//連用形
PUNC_CHANGE = 27,//音便基本形
DEFORM_MTD_OTHER = 28//*
};

}//namespace jpn
}//namespace ws
#endif
