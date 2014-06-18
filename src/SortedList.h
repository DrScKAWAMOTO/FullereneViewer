/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __SORTEDLIST_H__
#define __SORTEDLIST_H__

#include "List.h"

template <class T> class SortedList : public List<T> {
  // friend classes & functions

  // members

  // private tools

  // constructors & the destructor
public:
  SortedList() : List<T>() { }
  SortedList(const SortedList<T>& that) : List<T>()
  {
    p_add(that);
  }
  ~SortedList() { }
  SortedList<T>& operator = (const SortedList<T>& that)
  {
    if (this != &that)
      {
        List<T>::p_clean();
        List<T>::p_add(that);
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
    List<T>::p_make_room();
    int min = -1;
    int max = List<T>::p_length;
    while (min + 1 < max)
      {
        int mid = (min + max) / 2;
        int cmp = element->compare(List<T>::p_container[mid]);
        if (cmp == 0)
          cmp = List<T>::p_container[mid] - element;
        if (cmp > 0)
          min = mid;
        else
          max = mid;
      }
    for (int i = List<T>::p_length; i > max; --i)
      List<T>::p_container[i] = List<T>::p_container[i - 1];
    List<T>::p_length++;
    element->link_up();
    List<T>::p_container[max] = element;
  }
};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
