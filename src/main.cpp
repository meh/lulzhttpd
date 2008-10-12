/****************************************************************************
* This file is part of lulzHTTPd.                                           *
* Copyleft meh.                                                             *
*                                                                           *
* lulzHTTPd is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzHTTPd is distributed in the hope that it will be useful.              *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzHTTPd.  If not, see <http://www.gnu.org/licenses/>.        *
****************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <getopt.h>

#include "Server.h"
#include "Config.h"

static std::string showHelp (void);
static std::string showVersion (void);

int main (int argc, char *argv[])
{
    bool daemonize = true;

    bool testConfig   = false;
    String configFile = "/etc/lulzhttpd/lulz.conf";

    int cmd;
    while ((cmd = getopt(argc, argv, "f:m:hvVDpt")) != -1) {
        switch (cmd) {
        case 'h':
        std::cout << showHelp() << std::endl;
        return 0;
        break;

        case 'f':
        configFile = optarg;
        break;

        case 't':
        testConfig = false;
        break;

        case 'D':
        daemonize = false;
        break;

        case 'v':
        std::cout << showVersion() << std::endl;
        return 0;
        break;

        default:
        std::cout << showHelp() << std::endl;
        return -1;
        break;
        }
    }

    if (testConfig) {
        try {
            lulzHTTPd::Config::test(configFile);
            std::cout << lulzHTTPd::Config::testLog() << std::endl;
        }
        catch (std::exception e) {
            std::cerr << e.what() << std::endl;
        }

        return 0;
    }

    try {
        lulzHTTPd::Server::init(configFile);
        lulzHTTPd::Server::start();
    }
    catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

std::string showHelp (void)
{
    std::stringstream help;

    help << "lulzHTTPd - The webserver for the lulz."                              << std::endl;
    help << ""                                                                     << std::endl;
    help << "Usage:"                                                               << std::endl;
    help << "  -f <name>  filename of the config-file."                            << std::endl;
    help << "  -m <name>  module directory (default: dunno .-.)."                  << std::endl;
    help << "  -p         print the parsed config-file in internal form, and exit" << std::endl;
    help << "  -t         test the config-file, and exit."                         << std::endl;
    help << "  -D         don't go to background (default: go to background)"      << std::endl;
    help << "  -v         show version."                                           << std::endl;
    help << "  -V         show compile-time features."                             << std::endl;
    help << "  -h         show this help."                                         << std::endl;

    return help.str();
}

std::string showVersion (void)
{
    std::stringstream version;

    version << "THE GAME" << std::endl;

    return version.str();
}
