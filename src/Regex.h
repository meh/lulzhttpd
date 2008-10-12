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
// ie: g++ -lpcre -o program Regex.cpp program.cpp
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

typedef std::pair<int, int> Markers;
typedef std::vector<std::string> Strings;

class Regex
{
  public:
    Regex (void);
    Regex (const char* regex);
    Regex (const std::string& regex);
    Regex (const std::string& regex, unsigned int opts);
    Regex (const std::string& regex, const std::string& opts);
    
    ~Regex (void);

    unsigned int options (void);
    
    bool isGlobal (void);

    void options (unsigned int opts);
    void options (const std::string& opts);
    
    void compile (const std::string& regex, unsigned int opts = -1);
    void compile (const std::string& regex, const std::string& opts);
    
    int match (const std::string& string, unsigned int offset = 0);
    static int Match (const std::string& regex, const std::string& string);
    
    int matches (void);

    std::string group (int index);
    static std::string Group (int index);
    
    std::string sub (const std::string& replace, const std::string& string, bool backref = true);
    static std::string Sub (const std::string& sub, const std::string& string);

    Strings split (const std::string& string);
    static Strings Split (const std::string& regex, const std::string& string);
    
    bool isValid (void);
    
    void reset (void);
    
    std::string operator [] (int index);
    
  protected:
    std::string _regex;

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
    static pcre* _clonePCRE (pcre* re);
    
    unsigned int _parseOptions (const std::string& opts);
    
    static std::map<std::string, std::string> _parseSub (const std::string& sub);
    
    std::string _updateReplacement (const std::string& replace);

  private:
    void _init (const std::string& regex, unsigned int opts);
    void _init (const std::string& regex, const std::string& opts);
};



/// Match
int operator ^= (std::string string, const char* regex);
int operator ^= (const char* string, std::string regex);
int operator ^= (std::string string, std::string regex);

/// Substitute
std::string operator &= (std::string string, const char* regex);
std::string operator &= (const char* string, std::string regex);
std::string operator &= (std::string string, std::string regex);

#endif
