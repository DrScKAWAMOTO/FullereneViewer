/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/09/11 21:52:51 JST
 */

#ifndef __STRINGPAIR_H__
#define __STRINGPAIR_H__

#include "Object.h"

class StringPair : public Object {
  // friend classes & functions

  // members
private:
  const char* p_my_string;
  const char* p_your_string;

  // private tools

  // constructors & the destructor
public:
  StringPair(const char* my_string, const char* your_string);
  virtual ~StringPair();
  StringPair& operator = (const StringPair& you); /* dont use */

  // type converters

  // comparators
public:
  int compare(const StringPair* you) const;

  // math operators

  // I/O

  // class decision

  // member accessing methods
  const char* my_string() const { return p_my_string; }
  const char* your_string() const { return p_your_string; }

};

#endif /* __STRINGPAIR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
