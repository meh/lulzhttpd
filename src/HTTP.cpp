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

HTTP::StatusCodes HTTP::Codes;

HTTP::HTTP (void)
{
    this->clear();
    _initStatusCodes();
}

HTTP::~HTTP (void)
{
}

void
HTTP::request (String text)
{
    if (_method.empty()) {
        re.compile("^(\\w+) (/.*?) HTTP/(\\d.\\d)$", "");
        if (re.match(text)) {
            _method  = re.group(1);
            _uri     = re.group(2);
            _version = std::atof(re.group(3).c_str());
        }
        else {
            setError(400);
            return;
        }
    }
    else {
        if (_dataIncoming) {
            _data = text;
            _done = true;
            _isOk = true;
        }
        else {
            Header header = parseHeader(text);
            if (!header.first.empty()) {
                _headers[header.first] = header.second;
            }
        }

        if (text.empty()) {
            if (_method == "POST") {
                _dataIncoming = true;
            }
            else {
                _done = true;
            }

            if (_version >= 1.1) {
                if (_headers.find("Host") == _headers.end()) {
                    setError(400);
                    return;
                }
            }

            _isOk = true;
        }
    }
}

String
HTTP::get (void)
{
    std::stringstream resp;

    resp << "HTTP/" << std::fixed << std::setprecision(1) <<_version << " ";
    resp << this->getStatus() << " " << Codes[this->getStatus()] << std::endl;
   
    Headers::iterator head;
    for (head = _headers.begin(); head != _headers.end(); head++) {
        resp << head->first << ": " << head->second << std::endl;
    }

    resp << std::endl << _data << std::endl;

    return resp.str();
}

HTTP::Headers
HTTP::parseHeaders (String headersText)
{
    Headers headers;

    Regex::Strings lines = Regex::Split("/\\n/", headersText);
    for (size_t i = 0; i < lines.size(); i++) {
        Header header = this->parseHeader(String(lines.at(i)));
        if (!header.first.empty()) {
            _headers[header.first] = header.second;
        }
    }

    return headers;
}

HTTP::Header
HTTP::parseHeader (String text)
{
    re.compile("^([\\w\\-]+)\\s*:\\s*(.*)$", "");

    Header header;
    if (re.match(text)) {
        header.first  = _fixCase(re.group(1));
        header.second = re.group(2);
    }
    else {
        header.first  = "";
        header.second = "";
    }

    return header;
}

String
HTTP::getHeader (String name)
{
    String Name = _fixCase(name);

    if (_headers.find(Name) != _headers.end()) {
        return _headers[Name];
    }
    else {
        return "";
    }
}

void
HTTP::setHeader (String name, String value)
{
    _headers[_fixCase(name)] = value;
}

String
HTTP::getUri (void)
{
    return _uri;
}

String
HTTP::getHost (void)
{
    return _host;
}

String
HTTP::getMethod (void)
{
    return _method;
}

String
HTTP::getData (void)
{
    return _data;
}

void
HTTP::setData (String data)
{
    _data = data;
}

float
HTTP::getVersion (void)
{
    return _version;
}

void
HTTP::setVersion (float version)
{
    _version = version;
}

int
HTTP::getStatus (void)
{
    return _status;
}

void
HTTP::setStatus (int state)
{
    _status = state;
}

bool
HTTP::done (void)
{
    return _done;
}

bool
HTTP::isOk (void)
{
    return _isOk;
}

void
HTTP::clear (void)
{
    _done    = false;
    _isOk    = false;
    _status  = 0;
    _version = 0;

    _method  = "";
    _uri     = "";
    _host    = "";

    _data         = "";
    _dataIncoming = false;

    _headers.clear();
}

String
HTTP::_fixCase (String text)
{
    bool firstChar = true;

    String nText;
    for (size_t i = 0; i < text.length(); i++) {
        if (firstChar) {
            firstChar = false;
            nText += String::toUpper(text.at(i));
        }
        else {
            if (!std::isalpha(text.at(i))) {
                firstChar = true;
            }
            nText += String::toLower(text.at(i));
        }
    }

    return nText;
}

void
HTTP::_initStatusCodes (void)
{
    if (Codes.empty()) {
        // Informational
        Codes[100] = "Continue";
        Codes[101] = "Switching Protocols";
    
        // Successful
        Codes[200] = "OK";
        Codes[201] = "Created";
        Codes[202] = "Accepted";
        Codes[203] = "Non-Authoritative Information";
        Codes[204] = "No Content";
        Codes[205] = "Reset Content";
        Codes[206] = "Partial Content";
        
        // Redirection
        Codes[300] = "Multiple Choices";
        Codes[301] = "Moved Permanently";
        Codes[302] = "Found";
        Codes[303] = "See Other";
        Codes[304] = "Not Modified";
        Codes[305] = "Use Proxy";
        Codes[306] = "LOLNO";
        Codes[307] = "Temporary Redirect";
    
        // Client Error
        Codes[400] = "Bad Request";
        Codes[401] = "Unauthorized";
        Codes[402] = "Payment Required";
        Codes[403] = "Forbidden";
        Codes[404] = "Not Found";
        Codes[405] = "Method Not Allowed";
        Codes[406] = "Not Acceptable";
        Codes[407] = "Proxy Authentication Required";
        Codes[408] = "Request Timeout";
        Codes[409] = "Conflict";
        Codes[410] = "Gone";
        Codes[411] = "Length Required";
        Codes[412] = "Precondition Failed";
        Codes[413] = "Request Entity Too Large";
        Codes[414] = "Request-URI Too Long";
        Codes[415] = "Unsupported Media Type";
        Codes[416] = "Requested Range Not Satisfiable";
        Codes[417] = "Expectation Failed";
    
        // Server Error
        Codes[500] = "Internal Server Error";
        Codes[501] = "Not Implemented";
        Codes[502] = "Bad Gateway";
        Codes[503] = "Service Unavailable";
        Codes[504] = "Gateway Timeout";
        Codes[505] = "HTTP Version Not Supported";
    }
}

