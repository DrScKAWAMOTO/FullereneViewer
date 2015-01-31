/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
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
      Ring* target = ca->get_ring(i);
      if (target->number_of_carbons() != 5)
        continue;
      p_calculate_distances_to_a_ring(ca, target);
    }
}

void Characteristic::p_calculate_distances_to_a_ring(CarbonAllotrope* ca, Ring* target)
{
  int len = ca->number_of_rings();
  int target_seq_no = target->sequence_no();
  int distances[len];
  for (int i = 0; i < len; ++i)
    distances[i] = INT_MAX;
  Queue<Ring> operations;
  DistanceVector* dv = new DistanceVector();
  distances[target_seq_no - 1] = 0;
  dv->append(0);
  operations.enqueue(target);
  while (1)
    {
      Ring* next = operations.dequeue();
      if (next == 0)
        {
          DistanceVector* result = p_dvs.search_else_add(dv);
          assert(result == 0);
          return;
        }
      int next_seq_no = next->sequence_no();
      int next_distance = distances[next_seq_no - 1];
      int num_c = next->number_of_carbons();
      for (int j = 0; j < num_c; ++j)
        {
          Carbon* carbon = next->get_carbon(j);
          int num_r = carbon->number_of_rings();
          for (int k = 0; k < num_r; ++k)
            {
              Ring* ring = carbon->get_ring(k);
              int ring_seq_no = ring->sequence_no();
              if (distances[ring_seq_no - 1] == INT_MAX)
                {
                  int distance = next_distance + 1;
                  distances[ring_seq_no - 1] = distance;
                  if (ring->number_of_carbons() == 5)
                    dv->append(distance);
                  operations.enqueue(ring);
                }
            }
        }
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
