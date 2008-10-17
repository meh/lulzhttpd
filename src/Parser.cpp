/****************************************************************************
* This file is part of lulzHTTPd.                                           *
* Copyleft meh.                                                             *
*                                                                           *
* lulzHTTPd is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzHTTPd is distributed in the hope that it will be useful.              *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzHTTPd.  If not, see <http://www.gnu.org/licenses/>.        *
****************************************************************************/

#include "Parser.h"

namespace lulzHTTPd {

Parser::Parser (void)
{
}

xmlpp::DOM::Document*
Parser::load (String configFile, ConfType configType)
{
    xmlpp::DOM::Document* document;
    
    switch (configType) {
        case none:
        return Parser::load(configFile, _recognize(System::readFile(configFile))); 
        break;

        case lulzhttpd: {
            xmlpp::XML::Parser parser;
            document = parser.load(configFile);
        } break;

        case apache: {
            throw std::exception();
//            parser   = new xmlpp::Parser(new ApacheParser);
//            document = parser->load(configFile);
        } break;

        case lighttpd: {
            throw std::exception();
//            parser   = new xmlpp::Parser(new LighttpParser);
//            document = parser->load(configFile);
        } break;
    }

    document->normalizeDocument();
    return document;
}

Parser::ConfType
Parser::parseConfType (String configType)
{
    if (configType.toLower() == "lulzhttpd") {
        return lulzhttpd;
    }
    else if (configType.toLower() == "apache") {
        return apache;
    }
    else if (configType.toLower() == "lighttpd") {
        return lighttpd;
    }
    else {
        return none;
    }
}

Parser::ConfType
Parser::_recognize (String text)
{
    if (text ^= "/<\\s*\\?\\s*xml|^\\s*<server>/ms") {
        return lulzhttpd;
    }

    else if (text ^= "/^\\s*ServerRoot/") {
        return apache;
    }

    else if (text ^= "/^\\s*server\\.document-root") {
        return lighttpd;
    }
    
    else {
        throw Exception(Exception::CONFIG_FILE_NOT_SUPPORTED);
    }
}

}

