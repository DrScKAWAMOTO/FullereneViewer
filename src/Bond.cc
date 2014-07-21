/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/31 18:05:12 JST
 */

#include <assert.h>
#include <float.h>
#include "Bond.h"
#include "CarbonAllotrope.h"
#include "OpenGLUtil.h"

Bond::Bond(CarbonAllotrope* ca)
  : InteractiveRegularPolygon(ca, ca->bond_next_sequence++, 1.0, 2),
    p_bond_radius(ca->bond_radius), p_bond_color(ca->bond_color), p_left(0), p_right(0)
{
  ca->register_bond(this);
}

Bond::~Bond()
{
  if (p_left)
    {
      p_left->remove_bond(this);
      p_left = 0;
    }
  if (p_right)
    {
      p_right->remove_bond(this);
      p_right = 0;
    }
}

bool Bond::connect_to(Carbon* carbon)
{
  if (!p_left)
    {
      p_left = carbon;
      return true;
    }
  else if (!p_right)
    {
      p_right = carbon;
      return true;
    }
  return false;
}

void Bond::remove_carbon(Carbon* carbon)
{
  if (p_left == carbon)
    {
      p_left = 0;
      p_right->remove_bond(this);
      p_right = 0;
    }
  if (p_right == carbon)
    {
      p_right = 0;
      p_left->remove_bond(this);
      p_left = 0;
    }
}

void Bond::print_structure(int indent, bool deep) const
{
  for (int i = 0; i < indent; ++i)
    printf("  ");
  printf("Bond(%d)", sequence_no());
  if (deep)
    {
      printf("[");
      if (p_left)
        p_left->print_structure(0, false);
      if (p_right)
        p_right->print_structure(0, false);
      printf("]\n");
    }
}

void Bond::print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const
{
  Vector3 loc0 = get_left_carbon()->carbon_location();
  Vector3 loc1 = get_right_carbon()->carbon_location();
  fprintf(fptr, "cylinder {<%g,%g,%g>,<%g,%g,%g>,%g texture {pigment "
          "{color red %g green %g blue %g}}}\n",
          loc0.x(), loc0.y(), loc0.z(), loc1.x(), loc1.y(), loc1.z(), p_bond_radius,
          ((p_bond_color>>16)&255)/255.0, ((p_bond_color>>8)&255)/255.0,
          ((p_bond_color>>0)&255)/255.0);
  if (ca->print_out_sequence_no)
    {
      fprintf(fptr, "text {ttf \"crystal.ttf\", \"%d\", 0.1, x*(-0.01) pigment "
              "{color red 0.0 green 0.0 blue 0.5} "
              "scale 0.4 rotate y*180 translate <%g,%g,%g>}\n",
              sequence_no(), (loc0.x() + loc1.x())*0.5, (loc0.y() + loc1.y())*0.5,
              (loc0.z() + loc1.z())*0.5 + 1.2 * p_bond_radius);
    }
}

void Bond::print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr,
                                          const Matrix3& rot, const Vector3& move,
                                          bool clipped_by_Z_non_negative) const
{
  Vector3 loc0 = get_left_carbon()->carbon_location() * rot + move;
  Vector3 loc1 = get_right_carbon()->carbon_location() * rot + move;
  if (((loc0.z() > p_bond_radius) && (loc1.z() > p_bond_radius)) ||
      (!clipped_by_Z_non_negative))
    {
      fprintf(fptr, "cylinder {<%g,%g,%g>,<%g,%g,%g>,%g ",
              loc0.x(), loc0.y(), loc0.z(), loc1.x(), loc1.y(), loc1.z(), p_bond_radius);
      fprintf(fptr, "texture {pigment {color red %g green %g blue %g}}}\n",
              ((p_bond_color>>16)&255)/255.0, ((p_bond_color>>8)&255)/255.0,
              ((p_bond_color>>0)&255)/255.0);
    }
  else if ((loc0.z() >= -p_bond_radius) || (loc1.z() >= -p_bond_radius))
    {
      fprintf(fptr, "intersection {\n");
      fprintf(fptr, "  cylinder {<%g,%g,%g>,<%g,%g,%g>,%g}\n",
              loc0.x(), loc0.y(), loc0.z(), loc1.x(), loc1.y(), loc1.z(), p_bond_radius);
      fprintf(fptr, "  plane {<0,0,-1>,0}\n");
      fprintf(fptr, "  texture {pigment {color red %g green %g blue %g}}\n",
              ((p_bond_color>>16)&255)/255.0, ((p_bond_color>>8)&255)/255.0,
              ((p_bond_color>>0)&255)/255.0);
      fprintf(fptr, "}\n");
    }
}

void Bond::draw_opaque_by_OpenGL(bool selection) const
{
  Vector3 loc0 = get_left_carbon()->carbon_location();
  Vector3 loc1 = get_right_carbon()->carbon_location();
  OpenGLUtil::set_color(p_bond_color);
  OpenGLUtil::draw_cylinder(p_bond_radius, loc0, loc1);
}

Vector3 Bond::get_center_location() const
{
  Vector3 center = p_left->get_center_location() + p_right->get_center_location();
  return center * 0.5;
}

bool Bond::connected_with(const Carbon* carbon)
{
  if (p_left == carbon)
    return true;
  else if (p_right == carbon)
    return true;
  return false;
}

Carbon* Bond::get_carbon_beyond(const Carbon* carbon)
{
  if (p_left == carbon)
    return p_right;
  else if (p_right == carbon)
    return p_left;
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
