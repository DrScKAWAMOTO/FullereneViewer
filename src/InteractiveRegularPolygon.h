/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 07:07:09 JST
 */

#ifndef __INTERACTIVEREGULARPOLYGON_H__
#define __INTERACTIVEREGULARPOLYGON_H__

#include "Interactive.h"

class InteractiveRegularPolygon : public Interactive {
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
  InteractiveRegularPolygon(Interactives* interactives, int sequence_no,
                            double radius_length, int n);
  virtual ~InteractiveRegularPolygon();
  InteractiveRegularPolygon&
  operator = (const InteractiveRegularPolygon& that); /* dont use */

  // interactions

  // representation

  // I/O
public:
  virtual void draw_opaque_by_OpenGL() const;

  // class decision

  // member accessing methods
  // attributes
public:
  virtual int number_of_vertices() const;
  virtual Vector3 get_nearest_point(const Vector3& by_your_location) const;

};

#endif /* __INTERACTIVEREGULARPOLYGON_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
