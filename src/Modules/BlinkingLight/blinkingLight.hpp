#ifndef BLINKINGLIGHT_HPP_
#define BLINKINGLIGHT_HPP_

#include "Framework/Module/vmodule.hpp"


namespace BlinkingLight{

  /**
   * @brief Example module implementing a blinking light
   *
   * @pre This module does not depend on any other module
   */
  class BlinkingLight : public fwk::VModuleThread<128> {

  public:
    virtual msg_t init();

    virtual msg_t Main();

    virtual ~BlinkingLight(){};
  };

}


#endif /* BLINKINGLIGHT_HPP_ */
