#include "Socket.h"

namespace System {

Socket::Socket (const String& address, int port, int maxConnections)
{
    _sd = System::socket(AF_INET, SOCK_STREAM, 0);

    if (_sd < 0) {
        throw Exception(Exception::SOCKET_CREATION);
    }

    _bound = false;

    this->_bind(address, port);
    this->_listen(maxConnections);
}

Socket::Socket (const Socket& socket)
{
    sockaddr remote;
    socklen_t len = sizeof(remote);

    _sd = System::accept(socket._sd, &remote, &len);

    if (_sd < 0) {
        throw Exception(Exception::SOCKET_ACCEPT);
    }

    _bound = true;
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
Socket::recv (void)
{
    int n;
    int length = RECV_BUFSIZ;
    char* buffer = NULL;
    
    buffer = (char*) realloc(buffer, length+1);

    while ((n = read(_sd, buffer, RECV_BUFSIZ)) > 0) {
        if (n < RECV_BUFSIZ) {
            buffer[n] = '\0';
            break;
        }

        length += RECV_BUFSIZ;
        buffer = (char*) realloc(buffer, length+1);
    }

    if (n == -1) {
        throw Exception(Exception::SOCKET_READ);
    }

    String nBuffer = buffer;
    free(buffer);

    return nBuffer;
}

int
Socket::send (String string)
{
    int n;

    n = write(_sd, string.toChars(), string.length());
    if (n < 0) {
        throw Exception(Exception::SOCKET_WRITE);
    }

    return n;
}

void
Socket::close (void)
{
    if (::close(_sd) < 0 && errno != EBADF) {
        throw Exception(Exception::SOCKET_CLOSE);
    }

    _bound = false;
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

    _bound = true;

}

void
Socket::_listen (int maxConnections)
{
    if (System::listen(_sd, maxConnections) < 0) {
        throw Exception(Exception::SOCKET_LISTEN);
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
    nAddr.sin_port        = System::htons(port);
    nAddr.sin_addr.s_addr = addr;

    return *(struct sockaddr*) &nAddr;
}

};

