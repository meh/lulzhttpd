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

#include "common.h"

#define HTTPError(n) \
    _status = n;\
    _isOk   = false;\
    _done   = true;

class HTTP
{
  public:
    typedef std::map<std::string, String> Headers;
    typedef std::pair<String, String> Header;
    typedef std::map<int, String> StatusCodes;

  public:
    static StatusCodes Codes;

  public:
    HTTP (void);
    ~HTTP (void);

    void request (String text);

    String get (void);

    Headers parseHeaders (String headersText);
    Header parseHeader (String text);

    String getHeader (String name);
    void setHeader (String name, String value);

    String getUri (void);
    String getHost (void);
    String getMethod (void);

    String getData (void);
    void setData (String data);

    float getVersion (void);
    void setVersion (float version);

    int getStatus (void);
    void setStatus (int state);

    bool done (void);
    bool isOk (void);

    void clear (void);

  private:
    Regex re;

    bool _done;
    bool _isOk;

    int _status;

    String _method;
    String _uri;
    float _version;
    String _host;

    bool _dataIncoming;
    String _data;

    Headers _headers;

  private:
    void _initStatusCodes (void);

    String _fixCase (String text);
};

#endif
