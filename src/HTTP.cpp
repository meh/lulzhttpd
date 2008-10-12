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

HTTP::StatusCodes HTTP::Status;
Regex::Strings HTTP::RequestHeaders;
Regex::Strings HTTP::ResponseHeaders;

HTTP::HTTP (void)
{
    this->clear();
    _initStatusCodes();
    _initHeaders();
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
        if (_headers.find("host") == _headers.end()) {
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
HTTP::response (void)
{
    return "";
}

HTTP::Headers
HTTP::parseHeaders (const std::string& headersText, bool request)
{
    Headers headers;
    re.compile("^([\\w\\-]+)\\s*:\\s*(.*)$", "");

    Regex::Strings lines = Regex::Split("/\\n/", headersText);
    for (size_t i = 0; i < lines.size(); i++) {
        if (re.match(lines.at(i))) {
            std::string name  = _lowerCase(re.group(1));
            std::string value = re.group(2);

            if (isValidHeader(name, request)) {
                _headers[name] = value;
            }
        }
    }

    return headers;
}

void
HTTP::clear (void)
{
    _isOk    = false;
    _error   = 0;
    _version = 0;
    _uri     = "";
    _host    = "";
    _headers.clear();
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
        if (_lowerCase(header) == _lowerCase(checker->at(i))) {
            return true;
        }
    }

    return false;
}

std::string
HTTP::_lowerCase (const std::string& text)
{
    std::string nText;
    for (size_t i = 0; i < text.length(); i++) {
        nText += std::tolower(text.at(i));
    }
    return nText;
}

void
HTTP::_initStatusCodes (void)
{
    if (Status.empty()) {
        // Informational
        Status[100] = "Continue";
        Status[101] = "Switching Protocols";
    
        // Successful
        Status[200] = "OK";
        Status[201] = "Created";
        Status[202] = "Accepted";
        Status[203] = "Non-Authoritative Information";
        Status[204] = "No Content";
        Status[205] = "Reset Content";
        Status[206] = "Partial Content";
        
        // Redirection
        Status[300] = "Multiple Choices";
        Status[301] = "Moved Permanently";
        Status[302] = "Found";
        Status[303] = "See Other";
        Status[304] = "Not Modified";
        Status[305] = "Use Proxy";
        Status[306] = "LOLNO";
        Status[307] = "Temporary Redirect";
    
        // Client Error
        Status[400] = "Bad Request";
        Status[401] = "Unauthorized";
        Status[402] = "Payment Required";
        Status[403] = "Forbidden";
        Status[404] = "Not Found";
        Status[405] = "Method Not Allowed";
        Status[406] = "Not Acceptable";
        Status[407] = "Proxy Authentication Required";
        Status[408] = "Request Timeout";
        Status[409] = "Conflict";
        Status[410] = "Gone";
        Status[411] = "Length Required";
        Status[412] = "Precondition Failed";
        Status[413] = "Request Entity Too Large";
        Status[414] = "Request-URI Too Long";
        Status[415] = "Unsupported Media Type";
        Status[416] = "Requested Range Not Satisfiable";
        Status[417] = "Expectation Failed";
    
        // Server Error
        Status[500] = "Internal Server Error";
        Status[501] = "Not Implemented";
        Status[502] = "Bad Gateway";
        Status[503] = "Service Unavailable";
        Status[504] = "Gateway Timeout";
        Status[505] = "HTTP Version Not Supported";
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
        ResponseHeaders.push_back("Date");
        ResponseHeaders.push_back("ETag");
        ResponseHeaders.push_back("Expires");
        ResponseHeaders.push_back("Last-Modified");
        ResponseHeaders.push_back("Location");
        ResponseHeaders.push_back("Server");
        ResponseHeaders.push_back("Set-Cookie");
    }
}

