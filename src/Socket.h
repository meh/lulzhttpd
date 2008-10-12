/****************************************************************************
* This file is part of lulzHTTPd.                                           *
* Copyleft meh.                                                             *
*                                                                           *
* lulzHTTPd is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzHTTPd is distributed in the hope that it will be useful.              *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzHTTPd.  If not, see <http://www.gnu.org/licenses/>.        *
****************************************************************************/

#if !defined(LULZHTTPD_SOCKET_H)
#define LULZHTTPD_SOCKET_H

#include "common.h"

namespace lulzHTTPd {

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

    void setBlocking (bool state);

    String recv (int size = RECV_BUFSIZ);
    String readLine (void);

    int send (String string);

    void close (void);

  public:
    operator int (void);

    Socket& operator << (const char* string);
    Socket& operator << (const std::string& string);
    Socket& operator << (const String& string);

    Socket& operator >> (char* buffer);
    Socket& operator >> (std::string buffer);
    Socket& operator >> (String buffer);

  private:
    int _sd;
    int _reuse;

  private:
    void _bind (const String& addr, int port);
    void _listen (int maxConnections);

    in_addr_t _toIPv4 (String& addr);
    bool _isValidIPv4 (String& addr);
    struct sockaddr _initAddr (in_addr_t addr, int port);
};

};

};

#endif

