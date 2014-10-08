/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __RADIUS_H__
#define __RADIUS_H__

#include <math.h>
#include <float.h>

class Radius {
  // members
public:
  double length;
  bool fixed;
  double fixed_length;
  double last_length;
  double next_force;

  // constructors & the destructor
public:
  Radius(double len) : length(len), fixed(false), fixed_length(0.0), last_length(0.0),
                       next_force(0.0) { }
  ~Radius() { }
  Radius& operator = (const Radius& you); /* dont use */

  // stability
  bool stabled() const { return fabs(last_length - length) <= DBL_EPSILON; }

};

#endif /* __RADIUS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/


