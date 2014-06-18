/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/31 18:21:00 JST
 */

#include <assert.h>
#include <float.h>
#include "Ring.h"
#include "Bond.h"
#include "CarbonAllotrope.h"
#include "OpenGLUtil.h"
#include "AvoidBugs.h"
#include "DebugMemory.h"

static int index = 0;
static int five_membered_ring_colors[12] = {
  0xff0000, 0xc04000, 0x808000, 0x40c000, 0x00ff00, 0x00c040,
  0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040
};

void Ring::p_make_carbons(CarbonAllotrope* ca, Bond* bond_connection)
{
  Carbon* carbon_left = bond_connection->get_left_carbon();
  Carbon* carbon_right = bond_connection->get_right_carbon();
  Carbon* carbon;
  if ((carbon_left->number_of_rings() == 2) && (carbon_right->number_of_rings() == 1))
    carbon = carbon_right;
  else
    carbon = carbon_left;
  Bond* bond = bond_connection;
  bool closed = false;
  while (1)
    {
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      if (bond == bond_connection)
        {
          closed = true;
          break;
        }
      carbon = bond->get_carbon_beyond(carbon);
    }
  int i = 0;
  p_carbons.add(carbon);
  i++;
  carbon = bond->get_carbon_beyond(carbon);
  p_carbons.add(carbon);
  i++;
  while (i < p_number_of_carbons)
    {
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      carbon = bond->get_carbon_beyond(carbon);
      p_carbons.add(carbon);
      i++;
    }
  while (i < p_number_of_carbons)
    {
      Carbon* carbon2 = new Carbon(ca);
      carbon->connect_to(ca, carbon2);
      carbon = carbon2;
      p_carbons.add(carbon);
      i++;
    }
  if (!closed)
    p_carbons[0]->connect_to(ca, p_carbons[p_number_of_carbons - 1]);
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      int j = (i + 1) % p_number_of_carbons;
      int k = (j + 1) % p_number_of_carbons;
      Bond* bond_before = p_carbons[i]->bond_between(p_carbons[j]);
      Bond* bond_after = p_carbons[j]->bond_between(p_carbons[k]);
      p_carbons[j]->set_ring(bond_before, this, bond_after);
    }
}

void Ring::p_make_carbons(CarbonAllotrope* ca, const int* carbon_sequence_nos)
{
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      Carbon* carbon;
      if (carbon_sequence_nos[i])
        {
          carbon = ca->get_carbon_by_sequence_no(carbon_sequence_nos[i]);
          assert(carbon->number_of_rings() == 1);
        }
      else
        carbon = new Carbon(ca);
      p_carbons.add(carbon);
    }
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      Carbon* carbon1 = p_carbons[i];
      Carbon* carbon2 = p_carbons[(i + 1) % p_number_of_carbons];
      if (carbon1->bond_between(carbon2) == 0)
        carbon1->connect_to(ca, carbon2);
    }
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      int j = (i + 1) % p_number_of_carbons;
      int k = (j + 1) % p_number_of_carbons;
      Bond* bond_before = p_carbons[i]->bond_between(p_carbons[j]);
      Bond* bond_after = p_carbons[j]->bond_between(p_carbons[k]);
      p_carbons[j]->set_ring(bond_before, this, bond_after);
    }
}

void Ring::p_print_POVRay_semitransparent(FILE* fptr, const Vector3& one,
                                          const Vector3& two, const Vector3& three) const
{
  fprintf(fptr, "polygon {4, <%g,%g,%g>,", one.x(), one.y(), one.z());
  fprintf(fptr, "<%g,%g,%g>,", two.x(), two.y(), two.z());
  fprintf(fptr, "<%g,%g,%g>,", three.x(), three.y(), three.z());
  fprintf(fptr, "<%g,%g,%g> ", one.x(), one.y(), one.z());
  fprintf(fptr, "pigment {color rgb <%g,%g,%g> filter 1.0} interior {ior 1.0}}\n",
          ((p_ring_color>>16)&255)/255.0, ((p_ring_color>>8)&255)/255.0,
          ((p_ring_color>>0)&255)/255.0);
}

Vector3
Ring::p_clip_by_Z_non_negative(const Vector3& negative, const Vector3& positive) const
{
  assert(negative.z() < 0.0);
  assert(positive.z() >= 0.0);
  double ratio = positive.z() / (positive.z() - negative.z());
  return Vector3((negative.x() - positive.x()) * ratio + positive.x(),
                 (negative.y() - positive.y()) * ratio + positive.y(),
                 0.0);
}

