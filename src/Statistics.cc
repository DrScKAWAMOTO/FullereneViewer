/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include "Statistics.h"
#include "Matrix3.h"
#include "ShutUp.h"

Statistics::Statistics()
  : p_sum(0), p_sum_of_yzs(0.0), p_sum_of_zxs(0.0), p_sum_of_xys(0.0)
{
}

Statistics::~Statistics()
{
}

Statistics& Statistics::operator = (const Statistics& UNUSED(that))
{
  fprintf(stderr, "%s:%d: internal error dont use.\n", __FILE__, __LINE__);
  exit(1);
  return *this;
}

Matrix3 Statistics::variance_covariance_matrix() const
{
  return Matrix3(p_sum_of_samplesamples.x() / (double)p_sum,
                 p_sum_of_xys / (double)p_sum,
                 p_sum_of_zxs / (double)p_sum,
                 p_sum_of_xys / (double)p_sum,
                 p_sum_of_samplesamples.y() / (double)p_sum,
                 p_sum_of_yzs / (double)p_sum,
                 p_sum_of_zxs / (double)p_sum,
                 p_sum_of_yzs / (double)p_sum,
                 p_sum_of_samplesamples.z() / (double)p_sum);
}

void Statistics::sample(Vector3* s)
{
  p_samples.add(s);
  p_sum++;
  p_sum_of_samples += *s;
  p_sum_of_samplesamples += *s * *s;
  p_sum_of_yzs += s->y() * s->z();
  p_sum_of_zxs += s->z() * s->x();
  p_sum_of_xys += s->x() * s->y();
}

void Statistics::average_to_zero()
{
  Vector3 average = get_average();
  p_sum_of_samplesamples -= p_sum_of_samples * average;
  p_sum_of_yzs -= p_sum_of_samples.y() * average.z();
  p_sum_of_zxs -= p_sum_of_samples.z() * average.x();
  p_sum_of_xys -= p_sum_of_samples.x() * average.y();
  p_sum_of_samples = 0.0;
  int len = p_samples.length();
  for (int i = 0; i < len; ++i)
    {
      Vector3* sample = p_samples[i];
      *sample -= average;
    }
}

Vector3 Statistics::get_average() const
{
  return p_sum_of_samples / (double)p_sum;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
