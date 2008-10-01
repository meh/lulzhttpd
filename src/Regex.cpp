#include "Regex.h"

Regex::Regex (void)
{
    this->reset();
    _opts  = PCRE_CASELESS | PCRE_EXTENDED;
    _re    = NULL;
    _extra = NULL;

    _lastPosition = 0;
    _isGlobal     = false;

    _matchesNumber = 0;
}

Regex::Regex (const char* regex)
{
    Regex re("^/(.*?)/(\w+)?$", PCRE_CASELESS | PCRE_EXTENDED);
    re.match(regex);

    if (re.matches()) {
        std::string nRegex = re[1];
        std::string opts   = re[2];

        this->_init(nRegex.c_str(), opts.c_str());
    }
    else {
        throw std::exception();
    }
}

Regex::Regex (const std::string& regex)
{
    Regex re("^/(.*?)/(\w+)?$", PCRE_CASELESS | PCRE_EXTENDED);
    re.match(regex);

    if (re.matches()) {
        std::string nRegex = re[1];
        std::string opts   = re[2];

        this->_init(nRegex, opts);
    }
    else {
        throw std::exception();
    }
}

Regex::Regex (const char* regex, unsigned int opts)
{
    this->_init(regex, opts);
}

Regex::Regex (const char* regex, const char* opts)
{
    this->_init(regex, opts);
}

Regex::Regex (const std::string& regex, unsigned int opts)
{
    this->_init(regex.c_str(), opts);
}

Regex::Regex (const std::string& regex, const std::string& opts)
{
    this->_init(regex.c_str(), opts.c_str());
}

void
Regex::_init (const char* regex, unsigned int opts)
{
    this->reset();
    _isGlobal = false;

    _opts = opts;
    this->compile(regex);
    _extra = NULL;

    _lastPosition  = 0;
    _matchesNumber = 0;
}

void
Regex::_init (const char* regex, const char* opts)
{
    this->reset();
    _isGlobal = false;

    _opts = this->_parseOptions(opts);
    this->compile(regex);
    _extra = NULL;

    _lastPosition  = 0;
    _matchesNumber = 0;
}

Regex::~Regex (void)
{
    if (_re) {
        pcre_free(_re);
    }
}

void
Regex::reset (void)
{
    _p_lastMatched = NULL;
    _lastMatched   = "";

    _marks.clear();
    _lastPosition = 0;

    _isValid = false;
}

void
Regex::compile (const char* regex, unsigned int opts)
{
    _opts = opts;

    const char *error;
    int errorOffset;

    _re = pcre_compile(regex, _opts, &error, &errorOffset, 0)

    if (_re != NULL) {
        _isValid = true;
    }
    else {
        _isValid = false;
    }
}

void
Regex::compile (const char* regex, const char* opts)
{
    _opts = _parseOptions(opts);

    this->compile(regex);
}

void
Regex::compile (const std::string& regex, unsigned int opts)
{
    this->compile(regex.c_str(), opts);
}

void
Regex::compile (const std::string& regex, const std::string& opts)
{
    this->compile(regex.c_str(), opts.c_str());
}

int
Regex::match (const char* string, unsigned int offset)
{

}

unsigned int
Regex::_parseOptions (const std::string& opts)
{
    unsigned int rOpts = 0;
    static size_t end = std::string::npos;

    rOpts |= (opts.find('i') != end) ? PCRE_CASELESS  : NULL;
    rOpts |= (opts.find('m') != end) ? PCRE_MULTILINE : NULL;
    rOpts |= (opts.find('s') != end) ? PCRE_DOTALL    : NULL;
    rOpts |= (opts.find('x') != end) ? PCRE_EXTENDED  : NULL;
    rOpts |= (opts.find('U') != end) ? PCRE_UNGREEDY  : NULL;

    _isGlobal = (opts.find('g') != end) ? true : false;

    return rOpts;
}

pcre*
Regex::_clonePCRE (pcre* re)
{
    if (re == NULL) {
        return NULL;
    }

    size_t size;
    pcre* newRe;

    pcre_fullinfo(re, 0, PCRE_INFO_SIZE, &size);
    newRe = (pcre*) new char[size];
    memcpy(newRe, re, size);

    return newRe;
}
