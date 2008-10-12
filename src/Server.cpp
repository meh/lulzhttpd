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
* along with Foobar.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#include "Server.h"

namespace lulzHTTPd {

bool Server::_inited;
int Server::_highestSocket;

void
Server::init (String& config)
{
    if (!_inited) {
        _inited = true;

        Config::init(config);
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

    
}

};

