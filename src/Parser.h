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

#if !defined(LULZHTTPD_PARSER_H)
#define LULZHTTPD_PARSER_H

#include "common.h"

#include <xml++/xml++.h>

namespace lulzHTTPd {

class Parser
{
  public:
    typedef enum { none, lulzhttpd, apache, lighttpd } ConfType;

  protected:
    Parser (void);

  public:
    static xmlpp::DOM::Document* load (String configFile, ConfType configType);

    static ConfType parseConfType (String configType);

  private:
    static ConfType _recognize (String text);
};

}

#endif
