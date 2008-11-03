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

#include "Exception.h"

namespace lulzHTTPd {

Exception::Exception (int code)
{
    switch (code) {
        case SERVER_NOT_INITED:
        _description = "The server has not been initialized.";
        break;

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

        case SOCKET_ERROR_SET_BLOCK:
        _description = "Couldn't change the socket's block state.";
        break;

        default:
        _description = "Well, you did it wrong.";
        break;
    }
}

Exception::~Exception (void) throw()
{
}

const char*
Exception::what (void) throw()
{
    return _description.toChars();
}

}