void Ring::p_print_POVRay_semitransparent_clipped_by_Z_non_negative(FILE* fptr,
                                                                    Vector3 one,
                                                                    Vector3 two,
                                                                    Vector3 three) const
{
  if ((one.z() < 0.0) && (two.z() < 0.0) && (three.z() < 0.0))
    return;
  if ((one.z() == 0.0) && (two.z() == 0.0) && (three.z() == 0.0))
    return;
  if ((one.z() >= 0.0) && (two.z() >= 0.0) && (three.z() >= 0.0))
    {
      p_print_POVRay_semitransparent(fptr, one, two, three);
      return;
    }
  fprintf(fptr, "intersection {\n  plane {<0,0,-1>,0}\n  ");
  p_print_POVRay_semitransparent(fptr, one, two, three);
  fprintf(fptr, "}\n");
}

Ring::Ring(CarbonAllotrope* ca, int number_of_carbons)
  : InteractiveRegularPolygon(ca, ca->ring_next_sequence++, 1.0, number_of_carbons),
    p_number_of_carbons(number_of_carbons), p_clockwise(0),
    p_ring_color(five_membered_ring_colors[index])
{
  if (number_of_carbons != 6)
    ++index;
  if (index == 12)
    index = 0;
  for (int i = 0; i < p_number_of_carbons; ++i)
    p_carbons.add(new Carbon(ca));
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      int j = (i + 1) % p_number_of_carbons;
      p_carbons[i]->connect_to(ca, p_carbons[j]);
    }
  for (int i = 0; i < p_number_of_carbons; ++i)
    {
      int j = (i + 1) % p_number_of_carbons;
      int k = (j + 1) % p_number_of_carbons;
      Bond* bond_before = p_carbons[i]->bond_between(p_carbons[j]);
      Bond* bond_after = p_carbons[j]->bond_between(p_carbons[k]);
      p_carbons[j]->set_ring(bond_before, this, bond_after);
    }
  ca->register_ring(this);
}

Ring::Ring(CarbonAllotrope* ca, int number_of_carbons, Bond* bond_connection)
  : InteractiveRegularPolygon(ca, ca->ring_next_sequence++, 1.0, number_of_carbons),
    p_number_of_carbons(number_of_carbons), p_clockwise(0),
    p_ring_color(five_membered_ring_colors[index])
{
  if (number_of_carbons != 6)
    ++index;
  if (index == 12)
    index = 0;
  p_make_carbons(ca, bond_connection);
  ca->register_ring(this);
}

Ring::Ring(CarbonAllotrope* ca, int number_of_carbons, Carbon* carbon_connection)
  : InteractiveRegularPolygon(ca, ca->ring_next_sequence++, 1.0, number_of_carbons),
    p_number_of_carbons(number_of_carbons), p_clockwise(0),
    p_ring_color(five_membered_ring_colors[index])
{
  if (number_of_carbons != 6)
    ++index;
  if (index == 12)
    index = 0;
  assert(carbon_connection->number_of_rings() == 2);
  Carbon* carbon = carbon_connection;
  assert(carbon->number_of_bonds() == 3);
  Bond* bond0 = carbon->get_bond(0);
  Bond* bond1 = carbon->get_bond(1);
  Bond* bond2 = carbon->get_bond(2);
  Bond* bond_connection = ((carbon->ring_between(bond0, bond1)) ? bond2 : bond0);
  p_make_carbons(ca, bond_connection);
  ca->register_ring(this);
}

Ring::Ring(CarbonAllotrope* ca, int number_of_carbons, const int* carbon_sequence_nos)
  : InteractiveRegularPolygon(ca, ca->ring_next_sequence++, 1.0, number_of_carbons),
    p_number_of_carbons(number_of_carbons), p_clockwise(0),
    p_ring_color(five_membered_ring_colors[index])
{
  if (number_of_carbons != 6)
    ++index;
  if (index == 12)
    index = 0;
  p_make_carbons(ca, carbon_sequence_nos);
  ca->register_ring(this);
}

Ring::~Ring()
{
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    p_carbons[i]->remove_ring(this);
}

int Ring::distance_to_set()
{
  int distance = 0;
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      int dist = p_carbons[i]->distance_to_set();
      if (dist > distance)
        distance = dist;
    }
  return distance;
}

void Ring::reset_clockwise()
{
  p_clockwise = 0;
}

bool Ring::set_clockwise(Carbon* from, Carbon* to)
{
  int len = p_carbons.length();
  int clockwise = 0;
  for (int i = 0; i < len; ++i)
    {
      Carbon* my_from = p_carbons[i];
      Carbon* my_to = p_carbons[(i + 1) % len];
      if ((my_from == from) && (my_to == to))
        {
          clockwise = 1;
          break;
        }
      else if ((my_from == to) && (my_to == from))
        {
          clockwise = -1;
          break;
        }
    }
  assert(clockwise);
  if (p_clockwise)
    return (p_clockwise == clockwise);
  p_clockwise = clockwise;
  if (p_clockwise > 0)
    {
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon0 = p_carbons[i];
          Carbon* carbon1 = p_carbons[(i + 1) % len];
          Carbon* carbon2 = p_carbons[(i + 2) % len];
          if (!carbon1->set_clockwise(carbon2, carbon0))
            return false;
        }
    }
  else
    {
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon0 = p_carbons[i];
          Carbon* carbon1 = p_carbons[(i + 1) % len];
          Carbon* carbon2 = p_carbons[(i + 2) % len];
          if (!carbon1->set_clockwise(carbon0, carbon2))
            return false;
        }
    }
  return true;
}

