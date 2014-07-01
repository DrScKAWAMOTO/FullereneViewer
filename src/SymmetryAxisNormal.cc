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
  assert(p_axis->get_type() == AXIS_TYPE_CENTER_OF_RING);
  int north = p_axis->get_north_sequence_no();
  int south = p_axis->get_south_sequence_no();
  Ring* north_ring = p_ca->get_ring_by_sequence_no(north);
  Ring* south_ring = p_ca->get_ring_by_sequence_no(south);
  Vector3 north_location = north_ring->get_center_location();
  Vector3 south_location = south_ring->get_center_location();
  Vector3 normal = north_location - south_location;
  p_normal.clockwise = 1;
  fix_center_location(p_ca->get_center_location());
  fix_radius_length(normal.abs() * 0.55);
  fix_posture(Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), normal)));
}

void SymmetryAxisNormal::draw_opaque_by_OpenGL(bool selection) const
{
  Vector3 norm = get_normal();
  norm *= p_radius.length;
  OpenGLUtil::set_color(0x00ff00);
  OpenGLUtil::draw_cylinder(0.4, get_center_location() - norm,
                            get_center_location() + norm);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
