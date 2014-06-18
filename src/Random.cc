/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/04 10:51:50 JST
 */

#include <stdlib.h>
#include <time.h>
#include "Random.h"

void Random::initialize()
{
  srand((unsigned int)time(NULL));
}

double Random::minus1to1()
{
  return minus_width_to_width(1.0);
}

double Random::minus_width_to_width(double width)
{
  double result = (double)rand() / (double)RAND_MAX;
  return (result * 2.0 - 1.0) * width;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
