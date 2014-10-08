/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <limits.h>
#include <assert.h>
#include <float.h>
#include "Config.h"
#include "Carbon.h"
#include "Queue.h"
#include "CarbonAllotrope.h"
#include "Representation.h"
#include "Automorphism.h"
#include "OpenGLUtil.h"
#include "Debug.h"
#include "DebugMemory.h"
#include "ShutUp.h"

Carbon::Carbon(CarbonAllotrope* ca)
  : InteractiveRegularPolygon(ca, ca->carbon_next_sequence++, 1.0, 3),
    p_radius(ca->carbon_radius), p_color(ca->carbon_color),
    p_bond_0(0), p_ring_01(0), p_bond_1(0), p_ring_12(0), p_bond_2(0), p_ring_20(0),
    p_distance_to_set(INT_MAX)
{
  ca->register_carbon(this);
}

Carbon::~Carbon()
{
  if (p_bond_0)
    {
      p_bond_0->remove_carbon(this);
      p_bond_0->link_down();
      p_bond_0 = 0;
    }
  if (p_bond_1)
    {
      p_bond_1->remove_carbon(this);
      p_bond_1->link_down();
      p_bond_1 = 0;
    }
  if (p_bond_2)
    {
      p_bond_2->remove_carbon(this);
      p_bond_2->link_down();
      p_bond_2 = 0;
    }
  if (p_ring_01)
    p_ring_01 = 0;
  if (p_ring_12)
    p_ring_12 = 0;
  if (p_ring_20)
    p_ring_20 = 0;
}

void Carbon::copy_from(const CarbonAllotrope* ca, const Carbon* you)
{
  assert(sequence_no() == you->sequence_no());
  p_radius = you->p_radius;
  p_color = you->p_color;
  if (you->p_bond_0)
    {
      p_bond_0 = ca->get_bond_by_sequence_no(you->p_bond_0->sequence_no());
      p_bond_0->link_up();
    }
  else
    p_bond_0 = 0;
  if (you->p_ring_01)
    p_ring_01 = ca->get_ring_by_sequence_no(you->p_ring_01->sequence_no());
  else
    p_ring_01 = 0;
  if (you->p_bond_1)
    {
      p_bond_1 = ca->get_bond_by_sequence_no(you->p_bond_1->sequence_no());
      p_bond_1->link_up();
    }
  else
    p_bond_1 = 0;
  if (you->p_ring_12)
    p_ring_12 = ca->get_ring_by_sequence_no(you->p_ring_12->sequence_no());
  else
    p_ring_12 = 0;
  if (you->p_bond_2)
    {
      p_bond_2 = ca->get_bond_by_sequence_no(you->p_bond_2->sequence_no());
      p_bond_2->link_up();
    }
  else
    p_bond_2 = 0;
  if (you->p_ring_20)
    p_ring_20 = ca->get_ring_by_sequence_no(you->p_ring_20->sequence_no());
  else
    p_ring_20 = 0;
  p_distance_to_set = you->p_distance_to_set;
}

bool Carbon::connect_to(CarbonAllotrope* ca, Carbon* carbon)
{
  if (bond_between(carbon))
    return true;
  if (!p_bond_0)
    {
      p_bond_0 = new Bond(ca);
      p_bond_0->link_up();
      if (!p_bond_0->connect_to(this))
        return false;
      return carbon->connect_to(ca, p_bond_0);
    }
  else if (!p_bond_1)
    {
      p_bond_1 = new Bond(ca);
      p_bond_1->link_up();
      if (!p_bond_1->connect_to(this))
        return false;
      return carbon->connect_to(ca, p_bond_1);
    }
  else if (!p_bond_2)
    {
      p_bond_2 = new Bond(ca);
      p_bond_2->link_up();
      if (!p_bond_2->connect_to(this))
        return false;
      return carbon->connect_to(ca, p_bond_2);
    }
  return false;
}

