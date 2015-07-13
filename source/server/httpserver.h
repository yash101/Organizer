#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include "tcp_socket.h"
#include "http.h"
#include <string>
#include <map>
namespace dev
{
  class HttpServer;
  class HttpServerSession;

  enum HttpProtocol_t
  {
    HTTP_1_0,
    HTTP_1_1,
    PROTO_UNKNOWN
  };

  class HttpServer : public dev::TcpServer
  {
  private:
    //Order in typical invocation cycle ;)
    void worker(dev::TcpServerSession* session);
    void process_request(dev::HttpServerSession* session);
      void parse_get_queries(dev::HttpServerSession* session, long location);
      void add_cookie(dev::HttpServerSession* session, std::string data);
      void parse_post_queries(dev::HttpServerSession* session);
    void check_request(dev::HttpServerSession* session);
    void prepare_session(dev::HttpServerSession* session);

    //This is the function which the user should modify through inheritance
    virtual void on_request(dev::HttpServerSession& session);

    void check_session_response(dev::HttpServerSession* session);
    void send_response(dev::HttpServerSession* session);
  };

  class HttpServerSession
  {
  public:
    dev::TcpServerSession* connection;
    dev::HttpServer* server;

    dev::HttpQueryType method;
    dev::HttpProtocol_t proto;
    std::string path;
    std::string processed_path;

    //Queries
    ///Both
    std::map<std::string, dev::HttpQuery> queries;
    ///GET
    std::map<std::string, dev::HttpQuery> get;
    ///POST
    std::map<std::string, dev::HttpQuery> post;

    //Cookies
    std::map<std::string, dev::HttpCookie> cookies;

    //Headers
    ///Incoming headers
    std::map<std::string, std::string> incoming_headers;
    ///Outgoing headers
    std::map<std::string, std::string> headers;

    int responseCode;
    std::string responseStatus;
    //Response text --> What will be sent back as the content
    std::string response;
  };

  class HttpServerException : public std::exception
  {
  private:
    const int _code;
    const char* _message;

  public:
    HttpServerException();
    HttpServerException(const int code);
    HttpServerException(const char* message);
    HttpServerException(const int code, const char* message);

    virtual const char* what() const throw();
    const int code();

    virtual ~HttpServerException();
  };
}
#endif // HTTPSERVER_H
