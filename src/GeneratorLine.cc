/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/15 10:35:57 JST
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "GeneratorLine.h"
#include "DebugMemory.h"

int GeneratorLine::s_next_sequence = 1;

bool GeneratorLine::p_get_number_and_NoA(int& number, int& NoA) const
{
  char* ptr = p_line + 1;
  number = strtol(ptr, &ptr, 10);
  if (number < 60)
    return false;
  if (strncmp(ptr, " (NoA=", 6) != 0)
    return false;
  ptr += 6;
  NoA = strtol(ptr, &ptr, 10);
  return (NoA >= 1) ? true : false;
}

GeneratorLine::GeneratorLine(const char* line)
  : Object(s_next_sequence++)
{
  int len = strlen(line) + 1;
  p_line = new char[len];
  strcpy(p_line, line);
}

GeneratorLine::~GeneratorLine()
{
  if (p_line)
    delete[] p_line;
}

int GeneratorLine::compare(const GeneratorLine* that) const
{
  int mynumber;
  int myNoA;
  int yournumber;
  int yourNoA;
  assert(p_get_number_and_NoA(mynumber, myNoA));
  assert(that->p_get_number_and_NoA(yournumber, yourNoA));
  if (mynumber != yournumber)
    return mynumber - yournumber;
  if (myNoA != yourNoA)
    return yourNoA - myNoA;
  int mySequenceNo = sequence_no();
  int yourSequenceNo = that->sequence_no();
  return mySequenceNo - yourSequenceNo;
}

void GeneratorLine::print() const
{
  printf("%s\n", p_line);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
