/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "DistanceSummaryLine.h"
#include "DebugMemory.h"
#include "Utils.h"

int DistanceSummaryLine::s_next_sequence = 1;

bool DistanceSummaryLine::p_get_number(const char*& ptr, int& number)
{
  while (*ptr == ' ')
    ++ptr;
  if (!isdigit(*ptr))
    return false;
  number = atoi(ptr);
  while (isdigit(*ptr))
    ++ptr;
  return ((*ptr == ' ') || (!*ptr)) ? true : false;
}

DistanceSummaryLine::DistanceSummaryLine(const char* line)
  : Object(s_next_sequence++)
{
  assert(line);
  assert(line[0]);
  p_line = copy_string(line);
}

DistanceSummaryLine::~DistanceSummaryLine()
{
  if (p_line)
    delete[] p_line;
}

int DistanceSummaryLine::compare(const DistanceSummaryLine* you) const
{
  const char* myline = p_line;
  const char* yourline = you->p_line;
  while (1)
    {
      if (!*myline && !*yourline)
        return 0;
      int mynumber = 0;
      int yournumber = 0;
      p_get_number(myline, mynumber);
      p_get_number(yourline, yournumber);
      if (mynumber != yournumber)
        return yournumber - mynumber;
    }
}

void DistanceSummaryLine::print() const
{
  printf("%s\n", p_line);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
