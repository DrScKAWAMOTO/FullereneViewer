/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 22:30:13 JST
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Representation.h"
#include "DebugMemory.h"

Representation::Representation()
  : p_array_length(16), p_offset(0), p_array(new char[p_array_length])
{
}

Representation::~Representation()
{
  delete[] p_array;
}

bool Representation::operator == (const Representation& that) const
{
  return (strcmp(p_array, that.p_array) == 0);
}

bool Representation::operator == (const List<Representation>& that) const
{
  int len = that.length();
  for (int i = 0; i < len; ++i)
    {
      if ((*this) == (*that[i]))
        return true;
    }
  return false;
}

void Representation::print() const
{
  RepresentationInfo* info = get_info(0);
  printf("Clockwise(%d) Carbon(%d) Bond(%d) Same(%d) %s\n",
         info->clockwise, info->carbon_sequence_no, info->bond_sequence_no,
         number_of_infos(), p_array);
}

int Representation::number_of_infos() const
{
  return p_infos.length();
}

void Representation::add_info(RepresentationInfo* info)
{
  p_infos.add(info);
}

RepresentationInfo* Representation::get_info(int index) const
{
  assert((index >= 0) && (index < number_of_infos()));
  return p_infos[index];
}

void Representation::set_step(char step)
{
  if (p_array_length <= p_offset + 1)
    {
      int new_length = p_array_length * 2;
      char* new_array = new char[new_length];
      strcpy(new_array, p_array);
      delete[] p_array;
      p_array = new_array;
      p_array_length = new_length;
    }
  p_array[p_offset++] = step;
  p_array[p_offset] = '\0';
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
