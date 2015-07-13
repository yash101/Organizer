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

  class HttpCookie
  {
  public:
    std::string value;
    std::string path;

    HttpCookie(std::string value);
    HttpCookie(std::string value, std::string path);
    HttpCookie();
  };

  std::string encodeURI(std::string in);
  std::string decodeURI(std::string in);
  void iencodeURI(std::string& in);
  void idecodeURI(std::string& in);

  std::string httpStatus(int code);

  std::string getHTTPTimestamp();

  extern std::string day[];
  extern std::string months[];
}
#endif // HTTP_H
