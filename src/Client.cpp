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
    HTTP* request = new HTTP;
    while (!request->done()) {
        std::string string = _socket->readLine().toString();
        request->request(string);
    }

    HTTP* response = new HTTP;
    if (request->isOk()) {
        try {
            response->setData(System::readFile(
                Config::get("directories->document[path]")+System::normalizePath(request->getUri())
            ));

            response->setStatus(200);
            response->setVersion(request->getVersion());

            response->setHeader("Connection", "close");
            response->setHeader("Content-Type", Mime::getType(System::getExtension(request->getUri())));
            response->setHeader("Content-Length", response->getData().length());
            response->setHeader("Server", "lulzHTTPd/0.1");

            _socket->send(response->get());
        }
        catch (Exception e) {
            std::stringstream resp;
            resp << "WUT, ARE YOU BLIND? :O" << std::endl;

            response->setStatus(404);
            response->setVersion(request->getVersion());

            response->setHeader("Connection", "close");
            response->setHeader("Content-Type", "text/plain");
            response->setHeader("Content-Length", String((int)resp.str().length()).toString());
            response->setHeader("Server", "lulzHTTPd/0.1");

            response->setData(resp.str());

            _socket->send(response->get());
        }
    }
    else {
        std::stringstream resp;
        resp << "FAGGOT IT'S A BAD REQUEST" << std::endl;

        response->setStatus(request->getStatus());
        response->setVersion(request->getVersion() ? request->getVersion() : 1.0);

        response->setHeader("Connection", "close");
        response->setHeader("Content-Type", "text/plain");
        response->setHeader("Content-Length", String((int)resp.str().length()).toString());
        response->setHeader("Server", "lulzHTTPd/0.1");

        response->setData(resp.str());

        _socket->send(response->get());
    }

    delete request;
    delete response;
}

}

