/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */ 

#include <float.h>
#include <assert.h>
#include "InteractivePlane.h"
#include "ShutUp.h"

Vector3 InteractivePlane::p_get_vertex_location(int UNUSED(index)) const
{
  const char there_are_no_vertices = 0;
  assert(there_are_no_vertices);
  return Vector3();
}

InteractivePlane::InteractivePlane(Interactives* interactives, int sequence_no)
  : Interactive(interactives, sequence_no, FLT_EPSILON)
{
}

InteractivePlane::~InteractivePlane()
{
}

int InteractivePlane::number_of_vertices() const
{
  return 0;
}

Vector3 InteractivePlane::get_nearest_point(const Vector3& by_your_location) const
{
  // normal = (a,b,c)
  // point = (d,e,f)
  // 平面の方程式は ax + by + cz = 0
  // point を通って normal 方向の直線の方程式 x=(d+ak), y=(e+bk), z=(f+ck)
  // 交点は、a(d+ak) + b(e+bk) + c(f+ck) = 0
  // ad + be + cf + (a*a + b*b + c*c)k = 0
  // k = -(normal * point) / (normal * normal)
  const Vector3 my_normal = get_normal();
  const double k = (inner_product(my_normal, by_your_location) /
                    inner_product(my_normal, my_normal));
  return by_your_location - my_normal * k;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/










