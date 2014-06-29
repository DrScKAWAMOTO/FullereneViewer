/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 07:07:09 JST
 */ 

#include <float.h>
#include <assert.h>
#include "InteractivePoint.h"

Vector3 InteractivePoint::p_get_vertex_location(int index) const
{
  const char there_are_no_vertices = 0;
  assert(there_are_no_vertices);
  return Vector3();
}

InteractivePoint::InteractivePoint(Interactives* interactives, int sequence_no)
  : Interactive(interactives, sequence_no, FLT_EPSILON)
{
  assert(p_n > 0);
}

InteractivePoint::~InteractivePoint()
{
}

int InteractivePoint::number_of_vertices() const
{
  return 0;
}

Vector3 InteractivePoint::get_nearest_point(const Vector3& by_your_location) const
{
  return get_center_location();
}

Vector3 InteractivePoint::get_normal(const Vector3& by_your_location) const
{
  Vector3 work = by_your_location - get_center_location();
  return work / work.abs();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/










