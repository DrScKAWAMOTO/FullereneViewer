/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __OBJECTINT2_H__
#define __OBJECTINT2_H__

#include "Object.h"

class ObjectInt2 : public Object {
  // friend classes & functions

  // members
private:
  int p_key;
  int p_value;

  // private tools

  // constructors & the destructor
public:
  ObjectInt2() : p_key(0), p_value(0) { }
  ObjectInt2(int key, int value) : p_key(key), p_value(value) { }
  ObjectInt2(const ObjectInt2& you) : p_key(you.p_key), p_value(you.p_value) { }
  ~ObjectInt2() { }

  // comparators
public:
  int compare(const ObjectInt2* you) const { return p_key - you->p_key; }

  // I/O

  // member accessing methods
public:
  int get_key() const { return p_key; }
  int get_value() const { return p_value; }

};

#endif /* __OBJECTINT2_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
