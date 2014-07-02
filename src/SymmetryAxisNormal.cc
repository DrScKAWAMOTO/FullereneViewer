/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/26 18:06:18 JST
 */

#include <assert.h>
#include "SymmetryAxisNormal.h"
#include "CarbonAllotrope.h"
#include "OpenGLUtil.h"

SymmetryAxisNormal::SymmetryAxisNormal(CarbonAllotrope* ca, SymmetryAxis* axis)
  : InteractiveRegularPolygon(ca, 0, 1.0, 2), p_ca(ca), p_axis(axis)
{
}

SymmetryAxisNormal::~SymmetryAxisNormal()
{
}

void SymmetryAxisNormal::reset_interaction()
{
  InteractiveRegularPolygon::reset_interaction();
}

void SymmetryAxisNormal::interaction_original(OriginalForceType force_type,
                                              Interactives* interactives, double delta)
{
  Vector3 north_location;
  Vector3 south_location;
  int north = p_axis->get_north_sequence_no();
  int south = p_axis->get_south_sequence_no();
  switch (p_axis->get_type())
    {
    case AXIS_TYPE_CENTER_OF_RING:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        Ring* south_ring = p_ca->get_ring_by_sequence_no(south);
        north_location = north_ring->get_center_location();
        south_location = south_ring->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_BOND:
      {
        Bond* north_bond = p_ca->get_bond_by_sequence_no(north);
        Bond* south_bond = p_ca->get_bond_by_sequence_no(south);
        north_location = north_bond->get_center_location();
        south_location = south_bond->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_CARBON:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        Carbon* south_carbon = p_ca->get_carbon_by_sequence_no(south);
        north_location = north_carbon->get_center_location();
        south_location = south_carbon->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_BOND:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        Bond* south_bond = p_ca->get_bond_by_sequence_no(south);
        north_location = north_ring->get_center_location();
        south_location = south_bond->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_CARBON:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        Carbon* south_carbon = p_ca->get_carbon_by_sequence_no(south);
        north_location = north_ring->get_center_location();
        south_location = south_carbon->get_center_location();
      }
      break;
    default:
      assert(0);
    }
  Vector3 center = (north_location + south_location) * 0.5;
  Vector3 normal = north_location - south_location;
  p_normal.clockwise = 1;
  fix_center_location(center);
  fix_radius_length(normal.abs() * 0.6);
  fix_posture(Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), normal)));
}

void SymmetryAxisNormal::draw_opaque_by_OpenGL(bool selection) const
{
  Vector3 norm = get_normal();
  norm *= p_radius.length;
  OpenGLUtil::set_color(0xc0c0c0);
  OpenGLUtil::draw_cylinder(0.1, get_center_location() - norm,
                            get_center_location() + norm);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
