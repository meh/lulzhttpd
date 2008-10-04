#if !defined(LULZHTTPD_EXCEPTION_H)
#define LULZHTTPD_EXCEPTION_H

#include "common.h"

class Exception : public std::exception
{
  public:
    static const int SOCKET_CREATION = 20;
    static const int SOCKET_LISTEN   = 21;
    static const int SOCKET_ACCEPT   = 22;
    static const int SOCKET_BIND     = 23;
    static const int SOCKET_READ     = 24;
    static const int SOCKET_WRITE    = 25;
    static const int SOCKET_CLOSE    = 26;

  public:
    Exception (int code);

    const char* getMessage() throw();

  private:
    String _description;
};

#endif
