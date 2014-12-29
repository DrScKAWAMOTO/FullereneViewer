/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "GeneratorLine.h"
#include "DebugMemory.h"
#include "Utils.h"

int GeneratorLine::s_next_sequence = 1;

bool GeneratorLine::p_get_number_NoA_isS_isT_and_isA(int& number, int& NoA,
                                                     bool& isS, bool& isT,
                                                     bool& isA) const
{
  char* ptr = p_line + 1;
  number = strtol(ptr, &ptr, 10);
  if (strncmp(ptr, " (NoA=", 6) != 0)
    return false;
  ptr += 6;
  NoA = strtol(ptr, &ptr, 10);
  if (NoA < 1)
    return false;
  while ((*ptr != ')') && *ptr)
    ++ptr;
  if (strncmp(ptr, ") S", 3) == 0)
    {
      isS = true;
      isT = false;
      isA = false;
    }
  else if (strncmp(ptr, ") A", 3) == 0)
    {
      isS = false;
      isT = false;
      isA = true;
    }
  else if (strncmp(ptr, ") T", 3) == 0)
    {
      isS = false;
      isT = true;
      isA = false;
    }
  else
    return false;
  return true;
}

void GeneratorLine::p_get_n_m_and_h(int& n, int& m, int& h) const
{
  char* ptr = p_line;
  while (*ptr != ')')
    ++ptr;
  ptr += 3;
  n = strtol(ptr, &ptr, 10);
  if (*ptr == ',')
    ++ptr;
  m = strtol(ptr, &ptr, 10);
  if (*ptr == ',')
    ++ptr;
  h = strtol(ptr, &ptr, 10);
}


bool GeneratorLine::p_get_scrap(int& scrap) const
{
  char* ptr = p_line;
  while (*ptr != ')')
    ++ptr;
  ptr += 2;
  bool symmetric;
  if (ptr[0] == 'S')
    symmetric = true;
  else if (ptr[0] == 'A')
    symmetric = false;
  else
    return false;
  ptr++;
  scrap = strtol(ptr, &ptr, 10);
  if (symmetric)
    {
      if ((scrap < 1) || (scrap > 9))
        return false;
    }
  else
    {
      if ((scrap < 0) || (scrap > 1))
        return false;
    }
  return true;
}

GeneratorLine::GeneratorLine(const char* line)
  : Object(s_next_sequence++), p_line(line)
{
}

GeneratorLine::~GeneratorLine()
{
}

int GeneratorLine::compare(const GeneratorLine* you) const
{
  int mynumber;
  int myNoA;
  bool myS;
  bool myT;
  bool myA;
  int yournumber;
  int yourNoA;
  bool yourS;
  bool yourT;
  bool yourA;
  bool result;
  result = p_get_number_NoA_isS_isT_and_isA(mynumber, myNoA, myS, myT, myA);
  assert(result);
  result = you->p_get_number_NoA_isS_isT_and_isA(yournumber, yourNoA,
                                                 yourS, yourT, yourA);
  assert(result);
  if (mynumber != yournumber)
    return mynumber - yournumber;
  if (myNoA != yourNoA)
    return yourNoA - myNoA;
  if (( myS) && (!yourS))
    return -1;
  if (( myT) && ( yourA))
    return -1;
  if ((!myS) && ( yourS))
    return 1;
  if (( myA) && ( yourT))
    return 1;
  if (( myT) && ( yourT))
    {
      int myn, mym, myh;
      int yourn, yourm, yourh;
      p_get_n_m_and_h(myn, mym, myh);
      you->p_get_n_m_and_h(yourn, yourm, yourh);
      if (myn != yourn)
        return myn - yourn;
      if (mym != yourm)
        return mym - yourm;
      if (myh != yourh)
        return yourh - myh;
    }
  else
    {
      int myscrap;
      int yourscrap;
      result = p_get_scrap(myscrap);
      assert(result);
      result = you->p_get_scrap(yourscrap);
      assert(result);
      if (myscrap != yourscrap)
        return myscrap - yourscrap;
    }
  char* ptr = p_line;
  while (*ptr != ')')
    ++ptr;
  if (*ptr == ')')
    ++ptr;
  if (*ptr == ' ')
    ++ptr;
  char* yptr = you->p_line;
  while (*yptr != ')')
    ++yptr;
  if (*yptr == ')')
    ++yptr;
  if (*yptr == ' ')
    ++yptr;
  int comp_result = compare_generator_formula(ptr, yptr);
  if (comp_result != 0)
    return comp_result;
  int mySequenceNo = sequence_no();
  int yourSequenceNo = you->sequence_no();
  return mySequenceNo - yourSequenceNo;
}

bool GeneratorLine::same_number_NoA(const GeneratorLine* you) const
{
  int mynumber;
  int myNoA;
  bool myS;
  bool myT;
  bool myA;
  int yournumber;
  int yourNoA;
  bool yourS;
  bool yourT;
  bool yourA;
  bool result;
  result = p_get_number_NoA_isS_isT_and_isA(mynumber, myNoA, myS, myT, myA);
  assert(result);
  result = you->p_get_number_NoA_isS_isT_and_isA(yournumber, yourNoA,
                                                 yourS, yourT, yourA);
  assert(result);
  if (mynumber != yournumber)
    return false;
  if (myNoA != yourNoA)
    return false;
  return true;
}

void GeneratorLine::print(FILE* output) const
{
  fprintf(output, "%s\n", (char*)p_line);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
