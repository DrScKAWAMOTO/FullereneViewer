/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include "SortByZ.h"

SortByZ::SortByZ(Interactive* target, const Matrix3& mosv)
  : p_z(0.0), p_target(target)
{
  assert(p_target);
  Vector3 loc = p_target->get_center_location() * mosv;
  p_z = loc.z();
}

SortByZ::~SortByZ()
{
}

int SortByZ::compare(const SortByZ* you) const
{
  if (this == you)
    return 0;
  if (p_z > you->p_z)
    return 1;
  if (p_z < you->p_z)
    return -1;
  if (this > you)
    return 1;
  return -1;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
