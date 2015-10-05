#include "server/server.h"
#include <cppconn/driver.h>

bool serve_other(server::HttpServerSession& session)
{
  session.Response.ftype = fopen("resources/index.html", "r");
  session.Response.type = server::FILE;

  if(session.Response.ftype == NULL)
    return AppServer.handle404(session);
  return true;
}

void server_setup()
{
  AppServer.set_static("/(.*)", "resources");
  AppServer.errorHandler404 = serve_other;
}