bool Carbon::connect_to(CarbonAllotrope* UNUSED(ca), Bond* bond)
{
  if (connected_with(bond))
    return true;
  if (!p_bond_0)
    {
      p_bond_0 = bond;
      p_bond_0->link_up();
      return p_bond_0->connect_to(this);
    }
  else if (!p_bond_1)
    {
      p_bond_1 = bond;
      p_bond_1->link_up();
      return p_bond_1->connect_to(this);
    }
  else if (!p_bond_2)
    {
      p_bond_2 = bond;
      p_bond_2->link_up();
      return p_bond_2->connect_to(this);
    }
  return false;
}

void Carbon::remove_bond(Bond* bond)
{
  if (p_bond_0 == bond)
    {
      p_bond_0->link_down();
      p_bond_0 = 0;
      if (!p_bond_1)
        p_ring_01 = 0;
      if (!p_bond_2)
        p_ring_20 = 0;
    }
  else if (p_bond_1 == bond)
    {
      p_bond_1->link_down();
      p_bond_1 = 0;
      if (!p_bond_2)
        p_ring_12 = 0;
      if (!p_bond_0)
        p_ring_01 = 0;
    }
  else if (p_bond_2 == bond)
    {
      p_bond_2->link_down();
      p_bond_2 = 0;
      if (!p_bond_0)
        p_ring_20 = 0;
      if (!p_bond_1)
        p_ring_12 = 0;
    }
}

bool Carbon::set_ring(Bond* bond_before, Ring* ring, Bond* bond_after)
{
  if (p_bond_0 == bond_before)
    {
      if (p_bond_1 == bond_after)
        {
          if (p_ring_01 && (p_ring_01 != ring))
            return false;
          p_ring_01 = ring;
          return true;
        }
      else if (p_bond_2 == bond_after)
        {
          if (p_ring_20 && (p_ring_20 != ring))
            return false;
          p_ring_20 = ring;
          return true;
        }
    }
  else if (p_bond_1 == bond_before)
    {
      if (p_bond_2 == bond_after)
        {
          if (p_ring_12 && (p_ring_12 != ring))
            return false;
          p_ring_12 = ring;
          return true;
        }
      else if (p_bond_0 == bond_after)
        {
          if (p_ring_01 && (p_ring_01 != ring))
            return false;
          p_ring_01 = ring;
          return true;
        }
    }
  else if (p_bond_2 == bond_before)
    {
      if (p_bond_0 == bond_after)
        {
          if (p_ring_20 && (p_ring_20 != ring))
            return false;
          p_ring_20 = ring;
          return true;
        }
      else if (p_bond_1 == bond_after)
        {
          if (p_ring_12 && (p_ring_12 != ring))
            return false;
          p_ring_12 = ring;
          return true;
        }
    }
  return false;
}

void Carbon::remove_ring(Ring* ring)
{
  if (p_ring_01 == ring)
    {
      p_ring_01 = 0;
      if (!p_ring_12 && p_bond_1)
        {
          p_bond_1->remove_carbon(this);
          p_bond_1->link_down();
          p_bond_1 = 0;
        }
      if (!p_ring_20 && p_bond_0)
        {
          p_bond_0->remove_carbon(this);
          p_bond_0->link_down();
          p_bond_0 = 0;
        }
    }
  else if (p_ring_12 == ring)
    {
      p_ring_12 = 0;
      if (!p_ring_20 && p_bond_2)
        {
          p_bond_2->remove_carbon(this);
          p_bond_2->link_down();
          p_bond_2 = 0;
        }
      if (!p_ring_01 && p_bond_1)
        {
          p_bond_1->remove_carbon(this);
          p_bond_1->link_down();
          p_bond_1 = 0;
        }
    }
  else if (p_ring_20 == ring)
    {
      p_ring_20 = 0;
      if (!p_ring_01 && p_bond_0)
        {
          p_bond_0->remove_carbon(this);
          p_bond_0->link_down();
          p_bond_0 = 0;
        }
      if (!p_ring_12 && p_bond_2)
        {
          p_bond_2->remove_carbon(this);
          p_bond_2->link_down();
          p_bond_2 = 0;
        }
    }
}

