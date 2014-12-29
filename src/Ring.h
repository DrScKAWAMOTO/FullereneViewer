/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __RING_H__
#define __RING_H__

#include "InteractiveRegularPolygon.h"
#include "List.h"
#include "Carbon.h"

extern int *pentagon_cellophane_colors;
extern int pentagon_strong_cellophane_colors[];
extern int pentagon_tint_cellophane_colors[];
extern int pentagon_mono_chrome_cellophane_colors[];

class Bond;
class CarbonAllotrope;

class Ring : public InteractiveRegularPolygon {
  // friend classes & functions

  // members
private:
  int p_number_of_carbons;
  List<Carbon> p_carbons;
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
  Ring(int number_of_carbons, CarbonAllotrope* ca); /* ring only but not carbons/bonds */
  Ring(CarbonAllotrope* ca, int number_of_carbons);
  Ring(CarbonAllotrope* ca, int number_of_carbons, Bond* bond_connection);
  Ring(CarbonAllotrope* ca, int number_of_carbons, Carbon* carbon_connection);
  Ring(CarbonAllotrope* ca, int number_of_carbons, const int* carbon_sequence_nos);
  virtual ~Ring();
  Ring& operator = (const Ring& you); /* dont use */
  void copy_from(const CarbonAllotrope* ca, const Ring* you);

  // distance
public:
  int distance_to_set();

  // determining normal vector
public:
  void reset_clockwise();
  void set_clockwise(Carbon* from, Carbon* to, bool locally = false);
  void set_clockwise_locally();

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
  // attributes
public:
  int number_of_carbons() const { return p_number_of_carbons; }
  virtual Vector3 get_center_location() const;
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
