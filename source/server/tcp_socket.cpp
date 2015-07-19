#include "tcp_socket.h"

#ifndef _WIN32
#define POSIX
#endif

//Include these files if the platform is POSIX
#ifdef POSIX

//Provides access to functions relating file descriptors
#include <unistd.h>
//Provides a few useful functions relating to endianness conversion
#include <arpa/inet.h>
//Provides some vital strutures
#include <sys/types.h>
//Provides some vital things for the socket server
#include <netinet/in.h>

//Do this stuff if Windows (WinSock)
#else

//Link WinSock
#pragma comment(lib, "Ws2_32.lib")
//Include the Windows sockets libraries
#include <Windows.h>
#include <winsock.h>
//WinSock initialization data
WSADATA wsadata;
WORD wVersionRequested = NULL;
int wsaerr = 0;

#endif

//Useful string functions (memset, etc)
#include <string.h>
//Threading
#include <thread>
//String processing/miscellaneous functions
#include "../stringproc.h"

//A few error code definitions
#define E_SOCK_CREATE_ERROR -1
#define E_SOCK_BIND_ERROR -2
#define E_BAD_ALLOC -3
#define E_BAD_WRITE -4
#define E_BAD_READ -5

//Initializer fot the TCP server
dev::TcpServer::TcpServer() :
  _address_allocated(false)
{
#ifdef _WIN32
  //Initialize Windows sockets (Yeah, I know! They are a pain in the a$$!)
  if(wVersionRequested == NULL)
  {
    wVersionRequested = MAKEWORD(2, 2);
    //Start up WSA
    wsaerr = WSAStartup(wVersionRequested, &wsadata);
    if(wsaerr != 0)
    {
      fprintf(stderr, "Unable to find winsock!\n");
      exit(EXIT_FAILURE);
    }
  }
#endif
  //Initialization of the address pointer.
  _address = NULL;
}

//Converts void* to struct sockaddr_in* (cosmetic)
inline struct sockaddr_in* s(void* v)
{
  return (struct sockaddr_in*) v;
}

//Converts struct sockaddr_in* to void* (cosmetic)
inline void* v(struct sockaddr_in* s)
{
  return (struct sockaddr_in*) s;
}

//This function starts the server and begins listening.
int dev::TcpServer::start(int port)
{
  //Set the port number of the class as the given one
  _port = port;
  //Create the socket
  _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  //Check to make sure socket creation was successful
#ifndef _WIN32
  if(_fd < 0)
#else
  if(_fd == WSANOTINITIALISED)
#endif
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
  memset((char*) _address, 0, sizeof(struct sockaddr_in));

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
    //Pointe to a session object (not initialized yet)
    dev::TcpServerSession* session;
    //Allocate a new session
    try
    {
      session = new dev::TcpServerSession;
    }
    catch(std::bad_alloc& e)
    {
      //We were unable to allocate memory. Wait 10 ms before retrying. Hopefully another
      //server thread or something will have quit by now.
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    //Just a temporary var really
    int c = sizeof(struct sockaddr_in);

    //Accept the new connection
#ifndef _WIN32
    session->_fd = accept(_fd, (struct sockaddr*) session->_address, (socklen_t*) &c);
#else
    session->_fd = accept(_fd, (struct sockaddr*) session->_address, &c);
#endif
    //Check for accept failures
    if(session->_fd < 0)
    {
      //Prevent a memory leak
      delete session;
      //Retry
      continue;
    }

    //Set the pointer to the server in the TCP socket session object so that the user
    //has external access to this object
    session->server = this;

    //Handle the connection. Have fun :)
    std::thread(&dev::TcpServer::listenerProxy, this, session).detach();
  }
}

void dev::TcpServer::listenerProxy(dev::TcpServerSession* connection)
{
  //To catch any exceptions encountered. If any exceptions get past, the program will crash
  // (BAD). Note that failure is very common because an exception is thrown whenever there is
  // a connection error!
  try
  {
    //Let the user play with the connection
    worker(connection);
  }
  catch(std::exception& e)
  {}

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
#ifndef _WIN32
  close(_fd);
#else
  closesocket(_fd);
#endif
}

