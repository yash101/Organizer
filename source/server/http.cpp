#include "http.h"

dev::HttpQuery::HttpQuery() :
  value(""),
  type(GET)
{}

dev::HttpQuery::HttpQuery(HttpQueryType tp, std::string val) :
  value(val),
  type(tp)
{}

std::string dev::encodeURI(std::string in)
{
  return in;
}

std::string dev::decodeURI(std::string in)
{
  return in;
}
