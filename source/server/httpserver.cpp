#include "httpserver.h"
#include "../stringproc.h"
#define DEV_HTTP_SERVER_CPP
#include "httpserver_int.h"
void dev::HttpServer::worker(dev::TcpServerSession* connection)
{
  //Create a session object and set it up
  dev::HttpServerSession session;
  session.connection = connection;
  session.server = this;

  process_request(&session);
  check_request(&session);
  prepare_session(&session);

  on_request(session);

  check_session_response(&session);
  send_response(&session);
}

void dev::HttpServer::process_request(dev::HttpServerSession* session)
{
  //To add some compatibility with n00bs.
  std::vector<std::string> rn_nn;
  std::vector<std::string> rnrn_nnnn;
  rn_nn.push_back("\r\n");
  rn_nn.push_back("\n\n");
  rnrn_nnnn.push_back("\r\n\r\n");
  rnrn_nnnn.push_back("\n\n\n\n");

  //Buffer string
  std::string buffer;

  //Parse the first line [GET / HTTP/[1.0/1.1][\r\n]]
  //  Download the first line from the client
  buffer = session->connection->readLine('\n');
  if(buffer[buffer.size() - 1] == '\r' || buffer[buffer.size() - 1] == '\n') buffer.pop_back();
  //  Break the first line into it's components
  std::vector<std::string> parts = dev::split(buffer, ' ');

  //Make sure that we recieved all three parts. We'll ignore everything else
  if(parts.size() < 3) throw dev::HttpServerException("Bad Request");

  //Process the request method (GET/POST/etc.)
  if(parts[0] == "GET") session->method = GET;
  else if(parts[0] == "POST") session->method = POST;
  else throw dev::HttpServerException("Bad Request");

  //Process the path string
  session->path = parts[1];
  long loc = dev::find(parts[1], '?');
  if(loc >= 0) session->processed_path = parts[1].substr(0, loc);
  else session->processed_path = parts[1];
  dev::ipad(session->processed_path);
  //Parse GET queries
  if(loc >= 0 && parts[1][parts.size() - 1] != '?') parse_get_queries(session, loc + 1);

  //Parse protocol being used
  if(parts[2] == "HTTP/1.0") session->proto = HTTP_1_0;
  else if(parts[2] == "HTTP/1.1") session->proto = HTTP_1_1;
  else throw dev::HttpServerException("Bad Request");

  //Download and parse all headers
  while(true)
  {
    buffer = session->connection->readLine('\n');
    if(buffer[buffer.size() - 1] == '\r' || buffer[buffer.size() - 1] == '\n') buffer.pop_back();
    dev::ipad(buffer);

    if(buffer.size() == 0) break;

    long clocation = dev::find(buffer, ':');
    if(clocation < 0) continue;
    if(buffer[0] == ':') continue;

    std::string key = buffer.substr(0, clocation);
    std::string value = buffer.substr(clocation + 1, buffer.size() - 1);

    dev::ipad(key);
    dev::ipad(value);
    dev::itolower(key);

    if(key == "cookie")
    {
      add_cookie(session, value);
    }
    else
    {
      session->incoming_headers[key] = value;
    }
  }

  parse_post_queries(session);
}

void dev::HttpServer::parse_get_queries(dev::HttpServerSession* session, long location)
{
  std::string string = session->path.substr(location, session->path.size());
  std::vector<std::string> queries = dev::split(string, '&');
  for(size_t i = 0; i < queries.size(); i++)
  {
    //Handle corrupt request
    if(queries[i].size() == 1) continue;

    std::string key;
    std::string value;

    long loc = dev::find(queries[i], '=');
    if(loc < 0)
    {
      key = queries[i];
      value = "";
    }
    else
    {
      key = queries[i].substr(0, loc);
      value = queries[i].substr(loc + 1, queries[i].size());
    }

    dev::ipad(key);
    dev::ipad(value);

    session->get[key] = dev::HttpQuery(GET, value);
    session->queries[key] = session->get[key];
  }
}