//!----------Session functions. Random other clutter----------
dev::TcpServerSession::TcpServerSession() :
  _socketClosed(false)
{
  //Allocate space for an address structure
  _address = (void*) new struct sockaddr_in;
}

dev::TcpServerSession::~TcpServerSession()
{
  //Deallocate
  struct sockaddr_in* sock = (struct sockaddr_in*) _address;
  delete sock;
  //Check to make sure the socket has not been closed
  if(!_socketClosed)
  {
    //Shutdown socket first (Allow writes), then close the socket.
#ifdef _WIN32
    shutdown(_fd, 1);
    closesocket(_fd);
#else
    shutdown(_fd, SHUT_WR);
    close(_fd);
#endif
    //Set the flag that the socket has been closed to prevent an fd error
    _socketClosed = true;
  }
}

int dev::TcpServerSession::put(char byte)
{
#ifndef _WIN32
#ifdef __MACH__
  int o = ::send(_fd, &byte, 1, SO_NOSIGPIPE);
#else
  int o = ::send(_fd, &byte, 1, MSG_NOSIGNAL);
#endif
#else
  int o = ::send(_fd, &byte, 1, 0);
#endif
  if(o <= 0) throw dev::TcpServerException(E_BAD_WRITE, "Unable to write to socket");
  return o;
}

int dev::TcpServerSession::put(std::string string)
{
#ifndef _WIN32
#ifdef __MACH__
  int o = ::send(_fd, string.c_str(), string.size(), SO_NOSIGPIPE);
#else
  int o = ::send(_fd, string.c_str(), string.size(), MSG_NOSIGNAL);
#endif
#else
  int o = ::send(_fd, string.c_str(), string.size(), 0);
#endif
  if(o <= 0) throw dev::TcpServerException(E_BAD_WRITE, "Unable to write to socket");
  return o;
}

char dev::TcpServerSession::get()
{
  int o = 0;
  char ch;
  //Try to read until we read 1 character
  while((o = ::recv(_fd, &ch, 1, 0)) < 1)
  {
    if(o <= 0) throw dev::TcpServerException(E_BAD_READ, "Unable to read from socket");
  }
  return ch;
}

std::string dev::TcpServerSession::get(int mxlength)
{
  //Allocate space for a C String
  char* ch = new char[mxlength + 1];
  //Read some data
  int o = ::recv(_fd, ch, mxlength, 0);

  if(o <= 0)
  {
    delete[] ch;
    throw dev::TcpServerException(E_BAD_READ, "Unable to read from socket");
  }

  //Place the NULL terminator (for safety and prevention of ugly SEGFAULTS!!!)
  ch[o] = '\0';
  std::string ret = dev::toString(ch);
  //GC
  delete[] ch;
  return ret;
}

std::string dev::TcpServerSession::readLine(char end)
{
  std::stringstream str;
  while(true)
  {
    char ch;
    int x = ::recv(_fd, &ch, 1, 0);
    if(ch == end) return str.str();
    if(x < 0) return str.str();
    str << ch;
  }
}

std::string dev::TcpServerSession::readLine(std::string end)
{
  std::string line = "";
  while(true)
  {
    if(line.size() >= end.size())
    {
      if(line.substr(line.size() - (end.size() - 1), line.size() - 1) == end)
      {
        break;
      }
    }
    line += (char) get();
  }
  return line = line.substr(0, line.size() - (end.size() + 1));
}

std::string dev::TcpServerSession::readLine(std::vector<std::string> end)
{
  if(end.size() == 0) return "";

  size_t len = end[0].size();
  for(size_t i = 0; i < end.size(); i++)
  {
    if(end[i].size() != len) return "";
  }

  std::string line = "";
  while(true)
  {
    if(line.size() >= len)
    {
      for(size_t i = 0; i < end.size(); i++)
      {
        std::string x = line.substr(line.size() - (len - 1) - 1, line.size());
        if(x == end[i])
        {
          return line.substr(0, line.size() - len);
        }
      }
    }
    line += (char) get();
  }
}

void dev::TcpServerSession::closeSocket()
{
  if(!_socketClosed)
  {
#ifdef _WIN32
    shutdown(_fd, 1);
    close(_fd);
#else
    shutdown(_fd, SHUT_WR);
    close(_fd);
#endif
    _socketClosed = true;
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
