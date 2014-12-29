/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "DistanceVector.h"

DistanceVector::DistanceVector()
{
  for (int i = 0; i < 12; ++i)
    p_distances[i] = INT_MAX;
}

DistanceVector::DistanceVector(const DistanceVector& you)
  : Object(you.sequence_no())
{
  for (int i = 0; i < 12; ++i)
    p_distances[i] = you.p_distances[i];
}

DistanceVector::~DistanceVector()
{
}

DistanceVector& DistanceVector::operator = (const DistanceVector& you)
{
  if (this != &you)
    {
      for (int i = 0; i < 12; ++i)
        p_distances[i] = you.p_distances[i];
    }
  return *this;
}

int DistanceVector::compare(const DistanceVector* you) const
{
  for (int i = 0; i < 12; ++i)
    {
      int diff = p_distances[i] - you->p_distances[i];
      if (diff != 0)
        return diff;
    }
  return you - this;
}

void DistanceVector::print() const
{
  int val = -1;
  int num = 0;
  bool first_time = true;
  for (int i = 0; i < 12; ++i)
    {
      if (p_distances[i] > val)
        {
          if (val != -1)
            {
              if (!first_time)
                printf(" ");
              first_time = false;
              printf("%d@%d", num, val);
            }
          val = p_distances[i];
          num = 1;
        }
      else
        num++;
    }
  printf(" %d@%d", num, val);
}

void DistanceVector::append(int distance)
{
  for (int i = 0; i < 12; ++i)
    {
      if (p_distances[i] > distance)
        {
          for (int j = 10; j >= i; --j)
            p_distances[j+1] = p_distances[j];
          p_distances[i] = distance;
          return;
        }
    }
  assert(0);
}

int DistanceVector::operator [] (int index) const
{
  assert((index >= 0) && (index < 12));
  return p_distances[index];
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
