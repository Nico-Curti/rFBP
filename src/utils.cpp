#include <utils.h>

std::vector<std::string> split(const std::string &txt, const std::string &del)
{
  std::vector<std::string> token;
  std::size_t pos = txt.find_first_of(del), start = 0, end = txt.size();
  while(pos != std::string::npos)
  {
    if(pos) token.push_back(txt.substr(start, pos));
    start += pos + 1;
    pos = txt.substr(start, end).find_first_of(del);
  }
  if(start != end) token.push_back(txt.substr(start, pos));
  return token;
}
