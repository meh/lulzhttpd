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
Regex::Strings HTTP::RequestHeaders;
Regex::Strings HTTP::ResponseHeaders;

HTTP::HTTP (void)
{
    this->clear();
    _initStatusCodes();
    _initHeaders();
}

HTTP::~HTTP (void)
{
}

void
HTTP::parse (const std::string& text)
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
            Header header = parseHeader(text, true);
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

void
HTTP::request (const std::string& text)
{
    Regex::Strings lines = Regex::Split("/\\n/", text);

    if (lines.size() < 1) {
        setError(400);
        return;
    }

    re.compile("^(\\w+) (/.+?) HTTP/(\\d.\\d)$");
    if (re.match(lines.at(0))) {
        _method  = re.group(1);
        _uri     = re.group(2);
        _version = std::atof(re.group(3).c_str());
    }
    else {
        setError(400);
        return;
    }

    lines.erase(lines.begin());
    _headers = this->parseHeaders(Regex::Join("\n", lines), true);

    if (_version >= 1.1) {
        if (_headers.find("Host") == _headers.end()) {
            setError(400);
            return;
        }
    }

    if (_method == "POST") {
        re.compile("\\n\\n(.*)", "ms");
        if (re.match(Regex::Join("\n", lines))) {
            _data = re.group(1);
        }
        else {
            _data = "";
        }
    }
}

std::string
HTTP::get (void)
{
    std::stringstream resp;

    resp << "HTTP/" << std::fixed << std::setprecision(1) <<_version << " ";
    resp << this->status() << " " << Codes[this->status()] << std::endl;
   
    Headers::iterator head;
    for (head = _headers.begin(); head != _headers.end(); head++) {
        resp << _fixCase(head->first) << ": " << head->second << std::endl;
    }

    resp << std::endl << _data << std::endl;

    return resp.str();
}

HTTP::Headers
HTTP::parseHeaders (const std::string& headersText, bool request)
{
    Headers headers;

    Regex::Strings lines = Regex::Split("/\\n/", headersText);
    for (size_t i = 0; i < lines.size(); i++) {
        Header header = this->parseHeader(lines.at(i), request);
        if (!header.first.empty()) {
            if (isValidHeader(header.first, request)) {
                _headers[header.first] = header.second;
            }
        }
    }

    return headers;
}

HTTP::Header
HTTP::parseHeader (const std::string& text, bool request)
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

bool
HTTP::isValidHeader (const std::string& header, bool request)
{
    Regex::Strings* checker;
    if (request) {
        checker = &RequestHeaders;
    }
    else {
        checker = &ResponseHeaders;
    }

    for (size_t i = 0; i < checker->size(); i++) {
        if (_fixCase(header) == checker->at(i)) {
            return true;
        }
    }

    return false;
}

std::string
HTTP::getHeader (const std::string& name)
{
    std::string Name = _fixCase(name);

    if (_headers.find(Name) != _headers.end()) {
        return _headers[Name];
    }
    else {
        return "";
    }
}

void
HTTP::setHeader (const std::string& name, const std::string& value)
{
    std::string Name = _fixCase(name);

    if (isValidHeader(Name, false)) {
        _headers[Name] = value;
    }
}

std::string
HTTP::getData (void)
{
    return _data;
}

void
HTTP::setData (const std::string& data)
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
HTTP::status (void)
{
    return _status;
}

void
HTTP::status (int state)
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

std::string
HTTP::_fixCase (const std::string& text)
{
    bool firstChar = true;

    std::string nText;
    for (size_t i = 0; i < text.length(); i++) {
        if (firstChar) {
            firstChar = false;
            nText += std::toupper(text.at(i));
        }
        else {
            if (!std::isalpha(text.at(i))) {
                firstChar = true;
            }
            nText += std::tolower(text.at(i));
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

void
HTTP::_initHeaders (void)
{
    if (RequestHeaders.empty()) {
        RequestHeaders.push_back("Accept");
        RequestHeaders.push_back("Accept-Charset");
        RequestHeaders.push_back("Accept-Encoding");
        RequestHeaders.push_back("Accept-Language");
        RequestHeaders.push_back("Accept-Ranges");
        RequestHeaders.push_back("Authorization");
        RequestHeaders.push_back("Cache-Control");
        RequestHeaders.push_back("Connection");
        RequestHeaders.push_back("Cookie");
        RequestHeaders.push_back("Date");
        RequestHeaders.push_back("Host");
        RequestHeaders.push_back("If-Modified-Since");
        RequestHeaders.push_back("If-None-Match");
        RequestHeaders.push_back("User-Agent");
    }
    
    if (ResponseHeaders.empty()) {
        ResponseHeaders.push_back("Accept-Ranges");
        ResponseHeaders.push_back("Age");
        ResponseHeaders.push_back("Allow");
        ResponseHeaders.push_back("Cache-Control");
        ResponseHeaders.push_back("Content-Encoding");
        ResponseHeaders.push_back("Content-Language");
        ResponseHeaders.push_back("Content-Length");
        ResponseHeaders.push_back("Content-Location");
        ResponseHeaders.push_back("Content-Disposition");
        ResponseHeaders.push_back("Content-MD5");
        ResponseHeaders.push_back("Content-Range");
        ResponseHeaders.push_back("Content-Type");
        ResponseHeaders.push_back("Date");
        ResponseHeaders.push_back("ETag");
        ResponseHeaders.push_back("Expires");
        ResponseHeaders.push_back("Last-Modified");
        ResponseHeaders.push_back("Location");
        ResponseHeaders.push_back("Server");
        ResponseHeaders.push_back("Set-Cookie");
    }
}

