#include "stdafx.h"

std::string to_string(int val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}
 
int from_string(const std::string& s)
{
  std::istringstream iss(s);
  int res;
  iss >> res;
  return res;
}
