/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/16 17:07:43 JST
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Generator.h"
#include "Utils.h"
#include "DebugMemory.h"

void Generator::p_enlarge()
{
  if (p_history_length == p_array_length)
    {
      char* new_history = new char[p_array_length * 2];
      for (int i = 0; i < p_array_length; ++i)
        new_history[i] = p_history[i];
      delete[] p_history;
      p_history = new_history;
      p_array_length *= 2;
    }
}

void Generator::p_glow(int value)
{
  if (p_history_offset == p_history_length)
    {
      p_enlarge();
      ++p_history_length;
    }
  p_history[p_history_offset++] = value;
}

Generator::Generator(bool symmetric, int maximum_vertices_of_polygons)
  : p_type(symmetric ? GENERATOR_TYPE_SYMMETRIC : GENERATOR_TYPE_ORDINARY),
    p_scrap_no(1), p_n(0), p_m(0), p_h(0), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_array_length(16), p_history_length(0), p_history_offset(0),
    p_history(new char[p_array_length])
{
}

Generator::Generator(int n, int m, int h, int maximum_vertices_of_polygons)
  : p_type(GENERATOR_TYPE_TUBE), p_scrap_no(0), p_n(n), p_m(m), p_h(h),
    p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_array_length(16), p_history_length(0), p_history_offset(0),
    p_history(new char[p_array_length])
{
}

Generator::Generator(const char* generator_formula, int maximum_vertices_of_polygons)
  : p_type(GENERATOR_TYPE_ORDINARY), p_scrap_no(0), p_n(0), p_m(0), p_h(0),
    p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_array_length(16), p_history_length(0), p_history_offset(0),
    p_history(new char[p_array_length])
{
  const char* ptr = generator_formula;
  switch (*ptr++)
    {
    case 'A':
      p_scrap_no = character_to_No(*ptr++);
      break;
    case 'T':
      p_type = GENERATOR_TYPE_TUBE;
      p_n = strtol(ptr, (char**)&ptr, 10);
      if (*ptr == ',')
        ++ptr;
      p_m = strtol(ptr, (char**)&ptr, 10);
      if (*ptr == ',')
        ++ptr;
      p_h = strtol(ptr, (char**)&ptr, 10);
      break;
    case 'S':
      p_type = GENERATOR_TYPE_SYMMETRIC;
      p_scrap_no = character_to_No(*ptr++);
      break;
    default:
      p_type = GENERATOR_TYPE_ILLEGAL;
      break;
    }
  if (*ptr == '-')
    ++ptr;
  while (1)
    {
      if (*ptr == '\0')
        break;
      int No = digits10x10_to_No(ptr);
      int count = digits26x7_to_No(ptr);
      for (int i = 0; i < count; ++i)
        p_glow(No);
    }
  p_history_offset = 0;
}

Generator::Generator(const Generator& you)
  : p_type(you.p_type),
    p_scrap_no(you.p_scrap_no), p_n(you.p_n), p_m(you.p_m), p_h(you.p_h),
    p_minimum_polygons(you.p_minimum_polygons),
    p_maximum_vertices_of_polygons(you.p_maximum_vertices_of_polygons),
    p_array_length(you.p_array_length), p_history_length(you.p_history_length),
    p_history_offset(you.p_history_offset), p_history(new char[p_array_length])
{
  for (int i = 0; i < p_array_length; ++i)
    p_history[i] = you.p_history[i];
}

Generator::~Generator()
{
  delete[] p_history;
}

bool Generator::next_pattern()
{
  p_history_offset = p_history_length - 1;
  while (1)
    {
      p_history[p_history_offset]++;
      if (p_history[p_history_offset] <= p_maximum_vertices_of_polygons)
        {
          p_history_offset = 0;
          return true;
        }
      else
        {
          if (p_history_length == 1)
            {
              if (p_type != GENERATOR_TYPE_SYMMETRIC)
                return false;
              if (p_scrap_no == 4)
                return false;
              p_scrap_no++;
              p_history_length = 0;
              p_history_offset = 0;
              return true;
            }
          --p_history_length;
          p_history_offset = p_history_length - 1;
        }
    }
}

int Generator::glow()
{
  if (p_history_offset == p_history_length)
    {
      p_enlarge();
      ++p_history_length;
      p_history[p_history_offset] = p_minimum_polygons;
    }
  return p_history[p_history_offset++];
}

int Generator::history()
{
  if (p_history_offset == p_history_length)
    return -1;
  return p_history[p_history_offset++];
}

void Generator::get_generator_formula(char* buffer, int length)
{
  if (p_type == GENERATOR_TYPE_TUBE)
    sprintf(buffer, "T%d,%d,%d-", p_n, p_m, p_h);
  else if (p_type == GENERATOR_TYPE_SYMMETRIC)
    sprintf(buffer, "S%d-", p_scrap_no);
  else
    sprintf(buffer, "A%d-", p_scrap_no);
  int len = strlen(buffer);
  length -= len;
  buffer += len;
  assert(length > p_history_length);
  int current_No = 0;
  int count = 0;
  for (int i = 0; i < p_history_length; ++i)
    {
      if (current_No == p_history[i])
        ++count;
      else
        {
          if (count)
            No_No_to_digits10x10_digits26x7(current_No, count, buffer);
          current_No = p_history[i];
          count = 1;
        }
    }
  No_No_to_digits10x10_digits26x7(current_No, count, buffer);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
