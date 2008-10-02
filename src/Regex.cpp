#include "Regex.h"

std::vector<std::string> Regex::_globalGroups;

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
    Regex re("^/(.*?)/(\\w*)$", PCRE_CASELESS | PCRE_EXTENDED);
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
    Regex re("^/(.*?)/(\\w*)?$", PCRE_CASELESS | PCRE_EXTENDED);
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

Regex::Regex (const char* regex, unsigned int opts)
{
    this->_init((std::string) regex, opts);
}

Regex::Regex (const char* regex, const char* opts)
{
    this->_init((std::string) regex, (std::string) opts);
}

Regex::Regex (const std::string& regex, unsigned int opts)
{
    this->_init(regex, opts);
}

Regex::Regex (const std::string& regex, const std::string& opts)
{
    this->_init(regex, opts);
}

void
Regex::_init (const std::string& regex, unsigned int opts)
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
Regex::_init (const std::string& regex, const std::string& opts)
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

unsigned int
Regex::options (void)
{
    return _opts;
}

bool
Regex::isGlobal (void)
{
    return _isGlobal;
}

void
Regex::options (unsigned int opts)
{
    _opts = opts;
}

void
Regex::options (const char* opts)
{
    _opts = _parseOptions((std::string) opts);
}

void
Regex::options (const std::string& opts)
{
    _opts = _parseOptions(opts);
}

void
Regex::compile (const char* regex, unsigned int opts)
{
    this->compile((std::string) regex, opts);
}

void
Regex::compile (const char* regex, const char* opts)
{
    _opts = _parseOptions((std::string) opts);

    this->compile((std::string) regex);
}

void
Regex::compile (const std::string& regex, unsigned int opts)
{
    if (opts != -1) {
        _opts = opts;
    }

    const char *error;
    int errorOffset;

    _re = pcre_compile(regex.c_str(), _opts, &error, &errorOffset, NULL);

    if (_re != NULL) {
        _isValid = true;
    }
    else {
        _isValid = false;
    }
}

void
Regex::compile (const std::string& regex, const std::string& opts)
{
    _opts = _parseOptions(opts);

    this->compile(regex);
}

int
Regex::match (const char* string, unsigned int offset)
{
    return this->match((std::string) string, offset);
}

int
Regex::match (const std::string& string, unsigned int offset)
{
    size_t msize = NULL;
    pcre_fullinfo(_re, _extra, PCRE_INFO_CAPTURECOUNT, &msize);
    int* m = new int[(msize = 3 * (msize + 1))];
    int* p;

    std::vector<Markers> marks;

    if (_p_lastMatched != (void*) &string) {
        _lastPosition = 0;
    }

    if (_isGlobal) {
        offset += _lastPosition;
    }
    
    _groups.clear();
    _matchesNumber = pcre_exec(_re, _extra, string.c_str(), string.length(), offset, 0, m, msize);

    int i;
    for (i = 0, p = m; i < _matchesNumber; i++, p += 2) {
        marks.push_back(Markers(p[0], p[1]));
    }

    delete[] m;

    _marks         = marks;
    _lastMatched   = string;
    _p_lastMatched = (void*) &string;

    if (_isGlobal) {
        if (_matchesNumber == PCRE_ERROR_NOMATCH) {
            _lastPosition = 0; // reset the position for next match (perl does this)
        }
        else if (_matchesNumber > 0) {
            _lastPosition = marks.at(0).second; // increment by the end of the match
        }
        else {
            _lastPosition = 0;
        }
    }

    int rValue = 0;
    if (_matchesNumber > 0) {
        rValue = _matchesNumber;

        size_t i;
        for (i = 0; i < _marks.size(); i++) {
            int begin = _marks.at(i).first;

            if (begin == -1) {
                _groups.push_back("");
                continue;
            }

            int end = _marks.at(i).second;

            _groups.push_back(string.substr(begin, end-begin));

            _globalGroups.clear(); _globalGroups = _groups;
        }
    }

    return rValue;
}

int
Regex::match (const char* regex, const char* string)
{
    return Regex::match((std::string) regex, (std::string) string);
}

int
Regex::match (const std::string& regex, const std::string& string)
{
    Regex re(regex);
    return re.match(string);
}

int
Regex::matches (void)
{
    return _matchesNumber;
}

std::string
Regex::group (int index)
{
    return _groups.at(index);
}

std::string
Regex::Group (int index)
{
    return _globalGroups.at(index);
}

std::string
Regex::sub (const char* replace, const char* string, bool backref)
{
    return this->sub((std::string) replace, (std::string) string, backref);
}

std::string
Regex::sub (const std::string& replace, const std::string& string, bool backref)
{
    std::stringstream stream;

    if (_isGlobal) {
        int lastMatch = 0;

        while (this->match(string)) {
            stream << string.substr(lastMatch, _marks.at(0).first - lastMatch);

            std::string replacement = replace;

            if (backref) {
                replacement = _updateReplacement(replacement);
            }

            stream << replacement;

            lastMatch = _marks.at(0).second;
        }

        stream << string.substr(lastMatch);
    }
    else {
        int matchesNumber = this->match(string);

        if (matchesNumber > 0) {
            std::string replacement = replace;

            if (backref) {
                replacement = _updateReplacement(replacement);
            }

            stream << string.substr(0, _marks.at(0).first);
            stream << replacement;
            stream << string.substr(_marks.at(0).second);
        }
        else {
            stream << string;
        }
    }

    return stream.str();
}

bool
Regex::isValid (void)
{
    return _isValid;
}

std::string
Regex::operator [] (int index)
{
    return this->group(index);
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
    std::memcpy(newRe, re, size);

    return newRe;
}

std::string
Regex::_updateReplacement (const std::string& replace)
{
    std::string replacement = replace;

    Regex backrefs("\\$(\\d+)", "g");

    int backrefsNumber;
    while (backrefsNumber = backrefs.match(replace)) {
        std::stringstream reText;
        reText << "[$]" << backrefs[1];
        Regex backrefsSub(reText.str());

        replacement = backrefsSub.sub(replacement, (*this)[std::atoi(backrefs[1].c_str())], false);
    }

    return replacement;
}

int
operator ^= (std::string string, const char* regex)
{
    return Regex::match((std::string) regex, string);
}

int
operator ^= (const char* string, std::string regex)
{
    return Regex::match(regex, (std::string) string);
}

int
operator ^= (std::string string, std::string regex)
{
    return Regex::match(regex, string);
}

