#include "string_util.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
DIALOG_SEG_LOG_SETUP(qp, StringUtils);

using std::string;
using std::vector;

StringUtils::StringUtils() {}
StringUtils::~StringUtils() {}
 
int StringUtils::Replace(string &base, string src, string des) {
  string::size_type pos = 0;
  string::size_type srclen = src.size();
  pos = base.find(src, pos);
  while ((pos != string::npos)) {
    base.replace(pos, srclen, des);
    pos = base.find(src, pos);
  }
  return pos;
}

void StringUtils::Split(const string& str, vector<string>& ret, string sep) {
  if (str.empty()) {
    return;
  }
  string tmp;
  string::size_type pos_begin = str.find_first_not_of(sep);
  string::size_type comma_pos = 0;
  while (pos_begin != string::npos) {
    comma_pos = str.find(sep, pos_begin);
    if (comma_pos != string::npos) {
      tmp = str.substr(pos_begin, comma_pos - pos_begin);
      pos_begin = comma_pos + sep.length();
    } else {
      tmp = str.substr(pos_begin);
      pos_begin = comma_pos;
    }
    if (!tmp.empty()) {
      ret.push_back(tmp);
      tmp.clear();
    }
  }
}


string StringUtils::Trim(const string& str) {
  string::size_type pos = str.find_first_not_of(" \n\r\t");
  if (pos == string::npos) {
    return str;
  }
  string::size_type pos2 = str.find_last_not_of(" \n\r\t");
  if (pos2 != string::npos) {
    return str.substr(pos, pos2 - pos + 1);
  }
  return str.substr(pos);
}

vector<string> GetSubStr(string expr, string stuff) {

}

bool Matched(const string expr, string stuff) {

}

DIALOG_SEG_END_NAMESPACE(qp);

