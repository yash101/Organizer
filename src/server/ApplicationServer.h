#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H
#include "httpserver.h"

//Holds the mime data
#include "mimeserver.h"

#include <string>
#include <map>

//For using lambda functions
#ifndef DISABLE_LAMBDAS
#include <functional>
#endif


typedef bool (*ServerFunction_t)(server::HttpServerSession&);


class ApplicationServer : public server::HttpServer
{
protected:

  std::map<std::string, ServerFunction_t> _functions;
  std::map<std::string, std::string> _static_handlers;

#ifndef DISABLE_LAMBDAS
  std::map<std::string, std::function<bool(server::HttpServerSession&)> > _lambdas;
#endif

  static bool static_handler(server::HttpServerSession& session, std::string addbasepath);
  void request_handler(server::HttpServerSession& session);
  void error_handler(int code, server::HttpServerSession& session);

public:

  ApplicationServer();

  void set_request_function(std::string regex, ServerFunction_t function);
#ifndef DISABLE_LAMBDAS
  void set_request_lambda(std::string regex, std::function<bool (server::HttpServerSession&)>);
#endif

  void set_static(std::string regex);
  void set_static(std::string regex, std::string addbasepath);

  ServerFunction_t errorHandler404;
  static bool handle404(server::HttpServerSession& session);

};

extern server::MimeServer mime_server;
#endif // APPLICATIONSERVER_H
