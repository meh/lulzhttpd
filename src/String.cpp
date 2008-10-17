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

String::String (const char string)
{
    _string = std::string(1, string);
}

String::String (const char* string)
{
    _string = string;
}

String::String (const std::string& string)
{
    _string = string;
}

String::String (const size_t number)
{
    _string = String::toString(number);
}

String::String (const int number)
{
    _string = String::toString(number);
}

String::String (const long number)
{
    _string = String::toString(number);
}

String::String (const double number)
{
    _string = String::toString(number);
}

char
String::at (size_t index)
{
    return _string.at(index);
}

void
String::append (String& string)
{
    _string += string.toString();
}

void
String::append (const char string)
{
    _string.append(1, string);
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

void
String::append (const double number)
{
    _string += String::toString(number);
}

String
String::substr (int offset, int end)
{
    if (end < 0) {
        return _string.substr(offset);
    }
    else {
        return _string.substr(offset, end);
    }
}

int
String::length (void)
{
    return _string.length();
}

bool
String::empty (void)
{
    return _string.empty();
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
String::toString (const size_t number)
{
    std::stringstream string;
    string << number;

    return string.str();
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
String::toString (const double number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

String
String::toUpper (void)
{
    return String::toUpper(_string);
}

char
String::toUpper (const char string)
{
    return std::toupper(string);
}

String
String::toUpper (String string)
{
    String text;
    for (size_t i = 0; i < string.length(); i++) {
        text += String::toUpper(string.at(i));
    }
    return text;
}

String
String::toLower (void)
{
    return String::toLower(_string);
}

char
String::toLower (const char string)
{
    return std::tolower(string);
}

String
String::toLower (String string)
{
    String text;
    for (size_t i = 0; i < string.length(); i++) {
        text += String::toLower(string.at(i));
    }
    return text;
}

String
String::operator += (String string)
{
    this->append(string);

    return *this;
}

String
String::operator += (const char string)
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
String::operator + (char string)
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
String::operator + (double number)
{
    this->append(number);

    return *this;
}

String
operator + (const char text, String string)
{
    return String(text)+string;
}

String
operator + (const char* text, String string)
{
    return String(text)+string;
}

String
operator + (const std::string& text, String string)
{
    return String(text)+string;
}

String
operator + (const size_t number, String string)
{
    return String(number)+string;
}

String
operator + (const int number, String string)
{
    return String(number)+string;
}

String
operator + (const long number, String string)
{
    return String(number)+string;
}

String
operator + (const double number, String string)
{
    return String(number)+string;
}

bool
String::operator == (const char* other)
{
    return _string == other;
}

bool
String::operator == (const std::string& other)
{
    return _string == other;
}

bool
String::operator == (String& other)
{
    return _string == other.toString();
}

bool
String::operator != (const char* other)
{
    return _string != other;
}

bool
String::operator != (const std::string& other)
{
    return _string != other;
}

bool
String::operator != (String& other)
{
    return _string != other.toString();
}

String::operator int (void)
{
    return this->toInt();
}

String::operator long (void)
{
    return this->toLong();
}

String::operator double (void)
{
    return this->toDouble();
}

String::operator char* (void)
{
    return (char*) this->toChars();
}

String::operator std::string (void)
{
    return std::string(this->toString());
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

std::stringstream&
operator << (std::stringstream& stream, String string)
{
    stream << string.toChars();
    return stream;
}

