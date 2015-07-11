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

std::string dev::pad(std::string in)
{
  return dev::lpad(dev::rpad(in));
}

std::string dev::lpad(std::string in)
{
  size_t i = 0;
  for(size_t i = 0; i < in.size(); i++)
  {
    if(!isspace(in[i]))
    {
      return in.substr(i - 1, in.size());
    }
  }
  return "";
}

std::string dev::rpad(std::string in)
{
  size_t i = in.size();
  for(i; i > 0; i--)
  {
    size_t x = i - 1;
    if(!isspace(in[x]))
    {
      return in.substr(0, x + 1);
    }
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
  long tmp = 0;
  long otmp = 0;
  std::vector<std::string> out;
  while((tmp = dev::find(stream, f)) >= 0)
  {
    out.push_back(stream.substr(otmp, tmp));
    otmp = tmp;
  }
  return out;
}
