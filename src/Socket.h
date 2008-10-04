#if !defined(LULZHTTPD_SOCKET_H)
#define LULZHTTPD_SOCKET_H

#include "common.h"

namespace System {

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

class Socket
{
  public:
    static const int RECV_BUFSIZ = 1024;

  public:
    Socket (const String& address, int port, int maxConnections);
    Socket (const Socket& socket);

    ~Socket (void);

    Socket* accept (void);

    String recv (void);
    int send (String string);

    void close (void);

    Socket& operator << (const char* string);
    Socket& operator << (const std::string& string);
    Socket& operator << (const String& string);

    Socket& operator >> (char* buffer);
    Socket& operator >> (std::string buffer);
    Socket& operator >> (String buffer);

  private:
    int _sd;
    bool _bound;

  private:
    void _bind (const String& addr, int port);

    void _listen (int maxConnections);

    in_addr_t _toIPv4 (String& addr);
    bool _isValidIPv4 (String& addr);
    struct sockaddr _initAddr (in_addr_t addr, int port);
};

};

#endif

