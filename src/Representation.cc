/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Representation.h"
#include "DebugMemory.h"
#include "Config.h"

Representation::Representation()
  : p_last_char(0), p_last_count(0),
    p_array_length(16), p_offset(0), p_array(new char[p_array_length])
{
}

Representation::~Representation()
{
  delete[] p_array;
}

int Representation::compare(const Representation* you) const
{
  RepresentationInfo* my_info = get_info(0);
  RepresentationInfo* your_info = you->get_info(0);
  if (my_info->clockwise != your_info->clockwise)
    return your_info->clockwise - my_info->clockwise;
  return strcmp(p_array, you->p_array);
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

void Representation::p_flush_step()
{
  char work[100];
  if (p_last_count <= 0)
    return;
  if (p_last_count >= 2)
    sprintf(work, "%c%d", p_last_char, p_last_count);
  else
    sprintf(work, "%c", p_last_char);
  int length = strlen(work);
  if (p_array_length <= p_offset + length)
    {
      int new_length = p_array_length * 2;
      char* new_array = new char[new_length];
      strcpy(new_array, p_array);
      delete[] p_array;
      p_array = new_array;
      p_array_length = new_length;
    }
  strcpy(p_array + p_offset, work);
  p_offset += length;
  p_last_char = 0;
  p_last_count = 0;
}

void Representation::set_step(char step)
{
  if (p_last_char && (p_last_char != step))
    p_flush_step();
  p_last_char = step;
  ++p_last_count;
}

void Representation::finish_step()
{
  if (p_last_char)
    p_flush_step();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
