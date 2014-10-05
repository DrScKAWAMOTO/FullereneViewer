/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/03/25 23:32:50 JST
 */

#include "Pattern.h"
#include "DebugMemory.h"

Pattern::Pattern(int length, int* values)
  : p_length(length), p_offset(0), p_pattern(new int[p_length])
{
  for (int i = 0; i < p_length; ++i)
    p_pattern[i] = values[i];
}

Pattern::Pattern(int value0)
  : p_length(1), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
}

Pattern::Pattern(int value0, int value1)
  : p_length(2), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
  p_pattern[1] = value1;
}

Pattern::Pattern(int value0, int value1, int value2)
  : p_length(3), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
  p_pattern[1] = value1;
  p_pattern[2] = value2;
}

Pattern::Pattern(int value0, int value1, int value2, int value3)
  : p_length(4), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
  p_pattern[1] = value1;
  p_pattern[2] = value2;
  p_pattern[3] = value3;
}

Pattern::Pattern(int value0, int value1, int value2, int value3, int value4)
  : p_length(5), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
  p_pattern[1] = value1;
  p_pattern[2] = value2;
  p_pattern[3] = value3;
  p_pattern[4] = value4;
}

Pattern::Pattern(int value0, int value1, int value2, int value3, int value4, int value5)
  : p_length(6), p_offset(0), p_pattern(new int[p_length])
{
  p_pattern[0] = value0;
  p_pattern[1] = value1;
  p_pattern[2] = value2;
  p_pattern[3] = value3;
  p_pattern[4] = value4;
  p_pattern[5] = value5;
}

Pattern::~Pattern()
{
  if (p_pattern)
    delete[] p_pattern;
}

int Pattern::get_value_then_next()
{
  int value = p_pattern[p_offset];
  if (p_offset != p_length - 1)
    p_offset++;
  return value;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