int Carbon::compare(const Carbon* you) const
{
  return sequence_no() - you->sequence_no();
}

void Carbon::calculate_distance_around(Queue<Carbon>& operations)
{
  int distance = distance_to_set() + 1;
  if (p_bond_0)
    {
      Carbon* beyond = p_bond_0->get_carbon_beyond(this);
      if (beyond->p_distance_to_set == INT_MAX)
        {
          beyond->p_distance_to_set = distance;
          operations.enqueue(beyond);
        }
    }
  if (p_bond_1)
    {
      Carbon* beyond = p_bond_1->get_carbon_beyond(this);
      if (beyond->p_distance_to_set == INT_MAX)
        {
          beyond->p_distance_to_set = distance;
          operations.enqueue(beyond);
        }
    }
  if (p_bond_2)
    {
      Carbon* beyond = p_bond_2->get_carbon_beyond(this);
      if (beyond->p_distance_to_set == INT_MAX)
        {
          beyond->p_distance_to_set = distance;
          operations.enqueue(beyond);
        }
    }
}

void Carbon::reset_distance_to_set()
{
  p_distance_to_set = INT_MAX;
}

int Carbon::p_calc_clockwise(Carbon* from, Carbon* to)
{
  Bond* from_bond = bond_between(from);
  Bond* to_bond = bond_between(to);
  assert(from_bond);
  assert(to_bond);
  int clockwise = 0;
  if (from_bond == p_bond_0)
    {
      if (to_bond == p_bond_1)
        clockwise = 1;
      else if (to_bond == p_bond_2)
        clockwise = -1;
    }
  else if (from_bond == p_bond_1)
    {
      if (to_bond == p_bond_0)
        clockwise = -1;
      else if (to_bond == p_bond_2)
        clockwise = 1;
    }
  else if (from_bond == p_bond_2)
    {
      if (to_bond == p_bond_0)
        clockwise = 1;
      else if (to_bond == p_bond_1)
        clockwise = -1;
    }
  return clockwise;
}

bool Carbon::check_clockwise(Carbon* from, Carbon* to)
{
  int clockwise = p_calc_clockwise(from, to);
  assert(clockwise);
  assert(p_normal.clockwise);
  return (p_normal.clockwise == clockwise);
}

void Carbon::set_clockwise(Carbon* from, Carbon* to, bool locally)
{
  int clockwise = p_calc_clockwise(from, to);
  assert(clockwise);
  if (p_normal.clockwise)
    {
      assert(p_normal.clockwise == clockwise);
      return;
    }
  p_normal.clockwise = clockwise;
  if (locally)
    return;
  if (p_normal.clockwise > 0)
    {
      if (p_ring_01)
        p_ring_01->set_clockwise(this, p_bond_0->get_carbon_beyond(this));
      if (p_ring_12)
        p_ring_12->set_clockwise(this, p_bond_1->get_carbon_beyond(this));
      if (p_ring_20)
        p_ring_20->set_clockwise(this, p_bond_2->get_carbon_beyond(this));
    }
  else
    {
      if (p_ring_01)
        p_ring_01->set_clockwise(this, p_bond_1->get_carbon_beyond(this));
      if (p_ring_12)
        p_ring_12->set_clockwise(this, p_bond_2->get_carbon_beyond(this));
      if (p_ring_20)
        p_ring_20->set_clockwise(this, p_bond_0->get_carbon_beyond(this));
    }
}

