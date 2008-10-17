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

#include "Config.h"

namespace lulzHTTPd {

bool Config::_inited;
String Config::_t_log;
xmlpp::DOM::Document* Config::_doc;

void
Config::init (String configFile, String configType) throw()
{
    if (!_inited) {
        _inited = true;

        xmlpp::DOM::Document* config = Parser::load(configFile, Parser::parseConfType(configType));
        if (config != NULL) {
            Config::load(config);
        }
        else {
            throw Exception(Exception::CONFIG_PARSE_ERROR);
        }
    }
}

void
Config::load (xmlpp::DOM::Document* config)
{
    _doc = config;
}

String
Config::get (String attr)
{
    Regex::Strings parts = Regex::Split("/->/", attr);
    String result;

    xmlpp::DOM::Element* actual = _doc->documentElement();
    for (size_t i = 0; i < parts.size(); i++) {
        xmlpp::DOM::NodeList nodes = actual->getElementsByTagName((parts.at(i) &= "s/\\[.*\\]$//"));

        if (nodes.length() == 0) {
            break;
        }
        else if (nodes.length() > 1) {
            Regex re("/\\[(\\d+)\\]$/");
            if (re.match(parts.at(i))) {
                actual = (xmlpp::DOM::Element*) nodes.item(String(re.group(1)).toInt());
            }
            else {
                actual = (xmlpp::DOM::Element*) nodes.item(0);
            }
        }
        else {
            Regex re("/\\[(.*)\\]$/");
            if (re.match(parts.at(i))) {
                result = ((xmlpp::DOM::Element*)nodes.item(0))->getAttribute(re.group(1));
                break;
            }
            else {
                actual = (xmlpp::DOM::Element*) nodes.item(0);
            }
        }
    }

    if (result.empty()) {
        result = actual->firstChild()->nodeValue();
    }

    return result;
}

bool
Config::test (String& configFile)
{
    return false;
}

String
Config::testLog (void)
{
    return _t_log;
}

}

