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

#if !defined(STRINGPP)
#define STRINGPP

#include <string>
#include <string.h>
#include <sstream>
#include <cstdlib>

#include "Regex.h"

class String
{
  public:
    String (void);
    String (const char* string);
    String (const std::string& string);
    String (int number);
    String (double number);

    void append (String& string);
    void append (char string);
    void append (const std::string& string);
    void append (const int number);
    void append (const long number);
    void append (const long long number);
    void append (const double number);

    String substr (int offset, int end = -1);
    int length (void);
    bool empty (void);

    int toInt (void);
    static int toInt (const std::string& string);

    long toLong (void);
    static long toLong (const std::string& string);

    long long toLongLong (void);
    static long long toLongLong (const std::string& string);

    double toDouble (void);
    static double toDouble (const std::string& string);

    const char* toChars (void);
    static const char* toChars (const std::string& string);
    static const char* toChars (int number);
    static const char* toChars (long number);
    static const char* toChars (long long number);
    static const char* toChars (double number);

    std::string toString (void);
    std::string toString (const char* string);
    static std::string toString (const int number);
    static std::string toString (const long number);
    static std::string toString (const long long number);
    static std::string toString (const double number);

    String operator += (String string);
    String operator += (char string);
    String operator += (const char* string);
    String operator += (std::string string);
    String operator += (int number);
    String operator += (long number);
    String operator += (long long number);
    String operator += (double number);

    String operator + (String string);
    String operator + (char string);
    String operator + (const char* string);
    String operator + (std::string string);
    String operator + (int number);
    String operator + (long number);
    String operator + (long long number);
    String operator + (double number);

    friend std::ostream& operator << (std::ostream& out, String string);
    friend std::istream& operator >> (std::istream& in, String string);

  protected:
    std::string _string;
};

int operator ^= (String& string, String& regex);
int operator ^= (String& string, const char* regex);
int operator ^= (String& string, const std::string& regex);

#endif