void Carbon::write_representation(Representation& representation, Bond* from)
{
  if (get_done())
    return;
  set_done();
  Bond* first_left = 0;
  Bond* second_right = 0;
  if (p_bond_0 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_1;
          second_right = p_bond_2;
        }
      else
        {
          first_left = p_bond_2;
          second_right = p_bond_1;
        }
    }
  else if (p_bond_1 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_2;
          second_right = p_bond_0;
        }
      else
        {
          first_left = p_bond_0;
          second_right = p_bond_2;
        }
    }
  else if (p_bond_2 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_0;
          second_right = p_bond_1;
        }
      else
        {
          first_left = p_bond_1;
          second_right = p_bond_0;
        }
    }
  if (first_left)
    {
      Carbon* carbon = first_left->get_carbon_beyond(this);
      if (!carbon->get_done())
        {
          representation.set_step('L');
          carbon->write_representation(representation, first_left);
        }
    }
  if (second_right)
    {
      Carbon* carbon = second_right->get_carbon_beyond(this);
      if (!carbon->get_done())
        {
          representation.set_step('R');
          carbon->write_representation(representation, second_right);
        }
    }
  representation.set_step('b');
}

void Carbon::write_automorphism(Automorphism& automorphism, Bond* from)
{
  if (get_done())
    return;
  set_done();
  Bond* first_left = 0;
  Bond* second_right = 0;
  if (p_bond_0 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_1;
          second_right = p_bond_2;
        }
      else
        {
          first_left = p_bond_2;
          second_right = p_bond_1;
        }
    }
  else if (p_bond_1 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_2;
          second_right = p_bond_0;
        }
      else
        {
          first_left = p_bond_0;
          second_right = p_bond_2;
        }
    }
  else if (p_bond_2 == from)
    {
      if (p_normal.clockwise > 0)
        {
          first_left = p_bond_0;
          second_right = p_bond_1;
        }
      else
        {
          first_left = p_bond_1;
          second_right = p_bond_0;
        }
    }
  automorphism.set_step(sequence_no());
  if (first_left)
    {
      Carbon* carbon = first_left->get_carbon_beyond(this);
      carbon->write_automorphism(automorphism, first_left);
    }
  if (second_right)
    {
      Carbon* carbon = second_right->get_carbon_beyond(this);
      carbon->write_automorphism(automorphism, second_right);
    }
}

void Carbon::print_structure(int indent, bool deep) const
{
  for (int i = 0; i < indent; ++i)
    printf("  ");
  printf("Carbon(%d)%c", sequence_no(),
         (p_normal.clockwise > 0) ? '+' : ((p_normal.clockwise < 0) ? '-' : '0'));
  if (deep)
    {
      printf("\n");
      if (p_bond_0)
        {
          p_bond_0->print_structure(indent + 1, false);
          printf("[");
          p_bond_0->get_carbon_beyond(this)->print_structure(0, false);
          printf("]\n");
        }
      if (p_ring_01)
        {
          for (int i = 0; i < indent + 1; ++i)
            printf("  ");
          printf("[");
          p_ring_01->print_structure(0, false);
          printf("]\n");
        }
      if (p_bond_1)
        {
          p_bond_1->print_structure(indent + 1, false);
          printf("[");
          p_bond_1->get_carbon_beyond(this)->print_structure(0, false);
          printf("]\n");
        }
      if (p_ring_12)
        {
          for (int i = 0; i < indent + 1; ++i)
            printf("  ");
          printf("[");
          p_ring_12->print_structure(0, false);
          printf("]\n");
        }
      if (p_bond_2)
        {
          p_bond_2->print_structure(indent + 1, false);
          printf("[");
          p_bond_2->get_carbon_beyond(this)->print_structure(0, false);
          printf("]\n");
        }
      if (p_ring_20)
        {
          for (int i = 0; i < indent + 1; ++i)
            printf("  ");
          printf("[");
          p_ring_20->print_structure(0, false);
          printf("]\n");
        }
    }
}

