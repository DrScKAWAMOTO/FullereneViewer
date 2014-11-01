/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "Characteristic.h"
#include "CarbonAllotrope.h"
#include "DebugMemory.h"
#include "MyString.h"

Characteristic::Characteristic(CarbonAllotrope* ca)
{
  int len = ca->number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = ca->get_ring(i);
      if (ring->number_of_carbons() != 5)
        continue;
      DistanceVector* dv = new DistanceVector();
      dv->append(0);
      List<Carbon> set;
      for (int j = 0; j < 5; ++j)
        set.add(ring->get_carbon(j));
      ca->calculate_distances_to_set(set);
      for (int j = 0; j < len; ++j)
        {
          Ring* ring2 = ca->get_ring(j);
          if (i == j)
            continue;
          if (ring2->number_of_carbons() != 5)
            continue;
          int min_dist = INT_MAX;
          for (int k = 0; k < 5; ++k)
            {
              Carbon* carbon = ring2->get_carbon(k);
              int dist = carbon->distance_to_set();
              if (dist == 0)
                continue;
              if (dist < min_dist)
                min_dist = dist;
            }
          dv->append(min_dist);
        }
      DistanceVector* result = p_dvs.search_else_add(dv);
      assert(result == 0);
    }
}

Characteristic::~Characteristic()
{
}

void Characteristic::print_detail() const
{
  int len = p_dvs.length();
  DistanceVector* last_dv = 0;
  int count = 0;
  for (int i = 0; i < len; ++i)
    {
      DistanceVector* dv = p_dvs[i];
      if (last_dv)
        {
          if ((*last_dv) == (*dv))
            count++;
          else
            {
              printf("%d@(", count);
              last_dv->print();
              printf(")\n");
              last_dv = dv;
              count = 1;
            }
        }
      else
        {
          last_dv = dv;
          count = 1;
        }
    }
  printf("%d@(", count);
  last_dv->print();
  printf(")\n");
}

static MyString summary_buffer;

const char* Characteristic::get_summary() const
{
  int len = p_dvs.length();
  int max_dist = 0;
  for (int i = 0; i < len; ++i)
    {
      DistanceVector* dv = p_dvs[i];
      int dist = (*dv)[len - 1];
      if (dist > max_dist)
        max_dist = dist;
    }
  int* array = new int[max_dist + 1];
  for (int i = 0; i <= max_dist; ++i)
    array[i] = 0;
  for (int i = 0; i < len; ++i)
    {
      DistanceVector* dv = p_dvs[i];
      for (int j = 0; j < len; ++j)
        {
          int dist = (*dv)[j];
          assert((dist >= 0) && (dist <= max_dist));
          ++array[dist];
        }
    }
  summary_buffer = "";
  for (int i = 0; i < max_dist; ++i)
    {
      summary_buffer.append_int(array[i]);
      summary_buffer.append_char(';');
    }
  summary_buffer.append_int(array[max_dist]);
  delete[] array;
  return (char*)summary_buffer;
}

void Characteristic::print_summary(FILE* fptr) const
{
  const char* ptr;
  ptr = get_summary();
  fprintf(fptr, "%s\n", ptr);
}

DistanceVector* Characteristic::get_distance_vector(int index)
{
  assert((index >= 0) && (index < number_of_distance_vector()));
  return p_dvs[index];
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
