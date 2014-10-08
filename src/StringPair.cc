/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <assert.h>
#include "StringPair.h"
#include "DebugMemory.h"
#include "Utils.h"

StringPair::StringPair(const char* my_string, const char* your_string)
  : p_my_string(0), p_your_string(0)
{
  p_my_string = copy_string(my_string);
  p_your_string = copy_string(your_string);
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