void Carbon::print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const
{
  Vector3 loc = carbon_location();
  fprintf(fptr, "sphere {<%g,%g,%g>,%g texture {pigment "
          "{color red %g green %g blue %g}}}\n",
          loc.x(), loc.y(), loc.z(), p_radius, ((p_color>>16)&255)/255.0,
          ((p_color>>8)&255)/255.0, ((p_color>>0)&255)/255.0);
  if (ca->print_out_sequence_no)
    {
      fprintf(fptr, "text {ttf \"crystal.ttf\", \"%d\", 0.1, x*(-0.01) pigment "
              "{color red 0.4 green 1.0 blue 0.4} "
              "scale 0.6 rotate y*180 translate <%g,%g,%g>}\n",
              sequence_no(), loc.x(), loc.y(), loc.z() + 1.2 * p_radius);
    }
#if defined(CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_ANIMATION)
  Vector3 loc1 = get_normal() * 2.0 + loc;
  fprintf(fptr, "cylinder {<%g,%g,%g>,<%g,%g,%g>,%g texture {pigment "
          "{color red %g green %g blue %g}}}\n",
          loc.x(), loc.y(), loc.z(), loc1.x(), loc1.y(), loc1.z(), 0.1,
          0.1, 1.0, 0.1);
#endif
}

void Carbon::print_POVRay_scene_description(const CarbonAllotrope* UNUSED(ca), FILE* fptr,
                                            const Matrix3& rot, const Vector3& move,
                                            bool clipped_by_Z_non_negative) const
{
  Vector3 loc = carbon_location() * rot + move;
  if ((loc.z() > p_radius) || (!clipped_by_Z_non_negative))
    {
      fprintf(fptr, "sphere {<%g,%g,%g>,%g ", loc.x(), loc.y(), loc.z(), p_radius);
      fprintf(fptr, "texture {pigment {color red %g green %g blue %g}}}\n",
              ((p_color>>16)&255)/255.0, ((p_color>>8)&255)/255.0,
              ((p_color>>0)&255)/255.0);
#if defined(CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS)
      Vector3 loc1 = (get_normal() * 2.0) * rot + loc;
      fprintf(fptr, "cylinder {<%g,%g,%g>,<%g,%g,%g>,%g ",
              loc.x(), loc.y(), loc.z(), loc1.x(), loc1.y(), loc1.z(), 0.1);
      fprintf(fptr, "texture {pigment {color red %g green %g blue %g}}}\n",
              0.1, 1.0, 0.1);
#endif
    }
  else if (loc.z() >= -p_radius)
    {
      fprintf(fptr, "intersection {\n");
      fprintf(fptr, "  sphere {<%g,%g,%g>,%g}\n", loc.x(), loc.y(), loc.z(), p_radius);
      fprintf(fptr, "  plane {<0,0,-1>,0}\n");
      fprintf(fptr, "  texture {pigment {color red %g green %g blue %g}}\n",
              ((p_color>>16)&255)/255.0, ((p_color>>8)&255)/255.0,
              ((p_color>>0)&255)/255.0);
      fprintf(fptr, "}\n");
#if defined(CONFIG_DRAW_NORMAL_VECTOR_IN_POVRAY_SIX_VIEWS)
      Vector3 loc1 = (get_normal() * 2.0) * rot + loc;
      fprintf(fptr, "intersection {\n");
      fprintf(fptr, "  cylinder {<%g,%g,%g>,<%g,%g,%g>,%g}\n",
              loc.x(), loc.y(), loc.z(), loc1.x(), loc1.y(), loc1.z(), 0.1);
      fprintf(fptr, "  plane {<0,0,-1>,0}\n");
      fprintf(fptr, "  texture {pigment {color red %g green %g blue %g}}\n",
              0.1, 1.0, 0.1);
      fprintf(fptr, "}\n");
#endif
    }
}

void Carbon::draw_opaque_by_OpenGL(bool selection) const
{
  Vector3 loc = carbon_location();
  if (selection)
    OpenGLUtil::naming_start(sequence_no());
  OpenGLUtil::set_color(p_color);
  OpenGLUtil::draw_sphere(p_radius, loc);
  if (selection)
    OpenGLUtil::naming_end();
#if defined(CONFIG_DRAW_NORMAL_VECTOR_IN_GURUGURU_MODE)
  Vector3 loc1 = get_normal() * 2.0 + loc;
  OpenGLUtil::set_color(0x19ff19);
  OpenGLUtil::draw_cylinder(0.1, loc, loc1);
#endif
}

Vector3 Carbon::carbon_location() const
{
  return get_center_location();
}

