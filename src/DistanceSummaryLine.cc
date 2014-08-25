/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/06 15:42:58 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "DistanceSummaryLine.h"

int DistanceSummaryLine::s_next_sequence = 1;

bool DistanceSummaryLine::p_get_distance_and_number(const char*& ptr,
                                                    int& distance, int& number)
{
  while (*ptr == ' ')
    ++ptr;
  if (!isdigit(*ptr))
    return false;
  number = atoi(ptr);
  while (isdigit(*ptr))
    ++ptr;
  if (*ptr != '@')
    return false;
  ++ptr;
  if (!isdigit(*ptr))
    return false;
  distance = atoi(ptr);
  while (isdigit(*ptr))
    ++ptr;
  return ((*ptr == ' ') || (!*ptr)) ? true : false;
}

DistanceSummaryLine::DistanceSummaryLine(const char* line)
  : Object(s_next_sequence++)
{
  assert(line);
  assert(line[0]);
  int len = strlen(line) + 1;
  p_line = new char[len];
  strcpy(p_line, line);
}

DistanceSummaryLine::~DistanceSummaryLine()
{
  if (p_line)
    delete[] p_line;
}

int DistanceSummaryLine::compare(const DistanceSummaryLine* that) const
{
  const char* myline = p_line;
  const char* yourline = that->p_line;
  while (1)
    {
      if (!*myline && !*yourline)
        return 0;
      assert(*myline && *yourline);
      int mydistance;
      int mynumber;
      int yourdistance;
      int yournumber;
      assert(p_get_distance_and_number(myline, mydistance, mynumber));
      assert(p_get_distance_and_number(yourline, yourdistance, yournumber));
      if (mydistance != yourdistance)
        return mydistance - yourdistance;
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
