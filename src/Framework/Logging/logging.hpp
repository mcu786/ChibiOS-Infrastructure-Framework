/*
 * logging.hpp
 *
 *  Created on: May 10, 2012
 *      Author: mabl
 */

#ifndef LOGGING_HPP_
#define LOGGING_HPP_

#include <cstring>
#include "ch.h"
#include "Framework/Notifications/notifier.hpp"


namespace fwk {



struct LoggingMsg {
  enum LogLevel {eInfo, eDebug, eError, eHalt};
  enum LogMsgType {eCopy, eNoCopy};

  LogLevel level;
  char* msg;
  bool needsFree;

  LoggingMsg() : level(eInfo), msg(0), needsFree(false) {}

  ~LoggingMsg(){
    // Free memory if needed
    if(needsFree) {
      chHeapFree( (void*) msg);
    }
  }
};

template<int NLogListener, int LogBufferSize>
class Logging {

public:
  Logging() {
    chHeapInit(&logBufferHeap, &logBuffer, logBufferSize);
  }

  int log(LoggingMsg::LogLevel level, const char* msg, LoggingMsg::LogMsgType type);


public:
  fwk::StaticNotifier<LoggingMsg, NLogListener> notifier;

private:

  static const size_t logBufferSize = MEM_ALIGN_NEXT(LogBufferSize);
  char logBuffer[logBufferSize] __attribute__((aligned(sizeof(stkalign_t))));
  MemoryHeap logBufferHeap;

};



template<int NLogListener, int LogBufferSize>
int Logging<NLogListener,LogBufferSize>::log(LoggingMsg::LogLevel level,
                                                   const char* msg,
                                                   LoggingMsg::LogMsgType type){

  NotifierMsg<LoggingMsg>* notificMsg;
  notificMsg = notifier.acquireMsg(TIME_INFINITE);
  notificMsg->msg.level = level;

  switch (type) {
    case LoggingMsg::eCopy:
      notificMsg->msg.msg = (char*) chHeapAlloc(&logBufferHeap, strlen(msg));

      // Could not allocate memory
      if(!notificMsg->msg.msg) {
        notificMsg->tryFree();
        return RDY_RESET;
      }

      strcpy(notificMsg->msg.msg, msg);
      notificMsg->msg.needsFree = true;
      break;

    case LoggingMsg::eNoCopy:
      notificMsg->msg.msg = const_cast<char*>(msg);
      notificMsg->msg.needsFree = false;
      break;
  }


  notifier.broadcastMsg(notificMsg, TIME_INFINITE);

  return 0;

}


typedef Logging<1, 64> Logger;
extern Logger logger;

}

#endif /* LOGGING_HPP_ */
