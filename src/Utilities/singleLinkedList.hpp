/*
 * singleLinkedList.hpp
 *
 *  Created on: Apr 10, 2012
 *      Author: mabl
 */

#ifndef SINGLELINKEDLIST_HPP_
#define SINGLELINKEDLIST_HPP_

#include "ch.h"

namespace utl {

template <class T>
class LinkedList;

/**
 * @brief Simple Node which can be linked with @p LinkedList.
 */
template <class T>
class LinkedListNode{

public:
  LinkedListNode(T v) : next(0), nodeVal(v){}

  /**
   * @brief return the value stored in the node.
   * @return    Value stored in
   */
  inline T get() const {return nodeVal;}

  /**
   * @brief Get the next Node linked to this node.
   * @return    The next linked Node
   */
  inline LinkedListNode<T>* getNext() const { return next;}

private:
  LinkedListNode<T>* next;
  T nodeVal;

  friend class LinkedList<T>;
};

/**
 * @brief Simple Linked List implementation.
 *
 * @note This implementation is neither finished not complete and might be
 *       replaced by proper code some day.
 */
template <class T>
class LinkedList {

public:
  LinkedList() : head(0), tail(0) {
    chMtxInit(&mtxListChanges);
  }

  /**
   * @brief Check if the @p LinkedList is empty.
   * @return true if empty.
   */
  bool empty() const {return (!head && !tail); }


  /**
   * @brief Get a pointer to the tail of the list.
   * @return Pointer to tail of list.
   * @return 0 if the list is empty
   */
  LinkedListNode<T>* getTail() const {return tail;};

  /**
   * @brief Get a pointer to the head of the list.
   * @return Pointer to tail of list.
   * @return 0 if the list is empty
   */
  LinkedListNode<T>* getHead() const {return head;};

  /**
   * @brief Push a node at the end of the linked list
   * @param[in] node
   */
  void push_back(LinkedListNode<T>& node);

  /**
   * @brief Remove a node from the list independent of where it is.
   * @param[in] node
   *
   * @todo Thorough testing of this awful function!
   */
  void remove(LinkedListNode<T>& node);

private:
  LinkedListNode<T>* head;
  LinkedListNode<T>* tail;

  Mutex mtxListChanges;

};

template <class T>
void LinkedList<T>::push_back(LinkedListNode<T>& node){

  chMtxLock(&mtxListChanges);

  if(tail)
    tail->next = &node;
  else
    head = &node;

  tail = &node;
  node.next = 0;
  chMtxUnlock();
}

template <class T>
void LinkedList<T>::remove(LinkedListNode<T>& node){

  LinkedListNode<T>** previousNode = &head;


  for(LinkedListNode<T>** currNode = previousNode;
      *currNode; currNode = &((*currNode)->next)){

    if (*currNode == &node){
      chMtxLock(&mtxListChanges);

      // Update tail if we have remove the last element of the list
      if(!(*currNode)->next){
        tail = *previousNode;
      }

      // Take node out of the list
      *previousNode = (*currNode)->next;

      // Update tail if we deleted the last node
      if(tail == &node){
        tail = *previousNode;
      }

      chMtxUnlock();
      break;
    }



    previousNode = currNode;
  }

}


}


#endif /* SINGLELINKEDLIST_HPP_ */
