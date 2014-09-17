/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 16:57:28 JST
 */

#ifndef __SET_H__
#define __SET_H__

#include "List.h"

template <class T> class Set : public List<T> {
  // friend classes & functions

  // members

  // private tools
private:
  bool p_search(const T* element, int& result_index)
  {
    List<T>::p_make_room();
    int min = -1;
    int max = List<T>::p_length;
    while (min + 1 < max)
      {
        int mid = (min + max) / 2;
        int cmp = element->compare(List<T>::p_container[mid]);
        if (cmp == 0)
          {
            result_index = mid;
            return true;
          }
        if (cmp > 0)
          min = mid;
        else
          max = mid;
      }
    result_index = max;
    return false;
  }

  // constructors & the destructor
public:
  Set() : List<T>() { }
  Set(const Set<T>& you) : List<T>()
  {
    List<T>::add(you);
  }
  ~Set() { }
  Set<T>& operator = (const Set<T>& you)
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
  T* search(const T* element)
  {
    int result_index;
    if (p_search(element, result_index))
      return List<T>::p_container[result_index];
    return 0;
  }
  T* search_else_add(T* element)
  {
    int result_index;
    if (p_search(element, result_index))
      return List<T>::p_container[result_index];
    for (int i = List<T>::p_length; i > result_index; --i)
      List<T>::p_container[i] = List<T>::p_container[i - 1];
    List<T>::p_length++;
    element->link_up();
    List<T>::p_container[result_index] = element;
    return 0;
  }
  bool remove(T* element)
  {
    int result_index;
    if (!p_search(element, result_index))
      return false;
    --List<T>::p_length;
    for ( ; result_index < List<T>::p_length; ++result_index)
      List<T>::p_container[result_index] = List<T>::p_container[result_index + 1];
    element->link_down();
    return true;
  }

};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
