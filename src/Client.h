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

#if !defined(LULZHTTPD_CLIENT_H)
#define LULZHTTPD_CLIENT_H

#include "common.h"
#include "Socket.h"
#include "HTTP.h"

namespace lulzHTTPd {

class Client
{
  public:
    Client (System::Socket* sock);
    ~Client (void);

    void start (void);

  private:
    System::Socket* _socket;
};

}

#endif
