#include "ApplicationServer.h"
#include "stringproc.h"
#include "entry.h"
#include "defaults.h"
#include <regex>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

bool ApplicationServer::static_handler(server::HttpServerSession& session, std::string addbasepath)
{
  std::string refpth = addbasepath + "/" + session.Path;
  std::vector<std::string> parts = server::split(refpth, '/');
  bool directory = false;

  std::string path;
  for(size_t i = 0; i < parts.size(); i++)
  {
    if(parts[i].size() == 0 || parts[i] == "/" || parts[i] == "../" || parts[i] == "./" || parts[i] == ".")
      continue;

    path += parts[i] + "/";

    struct stat st;
    stat(path.substr(0, path.size() - 1).c_str(), &st);
    if(S_ISREG(st.st_mode))
    {
      directory = false;
      break;
    }
    else if(S_ISDIR(st.st_mode))
    {
      directory = true;
    }
  }

  if(path.size() > 0)
    path.pop_back();

  if(!directory)
  {
    if((session.Response.ftype = fopen(path.c_str(), "r")) != NULL)
    {
      session.Response.type = server::FILE;
      session.ResponseStatusCode = 200;

      size_t pos = path.find_last_of('.');
      if(pos == std::string::npos || path.back() == '.')
        session.headers["content-type"] = "text/html";
      else
        session.headers["content-type"] = mime_server[path.substr(pos + 1, path.size())].http_mime;

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    std::vector<std::string> indices = server::split(server::configuration()["indices"], ',');
    if(path.back() != '/') path.append("/");
    for(size_t i = 0; i < indices.size(); i++)
    {
      std::string npath = path + indices[i];
      if((session.Response.ftype = fopen(npath.c_str(), "r")) != NULL)
      {
        session.Response.type = server::FILE;
        session.ResponseStatusCode = 200;

        size_t pos = indices[i].find_last_of('.');
        if(pos == std::string::npos || npath.back() == '.')
          session.headers["content-type"] = "text/html";
        else
          session.headers["content-type"] = mime_server[npath.substr(pos + 1, npath.size())].http_mime;

        return true;
      }
    }

    return false;
  }
}
