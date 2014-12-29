/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include "ThreeViewNormal.h"
#include "CarbonAllotrope.h"
#include "OpenGLUtil.h"
#include "ShutUp.h"

int ThreeViewNormal::p_get_nth() const
{
  return sequence_no();
}

ThreeViewNormal::ThreeViewNormal(CarbonAllotrope* ca, int nth)
  : InteractiveRegularPolygon(ca, nth, 1.0, 2), p_ca(ca)
{
}

ThreeViewNormal::~ThreeViewNormal()
{
}

void ThreeViewNormal::reset_interaction()
{
  InteractiveRegularPolygon::reset_interaction();
  p_ca->reset_three_axes();
}

void ThreeViewNormal::interaction_original(OriginalForceType UNUSED(force_type),
                                           Interactives* UNUSED(interactives), double UNUSED(delta))
{
  p_ca->calculate_three_axes();
  double Eigenvalue;
  Vector3 Eigenvector;
  switch (p_get_nth())
    {
    case 1:
      p_ca->get_primary_Eigenvalue_and_Eigenvector(Eigenvalue, Eigenvector);
      break;
    case 2:
      p_ca->get_secondary_Eigenvalue_and_Eigenvector(Eigenvalue, Eigenvector);
      break;
    default:
      p_ca->get_tertiary_Eigenvalue_and_Eigenvector(Eigenvalue, Eigenvector);
      break;
    }
  p_normal.clockwise = 1;
  fix_center_location(p_ca->get_center_location());
  fix_radius_length(sqrt(Eigenvalue) * 2.0);
  fix_posture(Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Eigenvector)));
}

void ThreeViewNormal::draw_opaque_by_OpenGL(bool UNUSED(selection)) const
{
  Vector3 norm = get_normal();
  norm *= p_radius.length;
  switch (p_get_nth())
    {
    case 1:
      OpenGLUtil::set_color(0xff0000);
      break;
    case 2:
      OpenGLUtil::set_color(0x00ff00);
      break;
    default:
      OpenGLUtil::set_color(0x0000ff);
      break;
    }
  OpenGLUtil::draw_cylinder(0.1, get_center_location() - norm,
                            get_center_location() + norm);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
