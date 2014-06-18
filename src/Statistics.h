/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 08:46:27 JST
 */

#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "List.h"
#include "Vector3.h"

class Statistics {
  // friend classes & functions

  // members
private:
  List<Vector3> p_samples;
  int p_sum;
  Vector3 p_sum_of_samples;
  Vector3 p_sum_of_samplesamples;
  double p_sum_of_yzs;
  double p_sum_of_zxs;
  double p_sum_of_xys;

  // private tools

  // constructors & the destructor
public:
  Statistics();
  ~Statistics();
  Statistics& operator = (const Statistics& that); /* dont use */

  // type converters
  Matrix3 variance_covariance_matrix() const;

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
  void sample(Vector3* s);
  void average_to_zero();
  Vector3 get_average() const;

};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
