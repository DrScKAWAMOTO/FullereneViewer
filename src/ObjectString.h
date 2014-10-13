/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __OBJECTSTRING_H__
#define __OBJECTSTRING_H__

#include "Object.h"
#include "MyString.h"

class ObjectString : public Object, MyString {
  // friend classes & functions

  // members

  // private tools

  // constructors & the destructor
public:
  ObjectString();
  ObjectString(const char* value);
  ObjectString(const ObjectString& you);
#if 0
  ObjectString& operator = (const char* value);
  ObjectString& operator = (const ObjectString& you);
#endif
  ~ObjectString();

  // comparators
public:
  int compare(const ObjectString* you) const
  { return MyString::compare((const MyString*)you); }

  // I/O

  // member accessing methods
public:
  operator char*() const { return MyString::operator char*(); }

};

#endif /* __OBJECTSTRING_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
