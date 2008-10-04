#if !defined(LULZHTTPD_CONFIG_H)
#define LULZHTTPD_CONFIG_H

#include "common.h"

class Config
{
  public:
    static Config* instance (void);
    static void init (String& configFile);

    static bool test (String& configFile);
    static String testLog (void);

  private:
    Config (String& configFile);

  private:
    Config* _object;
    bool _inited;
};

#endif
