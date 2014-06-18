/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __LIST_H__
#define __LIST_H__

#include "DebugMemory.h"

template <class T> class List {
  // friend classes & functions

  // members
protected:
  int p_container_size;
  int p_length;
  T** p_container;

  // protected tools
protected:
  void p_make_room()
  {
    if (p_length == p_container_size)
      {
        p_container_size *= 2;
        T** new_ptr = new T*[p_container_size];
        for (int i = 0; i < p_length; ++i)
          new_ptr[i] = p_container[i];
        delete[] p_container;
        p_container = new_ptr;
      }
  }
  void p_clean()
  {
    for (int i = 0; i < p_length; ++i)
      p_container[i]->link_down();
    p_length = 0;
  }
  void p_add(const List<T>& that)
  {
    int len = that.length();
    for (int i = 0; i < len; ++i)
      add(that[i]);
  }

  // constructors & the destructor
public:
  List() : p_container_size(16), p_length(0), p_container(new T*[p_container_size]) { }
  List(const List<T>& that) : p_container_size(16), p_length(0),
                              p_container(new T*[p_container_size])
  {
    p_add(that);
  }
  ~List() { p_clean(); delete[] p_container; }
  List<T>& operator = (const List<T>& that)
  {
    if (this != &that)
      {
        p_clean();
        p_add(that);
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
  virtual void add(T* element)
  {
    p_make_room();
    element->link_up();
    p_container[p_length++] = element;
  }
  void remove(T* element)
  {
    for (int i = 0; i < p_length; ++i)
      {
        if (p_container[i] == element)
          {
            --p_length;
            for (i = i; i < p_length; ++i)
              p_container[i] = p_container[i + 1];
            element->link_down();
            return;
          }
      }
  }
  void clean() { p_clean(); }
  int length() const { return p_length; }
  T* operator[](int offset) const
  {
    if ((offset < 0) || (p_length <= offset))
      return 0;
    return p_container[offset];
  }
};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
