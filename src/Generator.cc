/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/16 17:07:43 JST
 */

#include <stdio.h>
#include <assert.h>
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
  : p_scrap_no(1), p_symmetric(symmetric), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_array_length(16), p_history_length(0), p_history_offset(0),
    p_history(new char[p_array_length])
{
}

Generator::Generator(const char* generator_label, int maximum_vertices_of_polygons)
  : p_scrap_no(character_to_No(generator_label[1])),
    p_symmetric(generator_label[0] == 'S'), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_array_length(16), p_history_length(0), p_history_offset(0),
    p_history(new char[p_array_length])
{
  const char* ptr = generator_label + 3;
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
              if (!p_symmetric)
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

void Generator::get_generator_label(char* buffer, int length)
{
  assert(p_history_length + 4 <= length);
  *buffer++ = p_symmetric ? 'S' : 'A';
  *buffer++ = No_to_character(p_scrap_no);
  *buffer++ = '-';
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
