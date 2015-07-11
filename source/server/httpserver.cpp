#include "httpserver.h"
#include "../stringproc.h"
void dev::HttpServer::worker(dev::TcpServerSession* connection)
{
  //Create a session object
  dev::HttpServerSession session;

  //Set the connection parameter of the session
  session.connection = connection;

  //Read the first request line
  //-->GET / HTTP/1.0
  std::string line = session.connection->readline("\r\n");
  std::vector<std::string> parts = dev::split(line, ' ');

  //A micro-optimization by inlining the code instead of using a jump
  session.method = parts[0] == "POST" ? POST : GET;

  //Set the incoming path
  session.path = parts[1];

  std::vector<std::string> pathparts = dev::split(session.path, '?');

  //Set the path as the text before the '?'
  session.processed_path = pathparts[0];

  //Parse the GET queries
  std::string qstring = pathparts[1].substr(1, pathparts[1].size());
  //Split the query string by the ampersands (&)
  std::vector<std::string> queryparts = dev::split(qstring, '&');
  //Iterate through and process each query
  for(size_t i = 0; i < queryparts.size(); i++)
  {
    std::string key;
    std::string value;

    long ch = dev::find(queryparts[i], '=');
    if(ch < 0)
    {
      key = queryparts[i];
      value = "";
    }

    key = queryparts[i].substr(0, ch - 1);
    value = queryparts[i].substr(ch + 1, queryparts[i].size());

    dev::ipad(key);
    dev::ipad(value);

    //Store the values
    session.queries[key] = dev::HttpQuery(GET, value);
    session.get[key] = dev::HttpQuery(GET, value);
  }

  //Set the protocol type
  if(parts[2] == "HTTP/1.0") session.proto = HTTP_1_0;
  else if(parts[2] == "HTTP/1.1") session.proto = HTTP_1_1;
  else session.proto = PROTO_UNKNOWN;

  //Download the headers from the client and save them
  while(dev::pad(line) != "")
  {
    line = session.connection->readline("\r\n");
    long ch = dev::find(line, ':');
    if(ch < 0) continue;
    std::string field = line.substr(0, ch - 1);
    std::string value = line.substr(ch + 1, line.size());
    dev::ipad(field);
    dev::ipad(value);
    dev::itolower(field);
    session.incoming_headers[field] = value;
  }

  if(session.method == POST)
  {
    std::string data = "";
    if(dev::tolower(session.incoming_headers["content-type"]) == "application/x-www-form-urlencoded")
    {
      long length = atol(session.incoming_headers["content-length"].c_str());
      data.reserve(length);
      for(long i = 0; i < length; i++)
      {
        data += session.connection->read();
      }
    }

    //Split the data by the ampersand
    queryparts = dev::split(data, '&');
    for(size_t i = 0; i < queryparts.size(); i++)
    {
      long ch = dev::find(queryparts[i], '=');
      std::string key = "";
      std::string value = "";
      if(ch < 0) key = queryparts[i];
      else
      {
        key = queryparts[i].substr(0, ch - 1);
        value = queryparts[i].substr(ch + 1, queryparts[i].size());
      }
      dev::ipad(key);
      dev::ipad(value);
      session.queries[key] = dev::HttpQuery(POST, value);
      session.post[key] = dev::HttpQuery(POST, value);
    }
  }

  //Validate HOST present if HTTP 1.1
  if(session.proto == HTTP_1_1)
  {
    if(session.incoming_headers["host"].size() == 0)
    {
      session.connection->put("HTTP/1.1 400 Bad Request\r\n");
      return;
    }
  }

  session.responseCode = 200;
  session.responseStatus = "OK";
  session.headers["content-type"] = "text/html";

  on_request(session);

  std::string buf = session.proto == HTTP_1_1 ? "HTTP/1.1 " : "HTTP/1.0 " + dev::toString(session.responseCode) + " " + session.responseStatus + "\r\n";
  session.connection->put(buf);
  for(std::map<std::string, std::string>::const_iterator it = session.headers.begin(); it != session.headers.end(); ++it)
  {
    session.connection->put(it->first + ": " + it->second + "\r\n");
  }
  session.connection->put("content-length: " + dev::toString(session.response.size()) + "\r\n\r\n");
  session.connection->put(session.response);
}

void dev::HttpServer::on_request(dev::HttpServerSession& session)
{}
