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

#if !defined(HTTP_H)
#define HTTP_H

#include <string>
#include <map>

class HTTP
{
  public:
    typedef std::map<std::string, std::string> Headers;

  public:
    HTTP (void);
    HTTP (const std::string& headersText, bool request = true)

    Headers parseHeaders (const std::string& headersText);

    bool isOk (void);

  private:
    bool _isOk;
};

#endif
