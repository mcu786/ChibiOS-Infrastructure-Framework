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


/**
 * @brief   @p LoggingMsg is used to carry the log message details via the
 *          Notification framework.
 *
 */
struct LoggingMsg {
  /**
   * @brief Logging message error level.
   */
  enum LogLevel {eDebug, eInfo, eError, eHalt};
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
  Logging() : mtx(_MUTEX_DATA(mtx)), minimalLogLevel(LoggingMsg::eInfo){
    chHeapInit(&logBufferHeap, &logBuffer, logBufferSize);
  }

  /**
   * @brief     Send a debug message to the listening loggers.
   *
   * @details   Only messages which exceed or are equal to the minimal log
   *            level are promoted to the loggers.
   *
   * @param level   Log level of the message.
   * @param msg     \0 teminated message.
   * @param type    Type of the message i.e. if it needs to be copied.
   * @return
   */
  int log(LoggingMsg::LogLevel level, const char* msg, LoggingMsg::LogMsgType type);

  /**
   * @brief Get the minimal log level.
   * @return        Minimal log level.
   */
  LoggingMsg::LogLevel getMinimalLogLevel();

  /**
   * @brief Set the minimal log level,
   * @param minimalLogLevel Minimal log level to be set
   */
  void setMinimalLogLevel(LoggingMsg::LogLevel minimalLogLevel);

public:
  fwk::StaticNotifier<LoggingMsg,NLogListener> notifier;

private:
  Mutex mtx;    // Protection of member variables

  LoggingMsg::LogLevel minimalLogLevel;

  static const size_t logBufferSize = MEM_ALIGN_NEXT(LogBufferSize);
  char logBuffer[logBufferSize] __attribute__((aligned(sizeof(stkalign_t))));
  MemoryHeap logBufferHeap;
};



template<int NLogListener, int LogBufferSize>
int Logging<NLogListener,LogBufferSize>::log(LoggingMsg::LogLevel level,
                                                   const char* msg,
                                                   LoggingMsg::LogMsgType type){

  if (level < getMinimalLogLevel())
    return 0;

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

template<int NLogListener, int LogBufferSize>
void Logging<NLogListener,LogBufferSize>::setMinimalLogLevel(LoggingMsg::LogLevel minimalLogLevel) {
  chMtxLock(&mtx);
  this->minimalLogLevel = minimalLogLevel;
  chMtxUnlock();
}

template<int NLogListener, int LogBufferSize>
LoggingMsg::LogLevel Logging<NLogListener,LogBufferSize>::getMinimalLogLevel() {

  chMtxLock(&mtx);
  LoggingMsg::LogLevel tmp = minimalLogLevel;
  chMtxUnlock();

  return tmp;
}


typedef Logging<1, 64> Logger;
extern Logger logger;

}

#endif /* LOGGING_HPP_ */
