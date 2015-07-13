#include "http.h"
#include <time.h>
#include <sstream>
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

std::string dev::getHTTPTimestamp()
{
  time_t local_time;
  struct tm* Time;

  local_time = time(NULL);
//  Time = localtime(&local_time);
  Time = gmtime(&local_time);

  std::stringstream str;
  str << dev::day[Time->tm_wday] << ", ";
  str << Time->tm_mday << " ";
  str << dev::months[Time->tm_mon] << " ";
  str << Time->tm_year << " ";
  str << Time->tm_hour << ":";
  str << Time->tm_min << ":";
  str << Time->tm_sec << " GMT";

  return str.str();
}

std::string dev::day[] =
{
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun"
};

std::string dev::months[] =
{
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

dev::HttpCookie::HttpCookie() :
  value(""),
  path("/")
{}

dev::HttpCookie::HttpCookie(std::string val) :
  value(val),
  path("/")
{}

dev::HttpCookie::HttpCookie(std::string val, std::string loc) :
  value(val),
  path(loc)
{}
