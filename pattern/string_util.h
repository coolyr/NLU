//author 105925xhz
//date:2016/6/2
//
#ifndef  QP_STRING_UTIL_H_
#define  QP_STRING_UTIL_H_

#include <string>
#include <vector>
#include "../mylib/pcre++.h"
#include "../common_macros.h"
#include "../log.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);

class StringUtils {
 public:
  StringUtils();
  ~StringUtils();

  int Replace(std::string &base, std::string src, std::string des);
  void Split(const std::string& str, std::vector<std::string>& ret_, std::string sep);
  std::string Trim(const std::string& str);
  std::vector<std::string> GetSubstr(std::string expr, std::string stuff);
  bool Matched(const std::string expr, const std::string stuff);
 private:

  DIALOG_SEG_LOG_DECLARE();
};

DIALOG_SEG_END_NAMESPACE(qp);

#endif
