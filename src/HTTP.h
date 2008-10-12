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

#include <cctype>
#include <cstdio>
#include <string>
#include <map>

#include "Regex.h"

#define setError(n) \
    _status = n;\
    _isOk   = false;

class HTTP
{
  public:
    typedef std::map<std::string, std::string> Headers;
    typedef std::pair<std::string, std::string> Header;
    typedef std::map<int, std::string> StatusCodes;

  public:
    static StatusCodes Codes;
    static Regex::Strings RequestHeaders;
    static Regex::Strings ResponseHeaders;

  public:
    HTTP (void);

    void parse (const std::string& text);
    void request (const std::string& text);

    std::string response (void);

    Headers parseHeaders (const std::string& headersText, bool request);
    Header parseHeader (const std::string& text, bool request);

    bool isValidHeader (const std::string& header, bool request);

    std::string getHeader (const std::string& name);
    void setHeader (const std::string& name, const std::string& value);

    std::string getData (void);
    void setData (const std::string& data);

    float getVersion (void);
    void setVersion (float version);

    int status (void);
    void status (int state);

    bool done (void);
    bool isOk (void);

    void clear (void);

  private:
    Regex re;

    bool _done;
    bool _isOk;

    int _status;

    std::string _method;
    std::string _uri;
    float _version;
    std::string _host;

    bool _dataIncoming;
    std::string _data;

    Headers _headers;

  private:
    void _initStatusCodes (void);
    void _initHeaders (void);

    std::string _fixCase (const std::string& text);
};

#endif
