#include "ApplicationServer.h"
#include "stringproc.h"
#include "entry.h"
#include "defaults.h"
#include <regex>
#include <sstream>
#include <iostream>

ApplicationServer::ApplicationServer()
{
  errorHandler404 = &ApplicationServer::handle404;
}

bool ApplicationServer::handle404(server::HttpServerSession& session)
{
  std::stringstream str;
#define o str << std::endl <<
  o "<DOCTYPE html>";
  o "<html>";
    o "<head>";
      o "<title>404 Not Found</title>";
    o "</head>";
    o "<body>";
      o "<h1><center>404 Not Found</center></h1>";
      o "<p>The path requested could not be loaded. This is all we know!</p>";
      o "<p>Please check the URL you may have entered or check the link you clicked!</p>";
      o "<h2>Error Traceback</h2>";
        o "<div style=\"background-color: #C6C6C6; margin: 32px;\">";
          o "<pre>";
            o "Path: [" << session.Path << "]";
          o "</pre>";
        o "</div>";
      o "<hr />";
      o "<div style=\"height: 300px;\"></div>";
      o "<p style=\"background-color: #C6C6C6;\">Powered by <a href=\"https://github.com/yash101/ApplicationServer/\">ApplicationServer</a>!</p>";
    o "</body>";
  o "<html>";
#undef o
  session.Response.stype = str.str();
  session.Response.type = server::STRING;
  session.ResponseStatusCode = 404;
  session.headers["content-type"] = "text/html";
}

void ApplicationServer::set_request_function(std::string query, ServerFunction_t callback)
{
  _functions[query] = callback;
}

#ifndef DISABLE_LAMBDAS
void ApplicationServer::set_request_lambda(std::string query, std::function<bool (server::HttpServerSession&)> callback)
{
  _lambdas[query] = callback;
}
#endif

void ApplicationServer::request_handler(server::HttpServerSession& session)
{
  bool success = false;

  for(std::map<std::string, ServerFunction_t>::const_iterator it = _functions.begin(); it != _functions.end(); ++it)
  {
    if(std::regex_match(session.Path, std::regex(it->first)))
    {
      if(it->second != NULL)
      {
        if(it->second(session))
        {
          success = true;
          break;
        }
      }
    }
  }

  if(success) return;

#ifndef DISABLE_LAMBDAS
  for(std::map<std::string, std::function<bool (server::HttpServerSession&)> >::const_iterator it = _lambdas.begin(); it != _lambdas.end(); ++it)
  {
    if(std::regex_match(session.Path, std::regex(it->first)))
    {
      if(it->second(session))
      {
        success = true;
        break;
      }
    }
  }
#endif

  if(success) return;

  for(std::map<std::string, std::string>::const_iterator it = _static_handlers.begin(); it != _static_handlers.end(); ++it)
  {
    if(std::regex_match(session.Path, std::regex(it->first)))
    {
      if(static_handler(session, it->second))
      {
        success = true;
        break;
      }
    }
  }

  if(success) return;

  errorHandler404(session);
}

void ApplicationServer::set_static(std::string regex)
{
  _static_handlers[regex] = "";
}

void ApplicationServer::set_static(std::string regex, std::string addbasepath)
{
  _static_handlers[regex] = addbasepath;
}

server::MimeServer mime_server(server::configuration()["mime_file"].c_str());
