#include "String.h"

String::String (void)
{
    _string = "";
}

String::String (const char* string)
{
    _string = string;
}

String::String (std::string string)
{
    _string = string;
}

String::String (int number)
{
    _string = String::toString(number);
}

String::String (double number)
{
    _string = String::toString(number);
}

void
String::append (String& string)
{
    _string += string.toString();
}

void
String::append (const char* string)
{
    _string += string;
}

void
String::append (std::string string)
{
    _string += string;
}

void
String::append (int number)
{
    _string += String::toString(number);
}

void
String::append (long number)
{
    _string += String::toString(number);
}

void String::append (long long number)
{
    _string += String::toString(number);
}

void
String::append (double number)
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
String::toInt (const char* string)
{
    return std::atoi(string);
}

int
String::toInt (std::string string)
{
    return std::atoi(String::toChars(string));
}

long
String::toLong (void)
{
    return std::atol(_string.c_str());
}

long
String::toLong (const char* string)
{
    return std::atol(string);
}

long
String::toLong (std::string string)
{
    return std::atol(String::toChars(string));
}

long long
String::toLongLong (void)
{
    return std::atoll(String::toChars(_string));
}

long long
String::toLongLong (const char* string)
{
    return std::atoll(string);
}

long long
String::toLongLong (std::string string)
{
    return std::atoll(String::toChars(string));
}

double
String::toDouble (void)
{
    return std::atof(String::toChars(_string));
}

double
String::toDouble (const char* string)
{
    return std::atof(string);
}

double
String::toDouble (std::string string)
{
    return std::atof(string.c_str());
}

const char*
String::toChars (void)
{
    return _string.c_str();
}

const char*
String::toChars (std::string string)
{
    return string.c_str();
}

const char*
String::toChars (int number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (long number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (long long number)
{
    return String::toChars(String::toString(number));
}

const char*
String::toChars (double number)
{
    return String::toChars(String::toString(number));
}

std::string
String::toString (void)
{
    return _string;
}

std::string
String::toString (const char* string)
{
    return (std::string) string;
}

std::string
String::toString (int number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

std::string
String::toString (long number)
{
    std::strinstream string;
    string << number;

    return srting.str();
}

std::string
String::toString (long long number)
{
    std::stringstream string;
    string << number;

    return string.str();
}

std::string
String::toString (double number)
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

    return *this;
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
operator << (std::ostream& out, nc::String string)
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

