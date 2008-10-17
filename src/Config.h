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

#if !defined(LULZHTTPD_CONFIG_H)
#define LULZHTTPD_CONFIG_H

#include "common.h"

#include "Parser.h"
#include <xml++/DOM.h>

namespace lulzHTTPd {

class Config
{
  protected:
    Config (void);

  public:
    static void init (String configFile, String configType) throw();

    static String get (String attr);

    static bool test (String& configFile);
    static String testLog (void);

  private:
    static xmlpp::DOM::Document* _parse (xmlpp::DOM::Document* doc);
    static void _fixElement (xmlpp::DOM::Element* element);
    static String _fixPath (String path);

  private:
    static bool _inited;

    static String _t_log;

    static xmlpp::DOM::Document* _doc;
};

}

#endif
