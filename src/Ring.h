/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/31 18:21:00 JST
 */

#ifndef __RING_H__
#define __RING_H__

#include "InteractiveRegularPolygon.h"
#include "List.h"
#include "Carbon.h"

class Bond;
class CarbonAllotrope;

class Ring : public InteractiveRegularPolygon {
  // friend classes & functions

  // members
private:
  int p_number_of_carbons;
  List<Carbon> p_carbons;
  // TODO not used Vector3 p_center_location;
  double p_radius;
  int p_clockwise;
  int p_ring_color;

  // private tools
private:
  void p_make_carbons(CarbonAllotrope* ca, Bond* bond_connection);
  void p_make_carbons(CarbonAllotrope* ca, const int* carbon_sequence_nos);
  void p_print_POVRay_semitransparent(FILE* fptr, const Vector3& one,
                                      const Vector3& two, const Vector3& three) const;
  Vector3
  p_clip_by_Z_non_negative(const Vector3& negative, const Vector3& positive) const;
  void p_print_POVRay_semitransparent_clipped_by_Z_non_negative(FILE* fptr,
                                                                Vector3 one,
                                                                Vector3 two,
                                                                Vector3 three) const;

  // constructors & the destructor
public:
  Ring(CarbonAllotrope* ca, int number_of_carbons);
  Ring(CarbonAllotrope* ca, int number_of_carbons, Bond* bond_connection);
  Ring(CarbonAllotrope* ca, int number_of_carbons, Carbon* carbon_connection);
  Ring(CarbonAllotrope* ca, int number_of_carbons, const int* carbon_sequence_nos);
  virtual ~Ring();
  Ring& operator = (const Ring& that); /* dont use */

  // distance
public:
  int distance_to_set();

  // determining normal vector
public:
  void reset_clockwise();
  bool set_clockwise(Carbon* from, Carbon* to);

  // interactions

  // I/O
public:
  void print_structure(int indent = 0, bool deep = true) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr,
                                      const Matrix3& rot, const Vector3& move,
                                      bool clipped_by_Z_non_negative) const;
  virtual void draw_semitransparent_by_OpenGL(bool selection, bool frontface) const;

  // member accessing methods
public:
  int number_of_carbons() const { return p_number_of_carbons; }
  virtual Vector3 get_normal() const;
  Carbon* get_carbon(int index);
  const Carbon* get_carbon(int index) const;
  Bond* get_bond_after(int index);
  Bond* get_bond_before(int index);
  void set_color(int color);

};

#endif /* __RING_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