void dev::HttpServer::add_cookie(dev::HttpServerSession* session, std::string data)
{
  std::string field = "";
  dev::HttpCookie cookie;
  std::vector<std::string> parts = dev::split(data, ';');

  if(parts.size() == 0) return;

  long chp = dev::find(parts[0], '=');
  if(chp < 0) field = parts[0];
  else
  {
    field = parts[0].substr(0, chp);
    cookie.value = parts[0].substr(chp + 1, parts[0].size());
  }

  dev::ipad(field);
  dev::ipad(cookie.value);

  for(size_t i = 1; i < parts.size(); i++)
  {
    chp = dev::find(parts[i], '=');
    if(chp < 0) continue;
    std::string query = parts[i].substr(0, chp);
    dev::ipad(query);
    dev::itolower(query);
    if(query == "path") cookie.path = parts[i].substr(chp + 1, parts[i].size());
  }

  dev::ipad(cookie.path);

  if(field.size() != 0) session->cookies[field] = cookie;
}

void dev::HttpServer::parse_post_queries(dev::HttpServerSession* session)
{
  if(session->method == POST)
  {
    if(dev::tolower(session->incoming_headers["content-type"]) == "application/x-www-form-urlencoded")
    {
      long length = atol(session->incoming_headers["content-length"].c_str());
      std::string data;
      data.reserve(length);
      for(long i = 0; i < length; i++)
      {
        length += (char) session->connection->get();
      }

      std::vector<std::string> queries = dev::split(data, '&');
      for(size_t i = 0; i < queries.size(); i++)
      {
        //Handle corrupt request
        if(queries[i].size() == 1) continue;

        std::string key;
        std::string value;

        long loc = dev::find(queries[i], '=');
        if(loc < 0)
        {
          key = queries[i];
          value = "";
        }
        else
        {
          key = queries[i].substr(0, loc);
          value = queries[i].substr(loc + 1, queries[i].size());
        }

        dev::ipad(key);
        dev::ipad(value);

        session->post[key] = dev::HttpQuery(POST, value);
        session->queries[key] = session->post[key];
      }
    }
  }
}

void dev::HttpServer::check_request(dev::HttpServerSession* session)
{
  if(session->proto == HTTP_1_1 && session->incoming_headers["host"].size() == 0)
  {
    throw dev::HttpServerException("Bad Request");
  }
}

void dev::HttpServer::prepare_session(dev::HttpServerSession* session)
{
  session->headers["content-type"] = "text/html";
  session->responseCode = 200;
  session->responseStatus = "OK";
}

void dev::HttpServer::check_session_response(dev::HttpServerSession* session)
{
  if(session->headers["content-type"].size() == 0) session->headers["content-type"] = "text/html";
  session->headers["content-length"] = dev::toString(session->response.size());
  session->headers["date"] = dev::getHTTPTimestamp();
}

void dev::HttpServer::send_response(dev::HttpServerSession* session)
{
  std::string line = "";
  line += session->proto == HTTP_1_0 ? "HTTP/1.0 " : "HTTP/1.1";
  line += dev::toString(session->responseCode);
  line += (" " + session->responseStatus + "\r\n");

  session->connection->put(line);

  for(std::map<std::string, std::string>::const_iterator it = session->headers.begin(); it != session->headers.end(); ++it)
  {
    session->connection->put(it->first + ": " + it->second + "\r\n");
  }

  for(std::map<std::string, dev::HttpCookie>::const_iterator it = session->cookies.begin(); it != session->cookies.end(); ++it)
  {
    //Make sure the path is set to a default of '/'
    if(session->cookies[it->first].path.size() == 0) session->cookies[it->first].path = "/";

    std::stringstream out;
    out << "set-cookie: " << it->first << "=" << it->second.value << "; path=" << it->second.path << "\r\n";
    session->connection->put(out.str().c_str());
  }

  session->connection->put("\r\n");
  session->connection->put(session->response);
  session->connection->put("\r\n");
}



//Sample request handler
void dev::HttpServer::on_request(dev::HttpServerSession& session)
{
  if(session.processed_path == "/cookie_add")
  {
    dev::HttpCookie cookie(session.queries["value"].value);
    session.cookies[session.queries["name"].value] = cookie;
  }
  generate_home_page(session);
}




//Exception stuffs


dev::HttpServerException::HttpServerException() :
  _message("HTTP Server Error"),
  _code(0)
{}

dev::HttpServerException::HttpServerException(const int code) :
  _message("HTTP Server Error"),
  _code(code)
{}

dev::HttpServerException::HttpServerException(const char* message) :
  _message(message),
  _code(0)
{}

dev::HttpServerException::HttpServerException(const int code, const char* message) :
  _message(message),
  _code(code)
{}

const char* dev::HttpServerException::what() const throw()
{
  return _message;
}

const int dev::HttpServerException::code()
{
  return _code;
}

dev::HttpServerException::~HttpServerException()
{}

