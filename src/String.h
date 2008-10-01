#if !defined(STRINGPP)
#define STRINGPP

#include <string>
#include <string.h>
#include <sstream>
#include <cstdlib>

class String
{
  public:
    String (void);

    String (const char* string);
    String (std::string string);
    String (int number);
    String (double number);

    void append (String& string);
    void append (const char* string);
    void append (std::string string);
    void append (int number);
    void append (long number);
    void append (long long number);
    void append (double number);

    int length (void);
    bool empty (void);

    int toInt (void);
    static int toInt (const char* string);
    static int toInt (std::string string);

    long toLong (void);
    static long toLong (const char* string);
    static long toLong (std::string string);

    long long toLongLong (void);
    static long long toLongLong (const char* string);
    static long long toLongLong (std::string string);

    double toDouble (void);
    static double toDouble (const char* string);
    static double toDouble (std::string string);

    const char* toChars (void);
    static const char* toChars (std::string string);
    static const char* toChars (int number);
    static const char* toChars (long number);
    static const char* toChars (long long number);
    static const char* toChars (double number);

    std::string toString (void);
    std::string toString (const char* string);
    static std::string toString (int number);
    static std::string toString (long number);
    static std::string toString (long long number);
    static std::string toString (double number);

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

};

#endif
