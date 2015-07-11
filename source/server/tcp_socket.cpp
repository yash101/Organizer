#include "tcp_socket.h"

#ifndef _WIN32
#define POSIX
#endif

//Include these files if the platform is POSIX
#ifdef POSIX
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif

#include <cstring>
#include <thread>

#include "../stringproc.h"

#define E_SOCK_CREATE_ERROR -1
#define E_SOCK_BIND_ERROR -2
#define E_BAD_ALLOC -3
#define E_BAD_WRITE -4
#define E_BAD_READ -5



#include <stdio.h>



dev::TcpServer::TcpServer() :
  _address_allocated(false)
{
  _address = NULL;
}

inline struct sockaddr_in* s(void* v)
{
  return (struct sockaddr_in*) v;
}

inline void* v(struct sockaddr_in* s)
{
  return (struct sockaddr_in*) s;
}

int dev::TcpServer::start(int port)
{
  //Set the port number of the class as the given one
  _port = port;

  //Create the socket
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  //Check to make sure socket creation was successful
  if(_fd < 0)
  {
    throw dev::TcpServerException(E_SOCK_CREATE_ERROR, "Unable to create socket");
  }

  //Allocate memory for the socket address structure and
  //initialize to zero
  try
  {
    _address = new struct sockaddr_in;
  }
  catch(std::bad_alloc& e)
  {
    throw dev::TcpServerException(E_BAD_ALLOC, "Unable to allocate memory for the address structre");
  }
  _address_allocated = true;

  bzero((char*) _address, sizeof(struct sockaddr_in));

  //Populate the server address structure with important information
  s(_address)->sin_family = AF_INET;
  s(_address)->sin_addr.s_addr = htonl(INADDR_ANY);
  s(_address)->sin_port = htons(_port);

  //Bind the socket to the address/port
  if(bind(_fd, (struct sockaddr*) _address, sizeof(struct sockaddr_in)) < 0)
  {
    throw dev::TcpServerException(E_SOCK_BIND_ERROR, "Unable to bind to socket");
  }

  //Begin listening for incoming connections. The queue depth is 3
  listen(_fd, 3);

  //Infinitely wait for an incoming connection and accept.
  while(true)
  {
    dev::TcpServerSession* session;
    //Allocate a new session
    try
    {
      session = new dev::TcpServerSession;
    }
    catch(std::bad_alloc& e)
    {
      //We were unable to allocate memory. Wait 10 ms before retrying. Hopefully another
      //server thread or something quit by now.
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    //Just a temporary var really
    int c = sizeof(struct sockaddr_in);

    //Accept the new connection
    session->_fd = accept(_fd, (struct sockaddr*) session->_address, (socklen_t*) &c);

    //Check for accept failures
    if(session->_fd < 0)
    {
      //Prevent a memory leak
      delete session;
      //Retry
      continue;
    }

    //Handle the connection. Have fun :)
    std::thread(&dev::TcpServer::listenerProxy, this, session).detach();
  }
}

void dev::TcpServer::listenerProxy(dev::TcpServerSession* connection)
{
  try
  {
    //Let the user play with the connection
    worker(connection);
  }
    //Catch any exceptions incurred by the worker proces to allow garbage collection
  catch(std::exception& e)
  {
    delete connection;
  }

  //Deallocate the connection structure. We hate runaway RAM ;)
  delete connection;
}

void dev::TcpServer::worker(dev::TcpServerSession* connection)
{
  connection->put("Hello World!\nYour TCP server works! Now, create a class which inherits off dev::TcpServer and implement your own worker function!\n");
  std::this_thread::sleep_for(std::chrono::seconds(10));
}

dev::TcpServer::~TcpServer()
{
  //Memory management -- deallocate RAM if it is not being used.
  if(_address_allocated)
  {
    struct sockaddr_in* ad = (struct sockaddr_in*) _address;
    delete ad;
  }
  close(_fd);
}


//!----------Session functions. Random other clutter----------



dev::TcpServerSession::TcpServerSession()
{
  //Allocate
  _address = (void*) new struct sockaddr_in;
}

dev::TcpServerSession::~TcpServerSession()
{
  //Deallocate
  struct sockaddr_in* sock = (struct sockaddr_in*) _address;
  delete sock;
  shutdown(_fd, SHUT_WR);
  close(_fd);
}

int dev::TcpServerSession::put(char byte)
{
  int o = write(_fd, &byte, sizeof(byte));
  if(o < 0) throw dev::TcpServerException(E_BAD_WRITE, "Unable to write to socket");
  return o;
}

int dev::TcpServerSession::put(std::string string)
{
  int o = write(_fd, string.c_str(), string.size());
  if(o < 0) throw dev::TcpServerException(E_BAD_WRITE, "Unable to write to socket");
  return 0;
}

char dev::TcpServerSession::read()
{
  char x;
  if(::read(_fd, (void*) &x, sizeof(char)) < 0) throw dev::TcpServerException(E_BAD_READ, "Unable to read from socket");
  return (int) x;
}

std::string dev::TcpServerSession::read(int maxlen)
{
  char* str = new char[maxlen + 1];
  str[maxlen + 1] = '\0';
  if(::read(_fd, (void*) str, maxlen) < 0)
  {
    delete[] str;
  }
  std::string x = dev::toString(str);
  delete[] str;
  return x;
}

std::string dev::TcpServerSession::readline(char end)
{
  std::string out;
  try
  {
    while(true)
    {
      char c = read();
      if(c == end)
      {
        return out;
      }
      else
      {
        out += c;
      }
    }

    return out;
  }
  catch(std::exception& e)
  {
    return out;
  }
}

std::string dev::TcpServerSession::readline(std::string end)
{
  std::string out;
  try
  {
    while(true)
    {
      if(out.size() >= end.size())
      {
        std::string tmp = out.substr(out.size() - end.size(), out.size());
        if(out.substr(out.size() - end.size(), out.size()) == end)
        {
          return out.substr(0, out.size() - end.size());
        }
      }
      out += (char) read();
    }
  }
  catch(std::exception& e)
  {
    return out;
  }
}

dev::TcpServerException::TcpServerException() :
  _code(0),
  _message("Server Error")
{}

dev::TcpServerException::TcpServerException(int code) :
  _code(code),
  _message("Server Error")
{}

dev::TcpServerException::TcpServerException(const char* message) :
  _code(0),
  _message(message)
{}

dev::TcpServerException::TcpServerException(int code, const char* message) :
  _code(code),
  _message(message)
{}

const int dev::TcpServerException::code()
{
  return _code;
}

const char* dev::TcpServerException::what() const throw()
{
  return _message;
}

dev::TcpServerException::~TcpServerException()
{}
