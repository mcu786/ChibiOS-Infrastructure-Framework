#ifndef SIMPLEWEBSERVER_HPP_
#define SIMPLEWEBSERVER_HPP_

#include "Framework/Module/vmodule.hpp"

#ifndef WEB_THREAD_STACK_SIZE
#define WEB_THREAD_STACK_SIZE   1024
#endif

#ifndef WEB_THREAD_PORT
#define WEB_THREAD_PORT         80
#endif

#ifndef WEB_THREAD_PRIORITY
#define WEB_THREAD_PRIORITY     (LOWPRIO + 2)
#endif

namespace SimpleWebserver {

  /**
   * @brief Example implementation of the standard minimal LwIP HTTP server.
   *
   * @pre This module does not depend on any other module
   */
  class SimpleWebserver : public fwk::VModuleThread<WEB_THREAD_STACK_SIZE> {

  public:
    virtual msg_t init();

    virtual msg_t Main();

    virtual ~SimpleWebserver(){};
  };

}


#endif /* SIMPLEWEBSERVER_HPP_ */
