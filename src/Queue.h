/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <assert.h>
#include "DebugMemory.h"

#define QUEUE_IMPL_CONTAINER_SIZE 10

// implementation inner classes
template <class T> class QueueImpl {
  // members
public:
  QueueImpl<T>* p_next;
  T* p_container[QUEUE_IMPL_CONTAINER_SIZE];

  // constructors & the destructor
public:
  QueueImpl() : p_next(0) { }
  ~QueueImpl() { }
  QueueImpl<T>& operator = (const QueueImpl<T>& you); /* dont use */
};

template <class T> class Queue {
  // friend classes & functions

  // members
protected:
  QueueImpl<T>* p_top;
  QueueImpl<T>* p_bottom;
  int p_length;
  int p_enq_offset_in_bottom;
  int p_deq_offset_in_top;

  // protected tools

  // constructors & the destructor
public:
  Queue() : p_top(new QueueImpl<T>()), p_length(0),
            p_enq_offset_in_bottom(0), p_deq_offset_in_top(0) { p_bottom = p_top; }
  ~Queue() { clean(); delete p_top; }
  Queue<T>& operator = (const Queue<T>& you); /* dont use */

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  void enqueue(T* element)
  {
    element->link_up();
    p_bottom->p_container[p_enq_offset_in_bottom++] = element;
    if (p_enq_offset_in_bottom == QUEUE_IMPL_CONTAINER_SIZE)
      {
        p_bottom->p_next = new QueueImpl<T>();
        p_bottom = p_bottom->p_next;
        p_enq_offset_in_bottom = 0;
      }
    ++p_length;
  }
  T* dequeue()
  {
    if (p_length == 0)
      return 0;
    --p_length;
    T* element = p_top->p_container[p_deq_offset_in_top++];
    element->link_down();
    if (p_deq_offset_in_top == QUEUE_IMPL_CONTAINER_SIZE)
      {
        if (p_top->p_next == 0)
          {
            assert(p_length == 0);
            p_enq_offset_in_bottom = 0;
            p_deq_offset_in_top = 0;
          }
        else
          {
            QueueImpl<T>* work = p_top;
            p_top = p_top->p_next;
            delete work;
            p_deq_offset_in_top = 0;
          }
      }
    return element;
  }
  void clean() { while (dequeue()) /* do nothing */; }
  int length() const { return p_length; }
};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
