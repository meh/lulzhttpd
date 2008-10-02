/****************************************************************************
* Copyleft meh.                                                            *
*                                                                           *
* This is free software: you can redistribute it and/or modify              *
* it under the terms of the GNU Affero General Public License as            *
* published by the Free Software Foundation, either version 3 of the        *
* License, or (at your option) any later version.                           *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Affero General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Affero General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/

//
// This is a simple PCRE wrapper.
//
// Here are some examples:
//
// MATCHING
// -------
// Regex re("^(\\d+)$", "");
// re.match("666");
//
// std::cout << re.group(1) << std::endl;
// -------
//
// This short source outputs 666.
//
// You can also use it in a perl-like way:
//
// -------
// std::string("666") ^= "/^(\\d+)$/";
// std::cout << $(1) << std::endl;
// -------
//
// The only limitation with perl-like regex is that you can't loop through
// /g regular expressions, you must use the normal way (it's a bug that
// i'll try to fix)
//
// REPLACING
// -------
// Regex.re("LOL", "");
// std::string replace = re.sub("OMG", "LOL OMG LOL LOL LOL");
//
// std::cout << replace << std::endl;
// -------
// That will output OMG OMG OMG OMG OMG
//
// Perl-like way:
// -------
// std::string replace = std::string("LOL OMG LOL LOL LOL") &= "/LOL/OMG/g";
// std::cout << replace << std::endl;
// -------
//
// Legend:
// ^= | Matches, it's like =~ m// in perl.
// &= | Replace, it's like =~ s/// in perl.
//
// To compile a source that uses Regex.h you have to link libpcre.
//
// ie: g++ -lpcre -o program program.cpp
//

#if !defined(REGEXPP)
#define REGEXPP

#include <iostream>
#include <sstream>

#include <cstdlib>
#include <cstring>
#include <string>

#include <vector>
#include <map>

#include <pcre.h>

#define $(n) Regex::Group(n)

class Regex
{
  public:
    typedef std::pair<int, int> Markers;
    typedef std::vector<std::string> Strings;

  public:
    Regex (void)
    {
        this->reset();
        _opts  = PCRE_CASELESS | PCRE_EXTENDED;
        _re    = NULL;
        _extra = NULL;

        _lastPosition = 0;
        _isGlobal     = false;

        _matchesNumber = 0;
    };

    Regex (const char* regex)
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
    };
    
    Regex (const std::string& regex)
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
    };

    Regex (const char* regex, unsigned int opts)
    {
        this->_init((std::string) regex, opts);
    };
    
    Regex (const char* regex, const char* opts)
    {
        this->_init((std::string) regex, (std::string) opts);
    };

    Regex (const std::string& regex, unsigned int opts)
    {
        this->_init(regex, opts);
    };
    
    Regex (const std::string& regex, const std::string& opts)
    {
        this->_init(regex, opts);
    };
    
    Regex (const Regex& re) {};

    ~Regex (void)
    {
        if (_re) {
            pcre_free(_re);
        }
    };

    unsigned int options (void)
    {
        return _opts;
    };

    bool isGlobal (void)
    {
        return _isGlobal;
    };

    void options (unsigned int opts)
    {
        _opts = opts;
    };

    void options (const char* opts)
    {
        _opts = _parseOptions((std::string) opts);
    };

    void options (const std::string& opts)
    {
        _opts = _parseOptions(opts);
    };

    void compile (const char* regex, unsigned int opts = -1)
    {
        this->compile((std::string) regex, opts);
    };
    
    void compile (const char* regex, const char* opts)
    {
        _opts = _parseOptions((std::string) opts);

        this->compile((std::string) regex);
    };

    void compile (const std::string& regex, unsigned int opts = -1)
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
    };
    
    void compile (const std::string& regex, const std::string& opts)
    {
        _opts = _parseOptions(opts);

        this->compile(regex);
    };

    int match (const char* string, unsigned int offset = 0)
    {
        return this->match((std::string) string, offset);
    };
    
    int match (const std::string& string, unsigned int offset = 0)
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
    };

    static int Match (const char* regex, const char* string)
    {
        return Regex::Match((std::string) regex, (std::string) string);
    };
    
    static int Match (const std::string& regex, const std::string& string)
    {
        Regex re(regex);
        return re.match(string);
    };

    int matches (void)
    {
        return _matchesNumber;
    };

    std::string group (int index)
    {
        return _groups.at(index);
    };
    
    static std::string Group (int index)
    {
        return _globalGroups.at(index);
    };

    std::string sub (const char* replace, const char* string, bool backref = true)
    {
        return this->sub((std::string) replace, (std::string) string, backref);
    };
    
    std::string sub (const std::string& replace, const std::string& string, bool backref = true)
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
    };

    static std::string Sub (const char* sub, const char* string)
    {
        return Regex::Sub((std::string) sub, (std::string) string);
    };
    
    static std::string Sub (const std::string& sub, const std::string& string)
    {
        std::map<std::string, std::string> data = Regex::_parseSub(sub);
        Regex re(data["regex"]);
        
        return re.sub(data["replace"], string, true);
    };

    bool isValid (void)
    {
        return _isValid;
    };

    void reset (void)
    {
        _p_lastMatched = NULL;
        _lastMatched   = "";

        _marks.clear();
        _lastPosition = 0;

        _isValid = false;
    };

    std::string operator [] (int index)
    {
        return this->group(index);
    };

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

    Strings _groups;
    static Strings _globalGroups;

  protected:
    static pcre* _clonePCRE (pcre* re)
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
    };

    unsigned int _parseOptions (const std::string& opts)
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
    };

    static std::map<std::string, std::string> _parseSub (const std::string& sub)
    {
        Regex re("^[s]?/(.*?)/(.*?)/(\\w*)$", "");
        re.match(sub);

        std::map<std::string, std::string> data;
        data["regex"]   = "/" + re[1] + "/" + re[3];
        data["replace"] = re[2];

        return data;
    };

    std::string _updateReplacement (const std::string& replace)
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
    };

  private:
    void _init (const std::string& regex, unsigned int opts)
    {
        this->reset();
        _isGlobal = false;

        _opts = opts;
        this->compile(regex);
        _extra = NULL;

        _lastPosition  = 0;
        _matchesNumber = 0;
    };

    void _init (const std::string& regex, const std::string& opts)
    {
        this->reset();
        _isGlobal = false;

        _opts = this->_parseOptions(opts);
        this->compile(regex);
        _extra = NULL;

        _lastPosition  = 0;
        _matchesNumber = 0;
    };
};

Regex::Strings Regex::_globalGroups;

/// Match
int operator ^= (std::string string, const char* regex)
{
    return Regex::Match((std::string) regex, string);
}

int operator ^= (const char* string, std::string regex)
{
    return Regex::Match(regex, (std::string) string);
}

int operator ^= (std::string string, std::string regex)
{
    return Regex::Match(regex, string);
}

/// Substitute
std::string operator &= (std::string string, const char* regex)
{
    return Regex::Sub((std::string) regex, string);
}

std::string operator &= (const char* string, std::string regex)
{
    return Regex::Sub(regex, (std::string) string);
}

std::string operator &= (std::string string, std::string regex)
{
    return Regex::Sub(regex, string);
}

#endif
