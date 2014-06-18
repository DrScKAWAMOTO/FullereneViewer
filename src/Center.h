/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/20 22:13:58 JST
 */

#ifndef __CENTER_H__
#define __CENTER_H__

#include "Vector3.h"

class Center {
  // members
public:
  Vector3 location;
  bool fixed;
  Vector3 fixed_location;
  Vector3 next_force;

  // constructors & the destructor
public:
  Center() : location(), fixed(false), fixed_location(), next_force() { }
  ~Center() { }
  Center& operator = (const Center& that); /* dont use */

};

#endif /* __CENTER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