Vector3 Carbon::bond_location(Bond* bond) const
{
  if (bond == p_bond_0)
    return get_vertex_location(0);
  else if (bond == p_bond_1)
    return get_vertex_location(1);
  else if (bond == p_bond_2)
    return get_vertex_location(2);
  return Vector3();
}

int Carbon::number_of_rings() const
{
  int result = 0;
  if (p_ring_01)
    ++result;
  if (p_ring_12)
    ++result;
  if (p_ring_20)
    ++result;
  return result;
}

Ring* Carbon::get_ring(int index) const
{
  if (p_ring_01)
    {
      if (index == 0)
        return p_ring_01;
      --index;
    }
  if (p_ring_12)
    {
      if (index == 0)
        return p_ring_12;
      --index;
    }
  if (p_ring_20)
    {
      if (index == 0)
        return p_ring_20;
      --index;
    }
  return 0;
}

int Carbon::number_of_bonds() const
{
  int result = 0;
  if (p_bond_0)
    ++result;
  if (p_bond_1)
    ++result;
  if (p_bond_2)
    ++result;
  return result;
}

Bond* Carbon::get_bond(int index) const
{
  if (p_bond_0)
    {
      if (index == 0)
        return p_bond_0;
      --index;
    }
  if (p_bond_1)
    {
      if (index == 0)
        return p_bond_1;
      --index;
    }
  if (p_bond_2)
    {
      if (index == 0)
        return p_bond_2;
      --index;
    }
  return 0;
}

bool Carbon::connected_with(Bond* bond) const
{
  if (p_bond_0 == bond)
    return true;
  else if (p_bond_1 == bond)
    return true;
  else if (p_bond_2 == bond)
    return true;
  return false;
}

Bond* Carbon::bond_between(Carbon* carbon) const
{
  if (p_bond_0)
    {
      Carbon* beyond = p_bond_0->get_carbon_beyond(this);
      if (beyond == carbon)
        return p_bond_0;
    }
  if (p_bond_1)
    {
      Carbon* beyond = p_bond_1->get_carbon_beyond(this);
      if (beyond == carbon)
        return p_bond_1;
    }
  if (p_bond_2)
    {
      Carbon* beyond = p_bond_2->get_carbon_beyond(this);
      if (beyond == carbon)
        return p_bond_2;
    }
  return 0;
}

Carbon* Carbon::carbon_beyond(Bond* bond) const
{
  if (p_bond_0 == bond)
    return p_bond_0->get_carbon_beyond(this);
  else if (p_bond_1 == bond)
    return p_bond_1->get_carbon_beyond(this);
  else if (p_bond_2 == bond)
    return p_bond_2->get_carbon_beyond(this);
  else
    return 0;
}

Ring* Carbon::ring_between(Bond* bond_before, Bond* bond_after) const
{
  if (p_bond_0 == bond_before)
    {
      if (p_bond_1 == bond_after)
        return p_ring_01;
      else if (p_bond_2 == bond_after)
        return p_ring_20;
    }
  if (p_bond_1 == bond_before)
    {
      if (p_bond_2 == bond_after)
        return p_ring_12;
      else if (p_bond_0 == bond_after)
        return p_ring_01;
    }
  if (p_bond_2 == bond_before)
    {
      if (p_bond_0 == bond_after)
        return p_ring_20;
      else if (p_bond_1 == bond_after)
        return p_ring_12;
    }
  return 0;
}

Bond* Carbon::bond_between(Ring* ring_before, Ring* ring_after) const
{
  if (p_ring_01 == ring_before)
    {
      if (p_ring_12 == ring_after)
        return p_bond_1;
      else if (p_ring_20 == ring_after)
        return p_bond_0;
    }
  else if (p_ring_12 == ring_before)
    {
      if (p_ring_20 == ring_after)
        return p_bond_2;
      else if (p_ring_01 == ring_after)
        return p_bond_1;
    }
  else if (p_ring_20 == ring_before)
    {
      if (p_ring_01 == ring_after)
        return p_bond_0;
      else if (p_ring_12 == ring_after)
        return p_bond_2;
    }
  return 0;
}

