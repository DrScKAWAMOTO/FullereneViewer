/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "ConnectedBoundary.h"
#include "CarbonAllotrope.h"

ConnectedBoundary::ConnectedBoundary(CarbonAllotrope* ca,
                                     const List<Carbon>& carbon_list)
  : Object(ca->boundary_next_sequence++), p_carbons(carbon_list)
{
  ca->register_boundary(this);
}

ConnectedBoundary::~ConnectedBoundary()
{
}

bool ConnectedBoundary::contained(Carbon* carbon) const
{
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      if (p_carbons[i] == carbon)
        return true;
    }
  return false;
}

Vector3 ConnectedBoundary::get_center_location() const
{
  int len = p_carbons.length();
  Vector3 value;
  for (int i = 0; i < len; ++i)
    value += p_carbons[i]->get_center_location();
  return value / len;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