void Ring::print_structure(int indent, bool deep) const
{
  for (int i = 0; i < indent; ++i)
    printf("  ");
  printf("%d-membered ring(%d)", p_number_of_carbons, sequence_no());
  if (deep)
    {
      printf("\n");
      for (int i = 0; i < p_number_of_carbons; ++i)
        p_carbons[i]->print_structure(indent + 1, deep);
    }
}

void Ring::print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const
{
  int num = number_of_carbons();
  if (num != 6)
    {
      Vector3 center = Vector3();
      for (int i = 0; i < num; ++i)
        center += get_carbon(i)->carbon_location();
      center /= (double)num;
      for (int i = 0; i < num; ++i)
        {
          Vector3 loc0 = get_carbon(i)->carbon_location();
          Vector3 loc1 = get_carbon((i + 1) % num)->carbon_location();
          fprintf(fptr, "polygon {4, <%g,%g,%g>,", center.x(), center.y(), center.z());
          fprintf(fptr, "<%g,%g,%g>,", loc0.x(), loc0.y(), loc0.z());
          fprintf(fptr, "<%g,%g,%g>,", loc1.x(), loc1.y(), loc1.z());
          fprintf(fptr, "<%g,%g,%g> pigment {color rgb <%g,%g,%g> filter 1.0}"
                  " interior {ior 1.0}}\n", center.x(), center.y(), center.z(),
                  ((p_ring_color>>16)&255)/255.0, ((p_ring_color>>8)&255)/255.0,
                  ((p_ring_color>>0)&255)/255.0);
        }
    }
}

void Ring::
print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr,
                               const Matrix3& rot, const Vector3& move,
                               bool clipped_by_Z_non_negative) const
{
  int num = number_of_carbons();
  if (num != 6)
    {
      if (clipped_by_Z_non_negative)
        {
          int i;
          for (i = 0; i < num; ++i)
            {
              Vector3 loc = get_carbon(i)->carbon_location() * rot + move;
              if (loc.z() > DBL_EPSILON)
                break;
            }
          if (i == num)
            return;
        }
      Vector3 center = Vector3();
      for (int i = 0; i < num; ++i)
        center += get_carbon(i)->carbon_location();
      center /= (double)num;
      center *= rot;
      center += move;
      for (int i = 0; i < num; ++i)
        {
          Vector3 loc0 = get_carbon(i)->carbon_location() * rot + move;
          Vector3 loc1 = get_carbon((i + 1) % num)->carbon_location() * rot + move;
          if (clipped_by_Z_non_negative)
            p_print_POVRay_semitransparent_clipped_by_Z_non_negative(fptr, center,
                                                                     loc0, loc1);
          else
            p_print_POVRay_semitransparent(fptr, center, loc0, loc1);
        }
    }
}

void Ring::draw_semitransparent_by_OpenGL(bool selection, bool frontface) const
{
  int num = number_of_carbons();
  if (num != 6)
    {
      OpenGLUtil::set_color(p_ring_color, 0.9);
      OpenGLUtil::draw_ring(frontface, this);
    }
}

Vector3 Ring::get_normal() const
{
  Vector3 normal;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    normal += get_carbon(i)->get_normal();
  normal /= (double)len;
  return normal / normal.abs();
}

Carbon* Ring::get_carbon(int index)
{
  assert((index >= 0) && (index < number_of_carbons()));
  return p_carbons[index];
}

const Carbon* Ring::get_carbon(int index) const
{
  assert((index >= 0) && (index < number_of_carbons()));
  return p_carbons[index];
}

Bond* Ring::get_bond_after(int index)
{
  assert((index >= 0) && (index < number_of_carbons()));
  int j = (index + 1) % p_number_of_carbons;
  return p_carbons[index]->bond_between(p_carbons[j]);
}

Bond* Ring::get_bond_before(int index)
{
  assert((index >= 0) && (index < number_of_carbons()));
  int j = (index - 1 + p_number_of_carbons) % p_number_of_carbons;
  return p_carbons[index]->bond_between(p_carbons[j]);
}

void Ring::set_color(int color)
{
  for (int i = 0; i < p_number_of_carbons; ++i)
    p_carbons[i]->set_color(color);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
