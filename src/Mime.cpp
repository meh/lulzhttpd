/****************************************************************************
* This file is part of lulzHTTPd.                                           *
* Copyleft meh.                                                             *
*                                                                           *
* lulzHTTPd is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzHTTPd is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzHTTPd.  If not, see <http://www.gnu.org/licenses/>.        *
****************************************************************************/

#include "Mime.h"

namespace lulzHTTPd {

Mime::Mime (void)
{
}

String
Mime::getType (String extension)
{
    int i = 0;
    while (!Config::get("mime-types->mime["+String(i)+"][extension]").empty()) {
        if (Config::get("mime-types->mime["+String(i)+"][extension]") == extension) {
            return Config::get("mime-types->mime["+String(i)+"][type]");
        }

        i++;
    }

    return Config::get("mime-types[default]");
}

}

