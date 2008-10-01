#if !defined(REGEXPP)
#define REGEXPP

#include <iostream>

#include <cstring>
#include <string>
#include <vector>

#include <pcre.h>

typedef std::vector<std::string> Strings;

class Regex
{
  public:
    Regex (void);

    Regex (const char* regex);
    Regex (const std::string& regex);

    Regex (const char* regex, unsigned int opts);
    Regex (const char* regex, const char* opts);

    Regex (const std::string& regex, unsigned int opts);
    Regex (const std::string& regex, const std::string& opts);
    
    Regex (const Regex& re);

    ~Regex (void);

    typedef std::pair<int, int> Markers;

    unsigned int options (void);

    void options (unsigned opts);
    void options (const char* opts);
    void options (const std::string& opts);

    void compile (const char* regex, unsigned int opts = -1);
    void compile (const char* regex, const char* opts);

    void compile (const std::string& regex, unsigned int opts = -1);
    void compile (const std::string& regex, const std::string& opts);

    int match (const char* string, unsigned int offset = 0);
    int match (const std::string& string, unsigned int offset = 0);

    static int match (const char* regex, const char* string);
    static int match (const std::string& regex, const std::string& string);

    int matches (void);

    std::string group (int index);
    static std::string Group (int index);

    bool isValid (void);

    void reset (void);

    std::string operator [] (int index);

  protected:
    pcre* _re;
    pcre_extra* _extra;

    unsigned int _opts;

    int _matchesNumber;

    std::vector<Markers> _marks;

    std::string _lastMatched;
    void* _p_lastMatched;

    bool _isGlobal;
    int _lastPosition;

    bool _isValid;

    std::vector<std::string> _groups;
    static std::vector<std::string> _globalGroups;

  protected:
    static pcre* _clonePCRE (pcre* re);

    unsigned int _parseOptions (const std::string& opts);

  private:
    void _init (const std::string& regex, unsigned int opts);
    void _init (const std::string& regex, const std::string& opts);
};

int operator ^= (std::string string, const char* regex);
int operator ^= (std::string string, std::string string);

#define $(n) Regex::Group(n)

#endif
