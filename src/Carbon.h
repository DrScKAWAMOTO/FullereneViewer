/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/26 18:06:18 JST
 */

#ifndef __CARBON_H__
#define __CARBON_H__

#include <stdio.h>
#include "ErrorCode.h"
#include "InteractiveRegularPolygon.h"
#include "Queue.h"
#include "List.h"

class Bond;
class Ring;
class Path;
class Representation;
class Automorphism;
class CarbonAllotrope;

class Carbon : public InteractiveRegularPolygon {
  // friend classes & functions
  friend class Ring;

  // members
private:
  double p_radius;
  int p_color;
  Bond* p_bond_0;
  Ring* p_ring_01;
  Bond* p_bond_1;
  Ring* p_ring_12;
  Bond* p_bond_2;
  Ring* p_ring_20;
  int p_distance_to_set;

  // private tools

  // constructors & the destructor
public:
  Carbon(CarbonAllotrope* ca);
  virtual ~Carbon();
  Carbon& operator = (const Carbon& you); /* dont use */
  void copy_from(const CarbonAllotrope* ca, const Carbon* you);
  bool connect_to(CarbonAllotrope* ca, Carbon* carbon);
  bool connect_to(CarbonAllotrope* ca, Bond* bond);
  void remove_bond(Bond* bond);
  bool set_ring(Bond* bond_before, Ring* ring, Bond* bond_after);
  void remove_ring(Ring* ring);

  // comparators
public:
  int compare(const Carbon* you) const;

  // distance
public:
  void calculate_distance_around(Queue<Carbon>& operations);
  int distance_to_set() { return p_distance_to_set; }
  void set_distance_to_set(int distance) { p_distance_to_set = distance; }
  void reset_distance_to_set();

  // determining normal vector
public:
  bool set_clockwise(Carbon* from, Carbon* to);

  // interactions

  // representation
public:
  void write_representation(Representation& representation, Bond* from);
  void write_automorphism(Automorphism& automorphism, Bond* from);

  // I/O
public:
  void print_structure(int indent = 0, bool deep = true) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr,
                                      const Matrix3& rot, const Vector3& move,
                                      bool clipped_by_Z_non_negative) const;
  virtual void draw_opaque_by_OpenGL(bool selection) const;
  Vector3 carbon_location() const;
  Vector3 bond_location(Bond* bond) const;

  // member accessing methods
public:
  int number_of_rings() const;
  Ring* get_ring(int index) const;
  int number_of_bonds() const;
  Bond* get_bond(int index) const;
  bool connected_with(Bond* bond) const;
  Bond* bond_between(Carbon* carbon) const;
  Carbon* carbon_beyond(Bond* bond) const;
  Ring* ring_between(Bond* bond_before, Bond* bond_after) const;
  Bond* bond_between(Ring* ring_before, Ring* ring_after) const;
  bool open_side_bond(Bond* bond, Bond*& result) const;
  Bond* boundary_bond(Bond* bond = 0) const;
  Bond* inner_bond() const;
  void set_color(int color) { p_color = color; }
  ErrorCode concave_boundary_segment(int n_members, int& length, Bond*& end_bond);
  bool has_pentagon() const;
  bool has_non_hexagon() const;

};

#endif /* __CARBON_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