bool Carbon::open_side_bond(Bond* bond, Bond*& result) const
{
  if (p_bond_0 == bond)
    {
      if (p_ring_01 && !p_ring_20)
        {
          result = p_bond_2;
          return true;
        }
      if (!p_ring_01 && p_ring_20)
        {
          result = p_bond_1;
          return true;
        }
    }
  else if (p_bond_1 == bond)
    {
      if (p_ring_01 && !p_ring_12)
        {
          result = p_bond_2;
          return true;
        }
      if (!p_ring_01 && p_ring_12)
        {
          result = p_bond_0;
          return true;
        }
    }
  else if (p_bond_2 == bond)
    {
      if (p_ring_20 && !p_ring_12)
        {
          result = p_bond_1;
          return true;
        }
      if (!p_ring_20 && p_ring_12)
        {
          result = p_bond_0;
          return true;
        }
    }
  return false;
}

Bond* Carbon::boundary_bond(Bond* bond) const
{
  if (bond)
    {
      if (p_bond_0 == bond)
        {
          if (!p_ring_01)
            return p_ring_12 ? p_bond_1 : p_bond_2;
          if (!p_ring_20)
            return p_ring_12 ? p_bond_2 : p_bond_1;
        }
      if (p_bond_1 == bond)
        {
          if (!p_ring_12)
            return p_ring_20 ? p_bond_2 : p_bond_0;
          if (!p_ring_01)
            return p_ring_20 ? p_bond_0 : p_bond_2;
        }
      if (p_bond_2 == bond)
        {
          if (!p_ring_20)
            return p_ring_01 ? p_bond_0 : p_bond_1;
          if (!p_ring_12)
            return p_ring_01 ? p_bond_1 : p_bond_0;
        }
      return 0;
    }
  else if (p_normal.clockwise == +1)
    {
      /* boundary_bond は、開を左手に見て閉を右手に見る境界ボンドを検索する */
      int rings = number_of_rings();
      if (rings == 0)
        return 0;
      else if (rings == 1)
        {
          if (p_ring_01)
            return p_bond_1;
          else if (p_ring_12)
            return p_bond_2;
          else
            return p_bond_0;
        }
      else if (rings == 2)
        {
          if (!p_ring_01)
            return p_bond_0;
          else if (!p_ring_12)
            return p_bond_1;
          else
            return p_bond_2;
        }
      else
        return 0;
    }
  else /* clockwise == -1 */
    {
      /* clockwise == -1 なので、clockwise == +1 の時の逆 */
      int rings = number_of_rings();
      if (rings == 0)
        return 0;
      else if (rings == 1)
        {
          if (p_ring_01)
            return p_bond_0;
          else if (p_ring_12)
            return p_bond_1;
          else
            return p_bond_2;
        }
      else if (rings == 2)
        {
          if (!p_ring_01)
            return p_bond_1;
          else if (!p_ring_12)
            return p_bond_2;
          else
            return p_bond_0;
        }
      else
        return 0;
    }
}

Bond* Carbon::inner_bond() const
{
  if (number_of_rings() == 2)
    {
      if (!p_ring_01)
        return p_bond_2;
      if (!p_ring_12)
        return p_bond_0;
      if (!p_ring_20)
        return p_bond_1;
    }
  return 0;
}

// open convex boundary segment とは、
//   １面の炭素-１面の炭素-１面の炭素...-１面の炭素
// という形のボンドの開いたつながり。
// closed convex boundary segment とは、
//   １面の炭素-１面の炭素-１面の炭素...-１面の炭素-最初の炭素
// という形のボンドの閉じたつながり。
// open convex boundary segment と closed convex boundary segment とをあわせて
// convex boundary segment と呼ぶ。
// convex boundary segment の長さとは、ボンドの数。

