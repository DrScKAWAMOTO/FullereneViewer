/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 07:07:09 JST
 */ 

#include <float.h>
#include <assert.h>
#include "InteractiveLine.h"
#include "OpenGLUtil.h"

Vector3 InteractiveLine::p_get_vertex_location(int index) const
{
  const char there_are_no_vertices = 0;
  assert(there_are_no_vertices);
  return Vector3();
}

InteractiveLine::InteractiveLine(Interactives* interactives, int sequence_no)
  : Interactive(interactives, sequence_no, FLT_EPSILON)
{
}

InteractiveLine::~InteractiveLine()
{
}

int InteractiveLine::number_of_vertices() const
{
  return 0;
}

Vector3 InteractiveLine::get_nearest_point(const Vector3& by_your_location) const
{
  // normal = (a,b,c)
  // point = (d,e,f)
  // point を通って normal に垂直な面の方程式 a(x-d) + b(y-e) + c(z-f) = 0
  // normal の定数倍 k (ak,bk,ck) のポイントがこの方程式を満たす。
  // aak + bbk + cck = ad + be + cf
  // (aa + bb + cc) k = ad + be + cf
  // k = (normal * point) / (normal * normal)
  const Vector3 my_normal = get_normal();
  const double k = (inner_product(my_normal, by_your_location) /
                    inner_product(my_normal, my_normal));
  return my_normal * k;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/










