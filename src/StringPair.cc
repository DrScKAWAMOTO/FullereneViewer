/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/09/11 20:46:28 JST
 */

#include <string.h>
#include <assert.h>
#include "StringPair.h"

StringPair::StringPair(const char* my_string, const char* your_string)
  : p_my_string(0), p_your_string(0)
{
  int len = strlen(my_string) + 1;
  assert(len > 0);
  char* ptr = new char[len];
  strcpy(ptr, my_string);
  p_my_string = ptr;
  len = strlen(your_string) + 1;
  assert(len > 0);
  ptr = new char[len];
  strcpy(ptr, your_string);
  p_your_string = ptr;
}

StringPair::~StringPair()
{
  if (p_my_string)
    delete[] p_my_string;
  if (p_your_string)
    delete[] p_your_string;
}

int StringPair::compare(const StringPair* you) const
{
  return strcmp(p_my_string, you->p_my_string);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
