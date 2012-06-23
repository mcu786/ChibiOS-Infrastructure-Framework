/*
 * notifiert_imp.hpp
 *
 *  Created on: Apr 10, 2012
 *      Author: mabl
 */

#ifndef NOTIFIER_IMP_HPP_
#define NOTIFIER_IMP_HPP_

#include "Framework/Notifications/notifier.hpp"
#include "Utilities/singleLinkedList.hpp"

using namespace fwk;


template <class MsgType>
void NotifierMsg<MsgType>::init() {
  usedCntr = 0;
}

template <class MsgType>
void NotifierMsg<MsgType>::tryFree() {
  int cntr;

  chSysLock();
  cntr = --usedCntr;
  chSysUnlock();

  if(cntr == 0) {
    // Invoke destructor explicitly
    this->~NotifierMsg<MsgType>();

    mailDelete(mailPool, (void*) this);
  }
}


template <class MsgType>
Notifier<MsgType>::Notifier(void* poolBuffer, size_t n) {

  // Init mail pool
  mailInit(&mailPool, sizeof(NotifierMsg<MsgType>), poolBuffer, n);
}

template <class MsgType>
void Notifier<MsgType>::registerListener(Listener<MsgType>& listener){
  listenersList.push_back(listener.node);
}

template <class MsgType>
void Notifier<MsgType>::unregisterListener(Listener<MsgType>& listener){
  listenersList.remove(listener.node);
}

template <class MsgType>
NotifierMsg<MsgType>* Notifier<MsgType>::acquireMsg(systime_t timeout){

  NotifierMsg<MsgType>* msgCopy = (NotifierMsg<MsgType>*) mailCreate(&mailPool, timeout);

  // Out of pool memory
  if(!msgCopy)
    return 0;

  // Call init
  msgCopy->init();
  msgCopy->mailPool = &mailPool;

  return msgCopy;
}

template <class MsgType>
msg_t Notifier<MsgType>::broadcastMsg(NotifierMsg<MsgType>* msg,
                                          systime_t timeout){


  msg_t errorState = RDY_OK;

  for(utl::LinkedListNode<Listener<MsgType>* >* n = listenersList.getHead();
      n; n = n->getNext()){

    Listener<MsgType>* listener = n->get();

    chSysLock();

    msg_t rdymsg = chMBPostS(&listener->mailbox, (msg_t) msg, timeout);

    if (rdymsg == RDY_OK){
      msg->usedCntr++;
    } else {
      errorState = RDY_RESET;
    }

    chSysUnlock()

  }

  return errorState;

}

template <class MsgType>
msg_t Notifier<MsgType>::broadcast(const MsgType& msg, systime_t timeout){

  // If there is no listener -> skip all the stuff
  if(listenersList.empty())
    return RDY_OK;

  NotifierMsg<MsgType>* msgCopy = acquireMsg(timeout);

  // Out of pool memory
  if(!msgCopy)
    return RDY_RESET;

  // Assign message to new copy of object
  msgCopy->msg = msg;

  return broadcastMsg(msgCopy, timeout);

}

template<class MsgType, int N>
StaticNotifier<MsgType, N>::StaticNotifier(void) :
  Notifier<MsgType>((void *) buffer, N){

}


template <class MsgType>
Listener<MsgType>::Listener(msg_t *buf, cnt_t n) :
  node(this), lastMsg(0){

  chMBInit(&mailbox, buf, n);
}

template <class MsgType>
int Listener<MsgType>::getSize(){
  chSysLock();
  const int useCount = chMBGetUsedCountI(&mailbox);
  chSysUnlock();

  return useCount;
}

template <class MsgType>
const MsgType* Listener<MsgType>::getData(){
  NotifierMsg<MsgType>* msg;

  msg_t err = chMBFetch(&mailbox, (msg_t*) &msg, TIME_INFINITE);

  if(err != RDY_OK)
    return 0;

  // Remember this message so that it can be freed again
  chDbgAssert(!lastMsg, "Listener<MsgType>::getData", "Last msg not freed!");
  lastMsg = msg;

  return &msg->msg;
}

template <class MsgType>
void Listener<MsgType>::releaseData(){
  chDbgAssert(lastMsg, "Listener<MsgType>::releaseData", "No msg to free");

  lastMsg->tryFree();
  lastMsg = 0;
}

template <class MsgType, int N>
StaticListener<MsgType, N>::StaticListener() : Listener<MsgType>(buffer, N){

}
#endif /* NOTIFIER_IMP_HPP_ */
