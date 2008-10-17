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

#include "Socket.h"

namespace lulzHTTPd {

namespace System {

Socket::Socket (const String& address, int port, int maxConnections)
{
    _reuse = 0;

    _sd = System::socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &_reuse, sizeof(_reuse));

    if (_sd < 0) {
        throw Exception(Exception::SOCKET_CREATION);
    }

    _bind(address, port);
    _listen(maxConnections);
}

Socket::Socket (const Socket& socket)
{
    _reuse = 0;

    sockaddr remote;
    socklen_t len = sizeof(remote);

    _sd = System::accept(socket._sd, &remote, &len);

    if (_sd < 0) {
        throw Exception(Exception::SOCKET_ACCEPT);
    }
}

Socket::~Socket (void)
{
    this->close();
}

Socket*
Socket::accept (void)
{
    return new Socket(*this);
}

String
Socket::recv (size_t length)
{
    char* buffer = new char[length+1];

    int n = read(_sd, buffer, length);
    if (n < 0) {
        throw Exception(Exception::SOCKET_READ);
    }
    buffer[n] = '\0';

    String nBuffer = buffer;
    delete[] buffer;
    return nBuffer;
}

String
Socket::readLine (void)
{
    String nText;
    
    char ch;
    while ((ch = recv(1).toChars()[0]) != '\n' && ch != '\0' && ch != EOF) {
        if (ch != '\r') {
            nText += ch;
        }
    }

    return nText;
}

int
Socket::send (String string)
{
    int n = 0;

    while ((n = write(_sd, string.substr(n).toChars(), string.length())) < string.length()) {
        if (n < 0) {
            throw Exception(Exception::SOCKET_WRITE);
        }
    }

    return n;
}

void
Socket::close (void)
{
    if (::close(_sd) < 0 && errno != EBADF) {
        throw Exception(Exception::SOCKET_CLOSE);
    }
}

Socket&
Socket::operator << (const char* string)
{
    this->send(String(string));
}

Socket&
Socket::operator << (const std::string& string)
{
    this->send(String(string));
}

Socket&
Socket::operator << (const String& string)
{
    this->send(string);
}

Socket&
Socket::operator >> (char* buffer)
{
    buffer = (char*) this->recv().toChars();
}

Socket&
Socket::operator >> (std::string buffer)
{
    buffer = this->recv().toString();
}

Socket&
Socket::operator >> (String buffer)
{
    buffer = this->recv();
}

Socket::operator int (void)
{
    return _sd;
}

void
Socket::_bind (const String& addr, int port)
{
    in_addr_t nAddr = this->_toIPv4((String&) addr);

    if (nAddr == INADDR_NONE) {
        throw Exception(Exception::SOCKET_BIND);
    }

    sockaddr local = this->_initAddr(nAddr, port);

    if (System::bind(_sd, &local, sizeof(local)) < 0) {
        throw Exception(Exception::SOCKET_BIND);
    }
}

void
Socket::_listen (int maxConnections)
{
    if (System::listen(_sd, maxConnections) < 0) {
        throw Exception(Exception::SOCKET_LISTEN);
    }
}

void
Socket::setBlocking (bool blocking)
{
    long flags = fcntl(_sd, F_GETFL);
    if (fcntl(_sd, F_SETFL, (blocking? flags | O_NONBLOCK : flags & ~O_NONBLOCK)) < 0) {
        throw Exception(Exception::SOCKET_ERROR_SET_BLOCK);
    }
}

in_addr_t
Socket::_toIPv4 (String& addr)
{
    in_addr_t nAddr;

    if (this->_isValidIPv4(addr)) {
        nAddr = System::inet_addr(addr.toChars());
    }
    else {
        hostent *he;
        he = System::gethostbyname(addr.toChars());

        if (he == NULL) {
            nAddr = INADDR_NONE;
        }
        else {
            nAddr = ((struct in_addr *) he->h_addr)->s_addr;
        }
    }

    return nAddr;
}

bool
Socket::_isValidIPv4 (String& addr)
{
    Regex ip("/^((\\d+)\\.){3}(\\d+)$/");

    if (ip.match(addr.toString())) {
        for (int i = 0; i < 4; i++) {
            if (String(ip.group(i+1)).toInt() > 255) {
                return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}

struct sockaddr
Socket::_initAddr (in_addr_t addr, int port)
{
    sockaddr_in nAddr;

    nAddr.sin_family      = AF_INET;
    nAddr.sin_port        = htons(port);
    nAddr.sin_addr.s_addr = addr;

    return *(struct sockaddr*) &nAddr;
}

}

}

