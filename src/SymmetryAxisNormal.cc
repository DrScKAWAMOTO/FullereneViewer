/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include "SymmetryAxisNormal.h"
#include "CarbonAllotrope.h"
#include "OpenGLUtil.h"
#include "ShutUp.h"

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

void SymmetryAxisNormal::interaction_original(OriginalForceType UNUSED(force_type),
                                              Interactives* UNUSED(interactives), double UNUSED(delta))
{
  Vector3 north_location;
  Vector3 south_location;
  int north = p_axis->get_north_sequence_no();
  int south = p_axis->get_south_sequence_no();
  switch (p_axis->get_type())
    {
    case AXIS_TYPE_CENTER_OF_TWO_CARBONS:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        Carbon* south_carbon = p_ca->get_carbon_by_sequence_no(south);
        north_location = north_carbon->get_center_location();
        south_location = south_carbon->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_BOND:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        Bond* south_bond = p_ca->get_bond_by_sequence_no(south);
        north_location = north_carbon->get_center_location();
        south_location = south_bond->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_RING:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        Ring* south_ring = p_ca->get_ring_by_sequence_no(south);
        north_location = north_carbon->get_center_location();
        south_location = south_ring->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_BOUNDARY:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        ConnectedBoundary* south_boundary = p_ca->get_boundary_by_sequence_no(south);
        north_location = north_carbon->get_center_location();
        south_location = south_boundary->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_TWO_BONDS:
      {
        Bond* north_bond = p_ca->get_bond_by_sequence_no(north);
        Bond* south_bond = p_ca->get_bond_by_sequence_no(south);
        north_location = north_bond->get_center_location();
        south_location = south_bond->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_BOND_AND_RING:
      {
        Bond* north_bond = p_ca->get_bond_by_sequence_no(north);
        Ring* south_ring = p_ca->get_ring_by_sequence_no(south);
        north_location = north_bond->get_center_location();
        south_location = south_ring->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_BOND_AND_BOUNDARY:
      {
        Bond* north_bond = p_ca->get_bond_by_sequence_no(north);
        ConnectedBoundary* south_boundary = p_ca->get_boundary_by_sequence_no(south);
        north_location = north_bond->get_center_location();
        south_location = south_boundary->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_TWO_RINGS:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        Ring* south_ring = p_ca->get_ring_by_sequence_no(south);
        north_location = north_ring->get_center_location();
        south_location = south_ring->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_BOUNDARY:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        ConnectedBoundary* south_boundary = p_ca->get_boundary_by_sequence_no(south);
        north_location = north_ring->get_center_location();
        south_location = south_boundary->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_TWO_BOUNDARIES:
      {
        ConnectedBoundary* north_boundary = p_ca->get_boundary_by_sequence_no(north);
        ConnectedBoundary* south_boundary = p_ca->get_boundary_by_sequence_no(south);
        north_location = north_boundary->get_center_location();
        south_location = south_boundary->get_center_location();
      }
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_CARBON:
      {
        Carbon* north_carbon = p_ca->get_carbon_by_sequence_no(north);
        Vector3 center_location = p_ca->get_center_location();
        north_location = north_carbon->get_center_location();
        south_location = Vector3() - north_location;
      }
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_BOND:
      {
        Bond* north_bond = p_ca->get_bond_by_sequence_no(north);
        Vector3 center_location = p_ca->get_center_location();
        north_location = north_bond->get_center_location();
        south_location = Vector3() - north_location;
      }
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_RING:
      {
        Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
        Vector3 center_location = p_ca->get_center_location();
        north_location = north_ring->get_center_location();
        south_location = Vector3() - north_location;
      }
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_BOUNDARY:
      {
        ConnectedBoundary* north_boundary = p_ca->get_boundary_by_sequence_no(north);
        Vector3 center_location = p_ca->get_center_location();
        north_location = north_boundary->get_center_location();
        south_location = Vector3() - north_location;
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

void SymmetryAxisNormal::draw_opaque_by_OpenGL(bool UNUSED(selection)) const
{
  Vector3 norm = get_normal();
  norm *= p_radius.length;
  OpenGLUtil::set_color(0x001810);
  OpenGLUtil::draw_cylinder(0.1, get_center_location() - norm,
                            get_center_location() + norm);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
