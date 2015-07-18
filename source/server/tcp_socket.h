#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include <string>
#include <exception>
#include <vector>
namespace dev
{
  //@"description": "A multi-threaded TCP socket server"
  class TcpServer;
  //@"description": "A session for a the multithreaded socket server"
  class TcpServerSession;
  //@"description": "Exception class for TCP Socket Server
  class TcpServerException : public std::exception
  {
  private:
    int _code;
    const char* _message;
  public:
    TcpServerException();
    TcpServerException(int code);
    TcpServerException(const char* message);
    TcpServerException(int code, const char* message);

    const int code();
    virtual const char* what() const throw();
    virtual ~TcpServerException();
  };

  class TcpServer
  {
  private:
    int _fd;
    int _port;
    void* _address;
    bool _address_allocated;
    void listener();
    void listenerProxy(dev::TcpServerSession* connection);
    virtual void worker(dev::TcpServerSession* connection);
  public:
    TcpServer();
    ~TcpServer();
    int start(int port);
  };

  class TcpServerSession
  {
  private:
    bool _socketClosed;
  public:
    int _fd;
    void* _address;
    dev::TcpServer* server;
    
    TcpServerSession();
    ~TcpServerSession();

    int put(char byte);
    int put(std::string string);
    char get();
    std::string get(int mxlength);
    std::string readLine(char end);
    std::string readLine(std::string end);
    std::string readLine(std::vector<std::string> end);

    void closeSocket();
  };
}
#endif
