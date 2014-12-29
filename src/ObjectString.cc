/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ObjectString.h"

ObjectString::ObjectString()
{
}

ObjectString::ObjectString(const char* value) : MyString(value)
{
}

ObjectString::ObjectString(const ObjectString& you) : MyString((char*)you)
{
}

#if 0
ObjectString& ObjectString::operator = (const char* value)
{
  MyString::operator = (value);
  return *this;
}

ObjectString& ObjectString::operator = (const ObjectString& you)
{
  MyString::operator = ((char*)you);
  return *this;
}
#endif

ObjectString::~ObjectString()
{
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
