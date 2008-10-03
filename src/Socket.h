#if !defined(LULZHTTPD_SOCKET_H)
#define LULZHTTPD_SOCKET_H

namespace System {

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

class Socket
{
  public:
    static int RECV_BUFSIZE = 1024;

  public:
    Socket (int domain, int protocol = 0);

    void bind (const char* addr, int port);
    void bind (const std::string& addr, int port);
    void bind (const String& addr, int port);

    void bind (in_addr_t addr, int port);

    void listen (int maxConnections);

  private:
    int _sd;
    bool _bound;

  private:
    in_addr_t _toIPv4 (const String& addr);
    bool _isValidIPv4 (const String& addr);
};

};

#endif

