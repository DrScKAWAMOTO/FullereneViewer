/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/26 18:06:18 JST
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
  const double p_bond_radius;
  const int p_bond_color;
  Carbon* p_left;
  Carbon* p_right;

  // private tools

  // constructors & the destructor
public:
  Bond(CarbonAllotrope* ca);
  virtual ~Bond();
  Bond& operator = (const Bond& that); /* dont use */
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
public:
  Carbon* get_left_carbon() const { return p_left; }
  Carbon* get_right_carbon() const { return p_right; }
  bool connected_with(const Carbon* carbon);
  Carbon* get_carbon_beyond(const Carbon* carbon);

};

#endif /* __BOND_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
