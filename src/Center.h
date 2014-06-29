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
  Vector3 last_location;
  Vector3 next_force;

  // constructors & the destructor
public:
  Center() : location(), fixed(false), fixed_location(), last_location(), next_force() { }
  ~Center() { }
  Center& operator = (const Center& that); /* dont use */

  // stability
  bool stabled() const { return last_location == location; }

};

#endif /* __CENTER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
