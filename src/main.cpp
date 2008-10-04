#include <iostream>
#include <string>
#include <sstream>
#include <getopt.h>

#include "Server.h"

static std::string showHelp(void);

int main (int argc, char *argv[])
{
    int cmd;

    while ((cmd = getopt(argc, argv, "f:m:hvVDpt")) != -1) {
        switch (cmd) {
            case 'h':
            std::cout << showHelp() << std::endl;
            return 0;
            break;

            default:
            std::cout << showHelp() << std::endl;
            return -1;
            break;
        }
    }

    return 0;
}

std::string showHelp (void)
{
    std::stringstream help;

    help << "lulzHTTPd - The webserver for the lulz."                              << std::endl;
    help << ""                                                                     << std::endl;
    help << "Usage:"                                                               << std::endl;
    help << "  -f <name>  filename of the config-file"                             << std::endl;
    help << "  -m <name>  module directory (default: dunno .-.)"                   << std::endl;
    help << "  -p         print the parsed config-file in internal form, and exit" << std::endl;
    help << "  -t         test the config-file, and exit"                          << std::endl;
    help << "  -D         don't go to background (default: go to background)"      << std::endl;
    help << "  -v         show version"                                            << std::endl;
    help << "  -V         show compile-time features"                              << std::endl;
    help << "  -h         show this help"                                          << std::endl;

    return help.str();
}
