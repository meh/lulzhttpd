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

#include "Server.h"

namespace lulzHTTPd {

bool Server::_inited;
int Server::_highestSocket;

void* createClient (void* arg)
{
    Client::start((System::Socket*) arg);
    pthread_exit(NULL);
}

void
Server::init (String configFile, String configType)
{
    if (!_inited) {
        _inited = true;

        Config::init(configFile, configType);
    }
}

void
Server::start (void)
{
    if (!_inited) {
        throw Exception(Exception::SERVER_NOT_INITED);
    }

    // --- Modules etc, all the stuff that has to be done.
    // Modules bla bla ---
    
    String host = Config::get("general->connection->host").empty()
                      ? "0.0.0.0"
                      : Config::get("general->connection->host");
    
    int port = Config::get("general->connection->port").empty()
                   ? 80
                   : Config::get("general->port").toInt();

    int maxConnections = Config::get("general->port-max-connections").empty()
                             ? 250
                             : Config::get("general->port-max-connections").toInt();

    System::Socket* sock = new System::Socket(host, port, maxConnections);

    while (1) {
        pthread_t thread = 0;
        pthread_create(&thread, NULL, createClient, sock->accept());
        pthread_join(thread, NULL);
    }

    delete sock;
}

}

