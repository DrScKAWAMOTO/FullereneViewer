/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __STACK_H__
#define __STACK_H__

#include "List.h"

template <class T> class Stack : public List<T> {
  // friend classes & functions

  // members

  // private tools

  // constructors & the destructor
public:
  Stack() : List<T>() { }
  Stack(const Stack<T>& you) : List<T>()
  {
    List<T>::add(you);
  }
  ~Stack() { }
  Stack<T>& operator = (const Stack<T>& you)
  {
    if (this != &you)
      {
        List<T>::p_clean();
        List<T>::add(you);
      }
    return *this;
  }

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  void push(T* element)
  {
    add(element);
  }
  T* pop()
  {
    if (List<T>::p_length == 0)
      return 0;
    return List<T>::remove(List<T>::p_length - 1);
  }
  T* refer_top()
  {
    if (List<T>::p_length == 0)
      return 0;
    return operator [] (List<T>::p_length - 1);
  }
};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
