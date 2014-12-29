/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __SYMMETRYAXIS_H__
#define __SYMMETRYAXIS_H__

#include "Object.h"

class Automorphism;

enum AxisType {
  AXIS_TYPE_CENTER_OF_TWO_CARBONS = 1,
  AXIS_TYPE_CENTER_OF_CARBON_AND_BOND = 2,
  AXIS_TYPE_CENTER_OF_CARBON_AND_RING = 3,
  AXIS_TYPE_CENTER_OF_CARBON_AND_BOUNDARY = 4,
  AXIS_TYPE_CENTER_OF_TWO_BONDS = 5,
  AXIS_TYPE_CENTER_OF_BOND_AND_RING = 6,
  AXIS_TYPE_CENTER_OF_BOND_AND_BOUNDARY = 7,
  AXIS_TYPE_CENTER_OF_TWO_RINGS = 8,
  AXIS_TYPE_CENTER_OF_RING_AND_BOUNDARY = 9,
  AXIS_TYPE_CENTER_OF_TWO_BOUNDARIES = 10,
  AXIS_TYPE_CENTER_OF_ONLY_ONE_CARBON = 11,
  AXIS_TYPE_CENTER_OF_ONLY_ONE_BOND = 12,
  AXIS_TYPE_CENTER_OF_ONLY_ONE_RING = 13,
  AXIS_TYPE_CENTER_OF_ONLY_ONE_BOUNDARY = 14,
};

class SymmetryAxis : public Object {
  // friend classes & functions

  // members
private:
  AxisType p_type;
  int p_order;
  int p_north_sequence_no;
  int p_south_sequence_no;
  Automorphism* p_generator;
  int p_generator_step;

  // private tools

  // constructors & the destructor
public:
  SymmetryAxis(AxisType type, int order, int north_sequence_no,
               int south_sequence_no, Automorphism* generator);
  virtual ~SymmetryAxis();

  // type converters

  // comparators
public:
  bool operator <= (const SymmetryAxis& you) const;
  bool operator >= (const SymmetryAxis& you) const;
  bool operator == (const SymmetryAxis& you) const;
  bool operator != (const SymmetryAxis& you) const;

  // math operators

  // I/O
public:
  void print_out() const;

  // class decision

  // member accessing methods
public:
  AxisType get_type() const { return p_type; }
  int get_order() const { return p_order; }
  void set_order(int order) { p_order = order; }
  int get_north_sequence_no() const { return p_north_sequence_no; }
  int get_south_sequence_no() const { return p_south_sequence_no; }
  Automorphism* get_generator() const { return p_generator; }
  void set_generator(Automorphism* generator, int generator_step);
  int get_generator_step() const { return p_generator_step; }

};

#endif /* __SYMMETRYAXIS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
