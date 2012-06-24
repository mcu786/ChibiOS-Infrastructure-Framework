
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
    palClearPad(LED_STATUS1_PORT, LED_STATUS1_PIN);
    chThdSleepMilliseconds(500);
    palSetPad(LED_STATUS1_PORT, LED_STATUS1_PIN);
    chThdSleepMilliseconds(500);
  }

  return RDY_OK;
}

}

