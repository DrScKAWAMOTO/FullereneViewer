/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 07:07:09 JST
 */ 

#include <assert.h>
#include <math.h>
#include "InteractiveRegularPolygon.h"
#include "Utils.h"
#include "OpenGLUtil.h"

Vector3 InteractiveRegularPolygon::p_get_vertex_location(int index) const
{
  assert((index >= 0) && (index < p_n));
  return Vector3(sin(index * PAI * 2.0 / p_n), cos(index * PAI * 2.0 / p_n), 0.0);
}

InteractiveRegularPolygon::InteractiveRegularPolygon(Interactives* interactives,
                                                     int sequence_no,
                                                     double radius_length, int n)
  : Interactive(interactives, sequence_no, radius_length), p_n(n)
{
  assert(p_n > 0);
}

InteractiveRegularPolygon::~InteractiveRegularPolygon()
{
}

int InteractiveRegularPolygon::number_of_vertices() const
{
  return p_n;
}

Vector3 InteractiveRegularPolygon::
get_nearest_point(const Vector3& by_your_location) const
{
  return Vector3();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/










