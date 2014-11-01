/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "Generator.h"
#include "Utils.h"
#include "Debug.h"
#include "DebugMemory.h"

void Generator::p_glow(int value)
{
  if (p_history_offset == p_history_length)
    {
      if (p_history_length == p_history.length())
        p_history.resize(p_history_length + 1);
      ++p_history_length;
    }
  p_history.set_char_at(p_history_offset++, value);
}

Generator::Generator(bool symmetric, int maximum_vertices_of_polygons)
  : Object(), p_type(symmetric ? GENERATOR_TYPE_SYMMETRIC : GENERATOR_TYPE_ORDINARY),
    p_scrap_no(1), p_n(0), p_m(0), p_h(0), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_state(GENERATOR_STATE_START_FROM_MINIMUM),
    p_history_length(0), p_history_offset(0)
{
}

Generator::Generator(int n, int m, int h, int maximum_vertices_of_polygons)
  : Object(), p_type(GENERATOR_TYPE_TUBE), p_scrap_no(0),
    p_n(n), p_m(m), p_h(h), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_state(GENERATOR_STATE_START_FROM_MINIMUM),
    p_history_length(0), p_history_offset(0)
{
}

Generator::Generator(const char* generator_formula, int maximum_vertices_of_polygons)
  : Object(), p_type(GENERATOR_TYPE_ORDINARY), p_scrap_no(0),
    p_n(0), p_m(0), p_h(0), p_minimum_polygons(5),
    p_maximum_vertices_of_polygons(maximum_vertices_of_polygons),
    p_state(GENERATOR_STATE_START_FROM_MINIMUM),
    p_history_length(0), p_history_offset(0)
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
  if (p_history_length > 0)
    p_state = GENERATOR_STATE_START_SPECIFIED;
}

Generator::Generator(const Generator& you)
  : Object(you), p_type(you.p_type),
    p_scrap_no(you.p_scrap_no), p_n(you.p_n), p_m(you.p_m), p_h(you.p_h),
    p_minimum_polygons(you.p_minimum_polygons),
    p_maximum_vertices_of_polygons(you.p_maximum_vertices_of_polygons),
    p_state(you.p_state),
    p_history_length(you.p_history_length), p_history_offset(you.p_history_offset),
    p_history(you.p_history)
{
}

Generator& Generator::operator = (const Generator& you)
{
  if (this != &you)
    {
      p_type = you.p_type;
      p_scrap_no = you.p_scrap_no;
      p_n = you.p_n;
      p_m = you.p_m;
      p_h = you.p_h;
      p_minimum_polygons = you.p_minimum_polygons;
      p_maximum_vertices_of_polygons = you.p_maximum_vertices_of_polygons;
      p_state = you.p_state;
      p_history_length = you.p_history_length;
      p_history_offset = you.p_history_offset;
      p_history = you.p_history;
    }
  return *this;
}

Generator::~Generator()
{
}

void Generator::print_detail() const
{
  MyString buffer;
  get_generator_formula(buffer);
  printf("%s\n", (char*)buffer);
}

void Generator::print_progress(int length) const
{
  if (p_history_offset <= length)
    {
      MyString buffer;
      get_generator_formula(buffer, false);
      struct rusage ru;
      if (getrusage(RUSAGE_SELF, &ru) == 0)
        {
          int sec;
          int usec;
          sec = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
          usec = ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;
          if (usec >= 1000000)
            {
              sec++;
              usec -= 1000000;
            }
          printf("pg$ %s %f\n", (char*)buffer, sec + (double)usec / 1000000.0);
        }
      else
        printf("pg$ %s\n", (char*)buffer);
    }
}

int Generator::glow_step()
{
  if (p_history_offset >= p_history_length - 1)
    {
      if (p_history_length == p_history.length())
        p_history.resize(p_history_length + 1);
      p_state = GENERATOR_STATE_START_FROM_MINIMUM;
    }
  else
    p_state = GENERATOR_STATE_START_SPECIFIED;
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
  printf("@@@ glow_step() = %d history_offset = %d\n", p_history[p_history_offset],
         p_history_offset);
  printf("@@@ ");
  print_detail();
  printf("@@@ now history_offset = %d\n", p_history_offset + 1);
#endif
  return p_history[p_history_offset++];
}

int Generator::history()
{
  if (p_history_offset == p_history_length)
    return -1;
  return p_history[p_history_offset++];
}

bool Generator::is_there_next_branch() const
{
  if (p_state != GENERATOR_STATE_RUNNING)
    return true;
  if (p_history[p_history_offset] < p_maximum_vertices_of_polygons)
    return true;
  return false;
}

void Generator::next_branch()
{
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
  printf("@@@ next_branch() history_offset = %d\n", p_history_offset);
#endif
  switch (p_state)
    {
    case GENERATOR_STATE_START_SPECIFIED:
      break;
    case GENERATOR_STATE_START_FROM_MINIMUM:
      p_history.set_char_at(p_history_offset, p_minimum_polygons);
      p_history_length = p_history_offset + 1;
      break;
    case GENERATOR_STATE_RUNNING:
    default:
      p_history[p_history_offset]++;
      p_history_length = p_history_offset + 1;
      break;
    }
  p_state = GENERATOR_STATE_RUNNING;
}

bool Generator::next_tree()
{
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
  printf("@@@ next_tree()\n");
#endif
  p_history_offset = p_history_length - 1;
  while (1)
    {
      p_history[p_history_offset]++;
      if (p_history[p_history_offset] <= p_maximum_vertices_of_polygons)
        {
          p_history_offset = 0;
          p_state = GENERATOR_STATE_START_SPECIFIED;
          return true;
        }
      else
        {
          if (p_history_length == 1)
            return false;
          --p_history_length;
          p_history_offset = p_history_length - 1;
        }
    }
}

bool Generator::next_by_rollback()
{
  p_history_offset = p_history_length - 1;
  while (1)
    {
      p_history[p_history_offset]++;
      if (p_history[p_history_offset] <= p_maximum_vertices_of_polygons)
        {
          p_state = GENERATOR_STATE_START_SPECIFIED;
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("@@@ next_by_rollback()\n@@@ now history_offset = %d\n",
                 p_history_offset);
#endif
          return true;
        }
      else
        {
          if (p_history_length == 1)
            return false;
          --p_history_length;
          p_history_offset = p_history_length - 1;
        }
    }
}

void Generator::get_generator_formula(MyString& buffer, bool compress) const
{
  if (p_type == GENERATOR_TYPE_TUBE)
    {
      buffer = "T";
      buffer.append_int(p_n);
      buffer.append_char(',');
      buffer.append_int(p_m);
      buffer.append_char(',');
      buffer.append_int(p_h);
      buffer.append_char('-');
    }
  else if (p_type == GENERATOR_TYPE_SYMMETRIC)
    {
      buffer = "S";
      buffer.append_int(p_scrap_no);
      buffer.append_char('-');
    }
  else
    {
      buffer = "A";
      buffer.append_int(p_scrap_no);
      buffer.append_char('-');
    }
  if (compress)
    {
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
  else
    {
      for (int i = 0; i < p_history_offset; ++i)
        buffer.append_char(p_history[i] + '0');
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
