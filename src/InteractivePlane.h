/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __INTERACTIVEPLANE_H__
#define __INTERACTIVEPLANE_H__

#include "Interactive.h"

class InteractivePlane : public Interactive {
  // friend classes & functions

  // members

  // private tools
  // attributes
protected:
  virtual Vector3 p_get_vertex_location(int index) const;

  // constructors & the destructor
public:
  InteractivePlane(Interactives* interactives, int sequence_no);
  virtual ~InteractivePlane();
  InteractivePlane& operator = (const InteractivePlane& you); /* dont use */

  // interactions

  // representation

  // I/O

  // class decision

  // member accessing methods
  // attributes
public:
  virtual int number_of_vertices() const;
  virtual Vector3 get_nearest_point(const Vector3& by_your_location) const;

};

#endif /* __INTERACTIVEPLANE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
