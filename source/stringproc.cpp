#include "stringproc.h"
#include <string.h>
bool operator==(std::string a, std::string b)
{
  return !strcmp(a.c_str(), b.c_str());
}

bool operator!=(std::string a, std::string b)
{
  return strcmp(a.c_str(), b.c_str());
}

long dev::find(std::string in, char f)
{
  for(size_t i = 0; i < in.size(); i++)
  {
    if(in[i] == f) return (long) i;
  }
  return -1;
}

long dev::find(std::string in, std::string f)
{
  //If the search string is shorter than query (micro-optimization)
  if(in.size() < f.size()) return -1;
  size_t x = in.find(f);
  //Lil' micro-optimization to produce inline assembly in dumber compilers
  return x == std::string::npos ? -1 : x;
}

template<typename T> void dev::fromString(T& x, std::string in)
{
  std::stringstream str(in);
  str >> x;
  return;
}

bool dev::contains(char find, std::string dict)
{
  for(size_t i = 0; i < dict.size(); i++)
  {
    if(find == dict[i]) return true;
  }
  return false;
}

std::string dev::pad(std::string str)
{
  std::string in = str;
  size_t i = 0;
  while(isspace(in[i]) && i < in.size()) i++;
  in = in.substr(i, in.size());

  i = in.size();
  while(isspace(in[i]) && i != 0) i--;
  in = in.substr(0, i);

  return in;
}

std::string dev::lpad(std::string in)
{
  for(size_t i = 0; i < in.size(); i++)
  {
    if(!isspace(in[i])) return in.substr(i, in.size());
  }
  return "";
}

std::string dev::rpad(std::string in)
{
  long long i = in.size() - 1;
  for(; i >= 0; i++)
  {
    if(!isspace(in[i])) return in.substr(0, i);
  }
  return "";
}

void dev::ipad(std::string& in)
{
  in = dev::pad(in);
}

void dev::ilpad(std::string& in)
{
  in = dev::lpad(in);
}

void dev::irpad(std::string& in)
{
  in = dev::rpad(in);
}

std::string dev::tolower(std::string in)
{
  std::string x = in;
  for(size_t i = 0; i < x.size(); i++)
  {
    x[i] = ::tolower(x[i]);
  }
  return x;
}

std::string dev::toupper(std::string in)
{
  std::string x = in;
  for(size_t i = 0; i < x.size(); i++)
  {
    x[i] = ::toupper(x[i]);
  }
  return x;
}

void dev::tolowerf(const char* str)
{
  char* x = (char*) str;
  while(*x != '\0')
  {
    *x = ::tolower(*x++);
  }
}

void dev::toupperf(const char* str)
{
  char* x = (char*) str;
  while(*x != '\0')
  {
    *x = ::toupper(*x++);
  }
}

void dev::itolower(std::string& in)
{
  for(size_t i = 0; i < in.size(); i++)
  {
    in[i] = ::tolower(in[i]);
  }
}

void dev::itoupper(std::string& in)
{
  for(size_t i = 0; i < in.size(); i++)
  {
    in[i] = ::tolower(in[0]);
  }
}

bool dev::getline(std::string& buffer, std::string end, std::istream& stream)
{
  buffer.clear();
  std::string out;
  while(stream.good())
  {
    if(out.size() >= end.size())
    {
      if(out.substr(out.size() - end.size(), out.size()) == end)
      {
        buffer = out.substr(0, out.size() - end.size());
        return true;
      }
    }
    out += (char) stream.get();
  }
  return false;
}

std::vector<std::string> dev::split(std::string stream, char f)
{
  std::string str = stream;
  std::vector<std::string> out;
  long ocurrence = 0;
  while((ocurrence = dev::find(str, f)) >= 0)
  {
    out.push_back(str.substr(0, ocurrence));
    str = str.substr(ocurrence + 1, str.size());
  }
  out.push_back(str);
  return out;
}

std::vector<std::string> dev::split(std::string stream, std::string f)
{
  std::string str = stream;
  std::vector<std::string> out;
  long ocurrence = 0;
  while((ocurrence = dev::find(str, f)) >= 0)
  {
    out.push_back(str.substr(0, ocurrence));
    str = str.substr(ocurrence + f.size(), str.size());
  }
  if(str.size() != 0) out.push_back(str);
  return out;
}
