#include "Socket.h"

namespace System {

Socket::Socket (int domain, int protocol)
{
    _sd = ::socket(AF_INET, domain, protocol);

    if (_sd < 0) {
        throw std::exception();
    }

    _bound = false;
}

Socket::~Socket (void)
{
    this->close();
}

void
Socket::bind (const char* addr, int port)
{
    this->bind((String) addr, port);
}

void
Socket::bind (const std::string& addr, int port)
{
    this->bind((String) addr, port);
}

void
Socket::bind (const String& addr, int port)
{
    this->bind(this->_toIPv4(addr), port);
}

void
Socket::bind (in_addr_t addr, int port)
{
    if (addr == INADDR_NONE) {
        throw std::exception;
    }

    sockaddr local = __initAddr(addr);

    if (::bind(_sd, &local, sizeof(local)) < 0) {
        throw std::exception;
    }

    _bound = true;
}

in_addr_t
Socket::_toIPv4 (const String& addr)
{
    in_addr_t nAddr;

    if (this->_isValidIPv4(addr)) {
        nAddr = ::inet_addr(addr.toChars());
    }
    else {
        hostent he;
        he = ::gethostbyname(addr.toChars());

        nAddr = ((in_addr)he->h_addr).s_addr;
    }

    if (nAddr == NULL) {
        nAddr = INADDR_NONE;
    }

    return nAddr;
}

bool
Socket::_isValidIPv4 (const String& addr)
{
    Regex ip("/^((\\d+)\.){3}(\\d+)$/");

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

sockaddr
Soket::_initAddr (in_addr_t addr, int port)
{
    sockaddr_in nAddr;

    nAddr.sin_family      = AF_INET;
    nAddr.sin_port        = ::htons(port);
    nAddr.sin_addr.s_addr = addr;

    return (sockaddr*) &nAddr;
}

};

