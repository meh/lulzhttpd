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
    void append (const std::string& string);
    void append (const int number);
    void append (const long number);
    void append (const long long number);
    void append (const double number);

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
    String operator += (const char* string);
    String operator += (std::string string);
    String operator += (int number);
    String operator += (long number);
    String operator += (long long number);
    String operator += (double number);

    String operator + (String string);
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

int operator ^= (String string, String regex);
int operator ^= (String string, const char* regex);
int operator ^= (String string, const std::string& regex);

#endif
