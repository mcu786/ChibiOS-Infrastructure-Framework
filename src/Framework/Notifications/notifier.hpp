/*
 * notifier.hpp
 *
 *  Created on: Apr 10, 2012
 *      Author: mabl
 */

#ifndef NOTIFIER_HPP_
#define NOTIFIER_HPP_

#include "ch.h"
#include "mail.h"

#include "Utilities/singleLinkedList.hpp"

namespace fwk {

template<class MsgType>
class Listener;


/**
 * @brief Internally used notification Message.
 *
 * @details
 *      The @p NotifierMsg is used to exchange messages between the @p
 *      Notifier and the multiple @p Listener.
 *
 *      It tracks if the memory of the message is still needed.
 */

template <class MsgType> class NotifierMsg {
public:

  /**
   * @brief Initialize the @p NotifierMsg.
   *
   * @details
   *    Must be called after the memory has been allocated for it. Resets
   *    the usage counters etc.
   */
  void init();

  /**
   * @brief Decrease usage counter and free if not used anymore.
   */
  void tryFree();

  /** @brief Message saved in @p NotifierMsg. */
  MsgType msg;

  /** @brief Usage counter. */
  int usedCntr;

  /** @brief Pointer to memory pool used to create this @p NotifierMsg. */
  MailPool* mailPool;
};



/**
 * @brief @p Notifier provides multiple messages to associated  @p Listener.
 *
 * @details
 *      The  @p Notifier holds a memory pool to create @p NotifierMsg
 *      messages which create a copy of the @p MsgType object.
 *
 *      These messages are sent to the mailboxes of all registered @p Listener
 *      as long as there is space in their mailbox.
 *
 *      The  @p NotifierMsg  are freed once all @p Listener have answered their
 *      mail.
 *
 * @note @p StaticNotifier should be preferred of @p Notifier in normal
 *       applications.
 *
 * @todo Implement a blocking broadcast method
 * @todo Implement a "would block" check
 *
 */
template<class MsgType>
class Notifier {

public:

  /**
   * @brief Constructor which requires a pointer to the memory provided
   *        for the memory pool.
   *
   * @param poolBuffer  Pointer to array of elements of the size of
   *                    MEM_ALIGN_NEXT(sizeof(NotifierMsg<MsgType>)).
   * @param n           Number of elements available in the @p poolBuffer.
   */
  Notifier(void* poolBuffer, size_t n);


  /**
   * @brief Broadcast message to registered @p Listener.
   *
   * @note The broadcast is guaranteed to NOT block. Blocking can either occur
   *       the @p Notifier message pool is exhausted or if the mail box of a
   *       @p Listener is full.
   *
   * @param[in] msg     Holds the message which is to be broadcasted.
   * @return RDY_OK     If all @p Listener were notified.
   * @return RDY_RESET  If at least one @p Listener got no message
   */
  msg_t broadcast(const MsgType& msg);

  /**
   * @brief Register a @p Listener to the Notifier
   * @param[in] listener    Listener which is to be registered
   */
  void registerListener(Listener<MsgType>& listener);

  /**
   * @brief Unregister a @p Listener from the Notifier.
   * @param[in] listener    Listener which is to be unregistered.
   *
   * @note You have to unregister a Listener before it is destroyed.
   *
   * @todo Finish this function.
   */
  void unregisterListener(Listener<MsgType>& listener);

private:
  /** @brief Guarded memory pool for creation of messages */
  MailPool mailPool;

  /** @brief Linked list holding all the registered @p Listener. */
  utl::LinkedList<Listener<MsgType> *> listenersList;
};


/**
 * @brief @p StaticNotifier extends @p Notifier and provides its own memory
 *        space.
 */
template<class MsgType, int N>
class StaticNotifier : public Notifier<MsgType>{

public:
  StaticNotifier(void);

private:
  /** @brief Memory pool for mailbox */
  uint8_t buffer[N*MEM_ALIGN_NEXT(sizeof(NotifierMsg<MsgType>))]
                              __attribute__((aligned(sizeof(stkalign_t))));
};


/**
 * @brief A @p Listener object receives messages from the @p Notifier object
 *        which it has been registered to.
 *
 * @details
 *      The @p Listener can receive as many messages as there are mailboxes
 *      registered to it. One one single message is retrieved at each time.
 *
 * @note Care should be taken to release a message as soon as it is not needed
 *       anymore since it will take memory until all @p Listener have released
 *       the message.
 *
 * @note @p StaticListener should be preferred of @p Listener in normal
 *       applications.
 */
template<class MsgType> class Listener {
public:
  /**
   * @brief Constructor which requires a pointer to the buffer space of the
   *        message mailbox.
   *
   * @details The size of the buffer @p n directly maps to the number of
   *        messages the @p Listener can receive.
   *
   * @param buf Buffer of type msg_t
   * @param n   Size of buffer
   */
  Listener(msg_t *buf, cnt_t n);


  /**
   * @brief Get the number of messages already in the mailbox.
   * @return Number of messages.
   */
  int getSize();

  /**
   * @brief Get first message.
   * @return Pointer to first message.
   *
   * @post You will have to free the message with @p releaseData().
   */
  const MsgType* getData();

  /**
   * @brief Release the last received message.
   *
   * @pre You first have to get the message with @p getData.
   * @warning Attempts to release a message without having called @p getData()
   *          is illegal.
   */
  void releaseData();

protected:
  /** @brief Node which is registered when registering */
  utl::LinkedListNode<Listener<MsgType>* > node;

  /** @brief Mailbox used to receive the messages of the @p Notifier*/
  Mailbox mailbox;

  /** @brief Tracker for the currently retrieved message */
  NotifierMsg<MsgType>* lastMsg;

  // The corresponding notifier is allowed
  friend class Notifier<MsgType>;
};


/**
 * @brief @p StaticListener extends @p Listener and provides its own memory
 *        space.
 */
template<class MsgType, int N>
class StaticListener : public Listener<MsgType>{

public:
  StaticListener();

private:
  /** @brief buffer for @p N Mailboxes */
  msg_t buffer[N] __attribute__((aligned(sizeof(stkalign_t))));
};

}

// Include implementation
#include "Framework/Notifications/notifier_imp.hpp"

#endif /* NOTIFIER_HPP_ */
