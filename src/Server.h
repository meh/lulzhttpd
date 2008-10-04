#if !defined(LULZHTTPD_SERVER_H)
#define LULZHTTPD_SERVER_H

#include "common.h"

namespace lulzHTTPd {

class Server
{
  public:
    static Server* instance (void);

    static void init (void);

  protected:
    Server (void) {};

  private:
    static Server* _object;
    static bool _initied;
};

};

#endif
