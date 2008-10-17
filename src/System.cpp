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

#include "System.h"

namespace lulzHTTPd {

namespace System {

String readFile (String fileName)
{
    std::ifstream file(fileName);

    if (file.is_open()) {
        std::string text;
        while (!file.eof()) {
            std::string line;

            getline(file, line);
            text += line + "\n";
        }
        file.close();
        text.resize(text.length()-2);

        return text;
    }
    else {
        throw Exception(Exception::FILE_NOT_FOUND);
    }
}

String normalizePath (String path)
{
    return path &= "s/\\.*\\/*//";
}

}

}

