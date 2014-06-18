/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 07:07:09 JST
 */

#ifndef __INTERACTIVEPOINT_H__
#define __INTERACTIVEPOINT_H__

#include "Interactive.h"

class InteractivePoint : public Interactive {
  // friend classes & functions

  // members
protected:
  int p_n;

  // private tools
  // attributes
protected:
  virtual Vector3 p_get_vertex_location(int index) const;

  // constructors & the destructor
public:
  InteractivePoint(Interactives* interactives, int sequence_no);
  virtual ~InteractivePoint();
  InteractivePoint& operator = (const InteractivePoint& that); /* dont use */

  // interactions

  // representation

  // I/O

  // class decision

  // member accessing methods
  // attributes
public:
  virtual int number_of_vertices() const;
  virtual Vector3 get_nearest_point(const Vector3& by_your_location) const;
  virtual Vector3 get_normal(const Vector3& by_your_location) const;

};

#endif /* __INTERACTIVEPOINT_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
