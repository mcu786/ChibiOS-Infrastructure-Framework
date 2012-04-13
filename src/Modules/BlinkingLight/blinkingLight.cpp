
#include "ch.h"
#include "hal.h"

#include "Modules/BlinkingLight/blinkingLight.hpp"

namespace BlinkingLight {

msg_t BlinkingLight::init(){
  // No need to do anything for initialization.
  return RDY_OK;
}

msg_t BlinkingLight::Main(){

  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearPad(GPIOC, GPIOC_LED_STATUS1);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOC, GPIOC_LED_STATUS1);
    chThdSleepMilliseconds(500);
  }

  return RDY_OK;
}

}

