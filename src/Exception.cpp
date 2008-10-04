#include "Exception.h"

Exception::Exception (int code)
{
    switch (code) {
        case SOCKET_CREATION:
        _description = "socket() failed.";
        break;

        case SOCKET_LISTEN:
        _description = "listen() failed.";
        break;

        case SOCKET_ACCEPT:
        _description = "accept() failed.";
        break;

        case SOCKET_BIND:
        _description = "bind() failed.";
        break;

        case SOCKET_READ:
        _description = "read() failed.";
        break;

        case SOCKET_WRITE:
        _description = "write() failed.";
        break;

        case SOCKET_CLOSE:
        _description = "close() failed.";
        break;
    }
}

const char*
Exception::getMessage() throw()
{
    return _description.toChars();
}

