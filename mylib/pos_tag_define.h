#ifndef POS_TAG_DEFINE_H
#define POS_TAG_DEFINE_H

#include <sys/types.h>
#include <stdint.h>

namespace ws
{

namespace chn
{
static const uint8_t POS_TAG_DEF = 0;//Default, Unknown
static const uint8_t POS_TAG_AG = 1; //Ag:形语素
static const uint8_t POS_TAG_BG = 2; //Bg:区别语素
static const uint8_t POS_TAG_DG = 3; //Dg:副语素
static const uint8_t POS_TAG_EG = 4; //Eg:叹语素
static const uint8_t POS_TAG_MG = 5; //Mg:数语素
static const uint8_t POS_TAG_NG = 6; //Ng:名语素
static const uint8_t POS_TAG_QG = 7; //Qg:量语素
static const uint8_t POS_TAG_RG = 8; //Rg:代语素
static const uint8_t POS_TAG_T = 9; //t:时间词
static const uint8_t POS_TAG_TG = 10; //Tg:时语素
static const uint8_t POS_TAG_UG = 11; //Ug:助语素
static const uint8_t POS_TAG_VG = 12; //Vg:动语素
static const uint8_t POS_TAG_YG = 13; //Yg:语气词语素
static const uint8_t POS_TAG_ZG = 14; //Zg:状态词语素
static const uint8_t POS_TAG_A = 15; //a:形容词
static const uint8_t POS_TAG_AD = 16; //ad:副形词
static const uint8_t POS_TAG_AN = 17; //an:名形词
static const uint8_t POS_TAG_B = 18; //b:区别词
static const uint8_t POS_TAG_C = 19; //c:连词
static const uint8_t POS_TAG_D = 20; //d:副词
static const uint8_t POS_TAG_DC = 21; //dc:程度副词
static const uint8_t POS_TAG_DF = 22; //df:否定副词
static const uint8_t POS_TAG_E = 23; //e:叹词
static const uint8_t POS_TAG_F = 24; //f:方位词
static const uint8_t POS_TAG_H = 25; //h:前接成分
static const uint8_t POS_TAG_I = 26; //i:成语
static const uint8_t POS_TAG_IA = 27; //ia:形容词功能成语
static const uint8_t POS_TAG_IB = 28; //ib:区别词功能成语
static const uint8_t POS_TAG_ID = 29; //id:副词功能成语
static const uint8_t POS_TAG_IN = 30; //in:名词功能成语
static const uint8_t POS_TAG_IV = 31; //iv:动词功能成语
static const uint8_t POS_TAG_J = 32; //j:简称略语
static const uint8_t POS_TAG_JB = 33; //jb:区别词功能简称
static const uint8_t POS_TAG_JD = 34; //jd:副词功能简称
static const uint8_t POS_TAG_JN = 35; //jn:名词功能简称
static const uint8_t POS_TAG_JV = 36; //jv:动词功能简称
static const uint8_t POS_TAG_K = 37; //k:后接成分
static const uint8_t POS_TAG_L = 38; //l:习用语
static const uint8_t POS_TAG_LA = 39; //la:形容词功能习用语
static const uint8_t POS_TAG_LB = 40; //lb:区别词功能成语
static const uint8_t POS_TAG_LD = 41; //ld:副词功能习用语
static const uint8_t POS_TAG_LN = 42; //ln:名词功能习用语
static const uint8_t POS_TAG_LV = 43; //lv:动词功能习用语
static const uint8_t POS_TAG_M = 44; //m:数词
static const uint8_t POS_TAG_MQ = 45; //mq:数量词
static const uint8_t POS_TAG_N = 46; //n:名词
static const uint8_t POS_TAG_NR = 47; //nr:人名
static const uint8_t POS_TAG_NRF = 48; //nrf:姓
static const uint8_t POS_TAG_NRG = 49; //nrg:名
static const uint8_t POS_TAG_NS = 50; //ns:地名
static const uint8_t POS_TAG_NT = 51; //nt:机构团体
static const uint8_t POS_TAG_NX = 52; //nx:非汉字串
static const uint8_t POS_TAG_NZ = 53; //nz:其他专名
static const uint8_t POS_TAG_O = 54; //o:拟声词
static const uint8_t POS_TAG_P = 55; //p:介词
static const uint8_t POS_TAG_Q = 56; //q:量词
static const uint8_t POS_TAG_QB = 57; //qb:不定量词
static const uint8_t POS_TAG_QC = 58; //qc:成形量词
static const uint8_t POS_TAG_QD = 59; //qd:度量词
static const uint8_t POS_TAG_QE = 60; //qe:个体量词
static const uint8_t POS_TAG_QJ = 61; //qj:集体量词
static const uint8_t POS_TAG_QL = 62; //ql:倍率量词
static const uint8_t POS_TAG_QR = 63; //qr:容器量词
static const uint8_t POS_TAG_QT = 64; //qt:时量词
static const uint8_t POS_TAG_QV = 65; //qv:动量词
static const uint8_t POS_TAG_QZ = 66; //qz:种类量词
static const uint8_t POS_TAG_R = 67; //r:代词
static const uint8_t POS_TAG_RR = 68; //rr:人称代词
static const uint8_t POS_TAG_RY = 69; //ry:疑问代词
static const uint8_t POS_TAG_RYW = 70; //ryw:谓词性疑问代词
static const uint8_t POS_TAG_S = 71; //s:处所词
static const uint8_t POS_TAG_TT = 73; //tt:专名时间词
static const uint8_t POS_TAG_U = 74; //u:助词
static const uint8_t POS_TAG_UD = 75; //ud:助词“的”
static const uint8_t POS_TAG_UE = 76; //ue:助词“得”
static const uint8_t POS_TAG_UI = 77; //ui:助词“地”
static const uint8_t POS_TAG_UL = 78; //ul:助词“了”
static const uint8_t POS_TAG_UO = 79; //uo:助词“过”
static const uint8_t POS_TAG_US = 80; //us:助词“所”
static const uint8_t POS_TAG_UZ = 81; //uz:助词“着”
static const uint8_t POS_TAG_V = 82; //v:动词
static const uint8_t POS_TAG_VD = 83; //vd:副动词
static const uint8_t POS_TAG_VI = 84; //vi:不及物动词
static const uint8_t POS_TAG_VL = 85; //vl:联系动词
static const uint8_t POS_TAG_VN = 86; //vn:名动词
static const uint8_t POS_TAG_VQ = 87; //vq:趋向动词
static const uint8_t POS_TAG_VTN = 88; //
static const uint8_t POS_TAG_VU = 89; //vu:助动词
static const uint8_t POS_TAG_VX = 90; //vx:形式动词
static const uint8_t POS_TAG_W = 91; //w:标点符号
static const uint8_t POS_TAG_WD = 92; //wd:逗号
static const uint8_t POS_TAG_WF = 93; //wf:分号
static const uint8_t POS_TAG_WJ = 94; //wj:句号
static const uint8_t POS_TAG_WKY = 95; //wky:右括号
static const uint8_t POS_TAG_WKZ = 96; //wkz:左括号
static const uint8_t POS_TAG_WM = 97; //wm:冒号
static const uint8_t POS_TAG_WP = 98; //wp:破折号
static const uint8_t POS_TAG_WS = 99; //ws:省略号
static const uint8_t POS_TAG_WT = 100; //wt:叹号
static const uint8_t POS_TAG_WU = 101; //wu:顿号
static const uint8_t POS_TAG_WW = 102; //ww:问号
static const uint8_t POS_TAG_WYY = 103; //wyy:右引号
static const uint8_t POS_TAG_WYZ = 104; //wyz:左引号
static const uint8_t POS_TAG_Y = 105; //y:语气词
static const uint8_t POS_TAG_Z = 106; //z:状态词
}//namespace chn

}//namespace ws
#endif
