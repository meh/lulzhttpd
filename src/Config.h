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

namespace lulzHTTPd {

class Config
{
  public:
    static void init (String& configFile) throw();
    static void load (DOM::Document* config);

    static bool test (String& configFile);
    static String testLog (void);

  protected:
    Config (String& configFile);

  private:
    static bool _inited;

    static String _t_log;
};

};

#endif
