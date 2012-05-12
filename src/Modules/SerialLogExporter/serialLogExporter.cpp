
#include "ch.h"
#include "hal.h"
#include "framework.h"

#include "Modules/SerialLogExporter/serialLogExporter.hpp"
#include "Framework/Logging/logging.hpp"
#include "Framework/Notifications/notifier.hpp"

namespace SerialLogExporter {

msg_t SerialLogExporter::init(){
  fwk::logger.notifier.registerListener(listener);

  return RDY_OK;
}

msg_t SerialLogExporter::Main(){

  chRegSetThreadName("SerialLogExporter");

  while (true){
    const fwk::LoggingMsg* msg = listener.getData();

    switch (msg->level) {

      case fwk::LoggingMsg::eDebug:
        chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t*) "[Debug] ", 8);
        break;

      case fwk::LoggingMsg::eInfo:
        chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t*) "[Info]  ", 8);
        break;

      case fwk::LoggingMsg::eError:
        chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t*) "[Error] ", 8);
        break;

      case fwk::LoggingMsg::eHalt:
        chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t*) "[Halt]  ", 8);
        break;
    }

    chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t *) msg->msg,
                            strlen(msg->msg));

    chSequentialStreamWrite(&FWK_LOG_SERIAL_OUT, (const uint8_t *)"\r\n", 2);

    listener.releaseData();

  }

  return RDY_OK;
}

}

