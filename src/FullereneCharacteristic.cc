/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 07:56:19 JST
 */

#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include "FullereneCharacteristic.h"
#include "CarbonAllotrope.h"
#include "DebugMemory.h"

FullereneCharacteristic::FullereneCharacteristic(CarbonAllotrope* ca)
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
      ca->reset_distances_to_set();
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
      p_dvs.add(dv);
    }
}

FullereneCharacteristic::~FullereneCharacteristic()
{
}

void FullereneCharacteristic::print_detail() const
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

void FullereneCharacteristic::print_summary(FILE* fptr) const
{
  int len = p_dvs.length();
  int max_dist = 0;
  for (int i = 0; i < len; ++i)
    {
      DistanceVector* dv = p_dvs[i];
      int dist = (*dv)[11];
      if (dist > max_dist)
        max_dist = dist;
    }
  int* array = new int[max_dist + 1];
  for (int i = 0; i <= max_dist; ++i)
    array[i] = 0;
  for (int i = 0; i < len; ++i)
    {
      DistanceVector* dv = p_dvs[i];
      for (int j = 0; j < 12; ++j)
        {
          int dist = (*dv)[j];
          assert((dist >= 0) && (dist <= max_dist));
          ++array[dist];
        }
    }
  for (int i = 0; i < max_dist; ++i)
    fprintf(fptr, "%d,", array[i]);
  fprintf(fptr, "%d\n", array[max_dist]);
}

DistanceVector* FullereneCharacteristic::get_distance_vector(int index)
{
  assert((index >= 0) && (index < number_of_distance_vector()));
  return p_dvs[index];
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
