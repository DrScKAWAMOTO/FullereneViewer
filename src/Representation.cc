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
  : p_last_char(0), p_last_count(0)
{
}

Representation::~Representation()
{
}

int Representation::compare(const Representation* you) const
{
  RepresentationInfo* my_info = get_info(0);
  RepresentationInfo* your_info = you->get_info(0);
  if (my_info->clockwise != your_info->clockwise)
    return your_info->clockwise - my_info->clockwise;
  return strcmp((char*)p_repres, (char*)you->p_repres);
}

void Representation::print() const
{
  RepresentationInfo* info = get_info(0);
  printf("Clockwise(%d) Carbon(%d) Bond(%d) Same(%d) %s\n",
         info->clockwise, info->carbon_sequence_no, info->bond_sequence_no,
         number_of_infos(), (char*)p_repres);
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
  if (p_last_count <= 0)
    return;
  p_repres.append_char(p_last_char);
  if (p_last_count >= 2)
    p_repres.append_int(p_last_count);
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
