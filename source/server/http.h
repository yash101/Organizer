#ifndef HTTP_H
#define HTTP_H
#include <string>
namespace dev
{
  class HttpQuery;
  enum HttpQueryType
  {
    GET,
    POST
  };

  class HttpQuery
  {
  public:
    std::string value;
    HttpQueryType type;

    HttpQuery();
    HttpQuery(HttpQueryType tp, std::string val);
  };

  std::string encodeURI(std::string in);
  std::string decodeURI(std::string in);
}
#endif // HTTP_H
