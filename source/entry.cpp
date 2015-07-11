//@"fn": "entry.cpp"
//@"author": "yash101 (Dev)"
//@"description": "This file serves as the entry point for the program. Once the program is launched, the function, main() is called."

//This is the header for entry.cpp. It contains the variable extern definitions so that
//The main variables in this file can be accessed elsewhere.
#include "entry.h"
#include "server/tcp_socket.h"
#include "server/httpserver.h"

class H : public dev::HttpServer
{
private:
  void on_request(dev::HttpServerSession& session)
  {
    session.response = "<html><body><h1>Hi!</h1></body></html>";
  }
};

int main(int argc, char** argv)
{
  //Assign the incoming arguments to our arguments' structure
  Arguments::argc = argc;
  Arguments::argv = argv;
  H().start(1234);
}

int Arguments::argc;
char** Arguments::argv;
