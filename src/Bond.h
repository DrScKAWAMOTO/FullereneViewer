/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __BOND_H__
#define __BOND_H__

#include <stdio.h>
#include "InteractiveRegularPolygon.h"

class Carbon;
class CarbonAllotrope;

class Bond : public InteractiveRegularPolygon {
  // friend classes & functions

  // members
private:
  double p_bond_radius;
  int p_bond_color;
  Carbon* p_left;
  Carbon* p_right;

  // private tools

  // constructors & the destructor
public:
  Bond(CarbonAllotrope* ca);
  virtual ~Bond();
  Bond& operator = (const Bond& you); /* dont use */
  void copy_from(const CarbonAllotrope* ca, const Bond* you);
  bool connect_to(Carbon* carbon);
  void remove_carbon(Carbon* carbon);

  // interactions

  // I/O
public:
  void print_structure(int indent = 0, bool deep = true) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr) const;
  void print_POVRay_scene_description(const CarbonAllotrope* ca, FILE* fptr,
                                      const Matrix3& rot, const Vector3& move,
                                      bool clipped_by_Z_non_negative) const;
  virtual void draw_opaque_by_OpenGL(bool selection) const;

  // member accessing methods
  // attributes
public:
  virtual Vector3 get_center_location() const;
  Carbon* get_left_carbon() const { return p_left; }
  Carbon* get_right_carbon() const { return p_right; }
  bool connected_with(const Carbon* carbon);
  Carbon* get_carbon_beyond(const Carbon* carbon);

};

#endif /* __BOND_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
