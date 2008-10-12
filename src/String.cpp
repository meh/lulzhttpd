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

#include "String.h"

String::String (void)
{
    _string = "";
}

String::String (const char* string)
{
    _string = string;
}

String::String (const std::string& string)
{
    _string = string;
}

String::String (const int number)
{
    _string = String::toString(number);
}

String::String (const double number)
{
    _string = String::toString(number);
}

void
String::append (String& string)
{
    _string += string.toString();
}

void
String::append (const std::string& string)
{
    _string += string;
}

void
String::append (const int number)
{
    _string += String::toString(number);
}

void
String::append (const long number)
{
    _string += String::toString(number);
}

void String::append (const long long number)
{
    _string += String::toString(number);
}

void
String::append (const double number)
{
    _string += String::toString(number);
}

bool
String::empty (void)
{
    return _string.empty();
}

int
String::length (void)
{
    return _string.length();
}

int
String::toInt (void)
{
    return std::atoi(String::toChars(_string));
}

int
String::toInt (const std::string& string)
{
    return std::atoi(String::toChars(string));
}

long
String::toLong (void)
{
    return std::atol(_string.c_str());
}

long
String::toLong (const std::string& string)
{
    return std::atol(String::toChars(string));
}

long long
String::toLongLong (void)
{
    return std::atoll(String::toChars(_string));
}

long long
String::toLongLong (const std::string& string)
{
    return std::atoll(String::toChars(string));
}

double
String::toDouble (void)
{
    return std::atof(String::toChars(_string));
}

double
String::toDouble (const std::string& string)
{
    return std::atof(string.c_str());
}

const char*
String::toChars (void)
{
    return _string.c_str();
}

const char*
String::toChars (const std::string& string)
{
    return string.c_str();
}

const char*
String::toChars (const int number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (const long number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (const long long number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (const double number)
{
    return String::toChars(String::toString(number));
}

std::string
String::toString (void)
{
    return _string;
}

std::string
String::toString (const int number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

std::string
String::toString (const long number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

std::string
String::toString (const long long number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

std::string
String::toString (const double number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

String
String::operator += (String string)
{
    this->append(string);

    return *this;
}

String
String::operator += (const char* string)
{
    this->append(string);
}

String
String::operator += (std::string string)
{
    this->append(string);

    return *this;
}

String
String::operator += (int number)
{
    this->append(number);

    return *this;
}

String
String::operator += (long number)
{
    this->append(number);

    return *this;
}

String
String::operator += (long long number)
{
    this->append(number);

    return *this;
}

String
String::operator += (double number)
{
    this->append(number);

    return *this;
}


String
String::operator + (String string)
{
    this->append(string);

    return *this;
}

String
String::operator + (const char* string)
{
    this->append(string);

    return *this;
}

String
String::operator + (std::string string)
{
    this->append(string);

    return *this;
}

String
String::operator + (int number)
{
    this->append(number);

    return *this;
}

String
String::operator + (long number)
{
    this->append(number);

    return *this;
}

String
String::operator + (long long number)
{
    this->append(number);

    return *this;
}

String
String::operator + (double number)
{
    this->append(number);

    return *this;
}

std::ostream&
operator << (std::ostream& out, String string)
{
    out << string.toString();
    return out;
}

std::istream&
operator >> (std::istream& in, String string)
{
    in >> string._string;
    return in;
}

int operator ^= (String string, String regex)
{
    Regex::Match(regex.toString(), string.toString());
}

int operator ^= (String string, const char* regex)
{
    Regex::Match(regex, string.toString());
}

int operator ^= (String string, std::string regex)
{
    Regex::Match(regex, string.toString());
}

