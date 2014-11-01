/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __OBJECTINT_H__
#define __OBJECTINT_H__

#include "Object.h"

class ObjectInt : public Object {
  // friend classes & functions

  // members
private:
  int p_value;

  // private tools

  // constructors & the destructor
public:
  ObjectInt() : p_value(0) { }
  ObjectInt(int value) : p_value(value) { }
  ObjectInt(const ObjectInt& you) : p_value(you.p_value) { }
  ~ObjectInt() { }

  // comparators
public:
  int compare(const ObjectInt* you) const { return p_value - you->p_value; }

  // I/O

  // member accessing methods
public:
  int get_value() const { return p_value; }

};

#endif /* __OBJECTINT_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
