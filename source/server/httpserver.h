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
    void worker(dev::TcpServerSession* session);
    virtual void on_request(dev::HttpServerSession& session);
  };

  class HttpServerSession
  {
  public:
    dev::TcpServerSession* connection;
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
}
#endif // HTTPSERVER_H
