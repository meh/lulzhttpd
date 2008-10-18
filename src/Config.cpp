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
            _doc = _parse(config);
            _doc->normalizeDocument();
        }
        else {
            throw Exception(Exception::CONFIG_PARSE_ERROR);
        }
    }
}

String
Config::get (String attr)
{
    Regex::Strings parts = Regex::Split("/->/", attr);
    bool error = false;
    String result;

    xmlpp::DOM::Element* actual = _doc->documentElement();
    for (size_t i = 0; i < parts.size(); i++) {
        xmlpp::DOM::NodeList nodes = actual->getElementsByTagName((parts.at(i) &= "s/\\[.*\\]$//g"));

        if (nodes.length() == 0) {
            error = true;
            break;
        }
        else if (nodes.length() > 1) {
            // Get the number in something like thing[3]
            Regex re("/\\[(\\d+)\\]/");
            if (re.match(parts.at(i))) {
                int item = String(re.group(1)).toInt();

                if (item >= nodes.length()) {
                    error = true;
                    break;
                }

                actual = (xmlpp::DOM::Element*) nodes.item(item);

            }
             
            // If there's an attribute in something like thing[2][attribute]
            // it gets the attribute.
            re.compile("\\[([^\\[\\]]*?)\\]$", "");
            if (re.match(parts.at(i))) {
                 result = actual->getAttribute(re.group(1));
                 break;
            }
            else {
                actual = (xmlpp::DOM::Element*) nodes.item(0);
            }
        }
        else {
            // Gets the attribute in something like thing[attribute]
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

    if (result.empty() && !error) {
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

xmlpp::DOM::Document*
Config::_parse (xmlpp::DOM::Document* doc)
{
    _doc = doc;
    _fixElement(doc->documentElement());

    xmlpp::XML::Parser parser;
    xmlpp::DOM::NodeList includes = doc->getElementsByTagName("include");

    for (size_t i = 0; i < includes.length(); i++) {
        xmlpp::DOM::Element* include = (xmlpp::DOM::Element*) includes.item(i);

        String path = include->getAttribute("path");
        xmlpp::DOM::Document* includeDoc = parser.load(path);
       
        delete include->parentNode()->replaceChild(
            includeDoc->documentElement()->cloneNode(),
            include
        );
        delete includeDoc;
    }

    return doc;
}

void
Config::_fixElement (xmlpp::DOM::Element* element)
{
    for (size_t i = 0; i < element->childNodes().length(); i++) {
        xmlpp::DOM::Node* node = element->childNodes().item(i);

        if (node->nodeType() == xmlpp::DOM::Node::ELEMENT_NODE) {
            xmlpp::DOM::Element* ele = (xmlpp::DOM::Element*) node;
            if (ele->hasAttribute("path")) {
                ele->setAttribute("path", _fixPath(ele->getAttribute("path")));
            }
            _fixElement(ele);
        }
    }
}

String
Config::_fixPath (String path)
{
    String fixedPath = path;

    fixedPath = Regex::Sub("s/%includes%/"+Config::get("directories->includes[path]")+"/", fixedPath);
    fixedPath = Regex::Sub("s/%modules%/"+Config::get("directories->modules[path]")+"/", fixedPath);
    fixedPath = Regex::Sub("s/%logs%/"+Config::get("directories->logs[path]")+"/", fixedPath);

    return fixedPath;
}

}

