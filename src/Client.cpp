/****************************************************************************
* This file is part of lulzHTTPd.                                           *
* Copyleft meh.                                                             *
*                                                                           *
* lulzHTTPd is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzHTTPd is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzHTTPd.  If not, see <http://www.gnu.org/licenses/>.        *
****************************************************************************/

#include "Client.h"

namespace lulzHTTPd {

Client::Client (System::Socket* socket)
{
    _socket = socket;
}

Client::~Client (void)
{
    delete _socket;
}

void
Client::start (void)
{
    std::cerr << _socket->readLine().toString() << std::endl;
    return;

    HTTP* request = new HTTP;
    while (!request->done()) {
        request->parse(_socket->readLine().toString());
    }

    HTTP* response = new HTTP;
    if (request->isOk()) {

    }
    else {
        std::stringstream resp;
        resp << "FAGGOT IT'S A BAD REQUEST" << std::endl;

        std::stringstream number;
        number << resp.str().length();

        response->status(request->status());
        response->setHeader("Connection", "close");
        response->setHeader("Content-Type", "text/plain");
        response->setHeader("Content-Length", number.str());
        response->setHeader("Server", "lulzHTTPd/0.1");

        response->setData(resp.str());
    }

    delete request;
    delete response;
}

};
