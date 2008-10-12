/****************************************************************************
* Copyleft meh.                                                             *
*                                                                           *
* This is free software: you can redistribute it and/or modif.              *
* it under the terms of the GNU Affero General Public License a.            *
* published by the Free Software Foundation, either version 3 of the        *
* License, or (at your option) any later version.                           *
*                                                                           *
* This program is distributed in the hope that it will be useful.           *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU Affero General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Affero General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/

#include "HTTP.h"

HTTP::HTTP (void)
{
    _isOk = false;
}

HTTP::HTTP (const std::string& headersText, bool request)
{
    Headers headers = this->parseHeaders(head);

    if (request) {

    }
    else {
    }
}

Headers
HTTP::parseHeaders (const std::string& headersText)
{
    Headers headers;

    
}