// open concave boundary segment とは、
//   １面の炭素-２面の炭素-２面の炭素...-１面の炭素
// という形のボンドの開いたつながり。
// closed concave boundary segment とは、
//   ２面の炭素-２面の炭素-２面の炭素...-２面の炭素-最初の炭素
// という形のボンドの閉じたつながり。
// open concave boundary segment と closed concave boundary segment とをあわせて
// concave boundary segment と呼ぶ。
// concave bounadry segment の長さとは、ボンドの数。

// convex boundary とは、一つの closed convex boundary segment のこと。
// concave boundary とは、一つの closed concave boundary segment のこと。
// mixed boundary とは、互い違いに並び輪になった open convex boundary segment と
// open concave boundary segment の連続のこと。

// 本メソッド呼び出しは、this は２面の炭素でなければならない。
// 返り値はエラーコード。
// length は concave boundary segment の長さを返す。
// end_bond は、以下のとおり。
// end_bond の片側の炭素が１面で、もう片側の炭素が２面
//    .... open concave boundary segment の端ボンド
// end_bond の両側の炭素は２面
//     ... closed concave boundary segment のボンドの一つ
//
ErrorCode
Carbon::concave_boundary_segment(int n_members, int& length, Bond*& end_bond)
{
  end_bond = 0;
  Carbon* end_carbon = 0;
  assert(number_of_rings() == 2);
  assert(number_of_bonds() == 3);
  Bond* bond_connection = boundary_bond(0);
  Bond* bond = bond_connection;
  Carbon* carbon = this;
  bool has_pentagon = false;
  length = 0;
  while (1)
    {
      if (carbon->has_pentagon())
        has_pentagon = true;
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      carbon = bond->get_carbon_beyond(carbon);
      length++;
      if (bond == bond_connection)
        {
          end_bond = bond;
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
          printf("Closed concave boundary segment (length=%d) ...C%d...\n",
                 length, carbon->sequence_no());
#endif
          if ((n_members == 5) && has_pentagon)
            return ERROR_CODE_BREAKING_ISOLATED_PENTAGON_RULE;
          if (length != n_members)
            return ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_CLOSED_CONCAVE_BOUNDARY_SEGMENT;
          return ERROR_CODE_OK;
        }
    }
  end_bond = bond;
  end_carbon = carbon;
  int seq = carbon->sequence_no();
  bond_connection = boundary_bond(bond_connection);
  bond = bond_connection;
  carbon = this;
  while (1)
    {
      if (carbon->has_pentagon())
        has_pentagon = true;
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      carbon = bond->get_carbon_beyond(carbon);
      length++;
    }
  Carbon* more_carbon = carbon;
  Bond* more_bond = bond;
  while (1)
    {
      if (more_carbon->number_of_rings() == 2)
        break;
      if (more_carbon == end_carbon)
        {
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
          printf("Only one concave and only one convex boundary segment (length=%d) "
                 "C%d-...-C%d\n",
                 length, end_carbon->sequence_no(), carbon->sequence_no());
#endif
          return ERROR_CODE_ONLY_ONE_CONCAVE_AND_ONLY_ONE_CONVEX_BOUNDARY_SEGMENT;
        }
      more_bond = more_carbon->boundary_bond(more_bond);
      more_carbon = more_bond->get_carbon_beyond(more_carbon);
    }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("Open concave boundary segment (length=%d) C%d-...-C%d\n",
         length, end_carbon->sequence_no(), carbon->sequence_no());
#endif
  if (carbon->sequence_no() < seq)
    {
      end_bond = bond;
      end_carbon = carbon;
    }
  if ((n_members == 5) && has_pentagon)
    return ERROR_CODE_BREAKING_ISOLATED_PENTAGON_RULE;
  if (length + 1 > n_members)
    return ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_OPEN_CONCAVE_BOUNDARY_SEGMENT;
  return ERROR_CODE_OK;
}

bool Carbon::has_pentagon() const
{
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      if (get_ring(i)->number_of_carbons() == 5)
        return true;
    }
  return false;
}

bool Carbon::has_non_hexagon() const
{
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      if (get_ring(i)->number_of_carbons() != 6)
        return true;
    }
  return false;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
