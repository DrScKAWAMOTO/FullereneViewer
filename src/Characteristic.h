/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CHARACTERISTIC_H__
#define __CHARACTERISTIC_H__

#include <stdio.h>
#include "DistanceVector.h"
#include "Set.h"
#include "Object.h"

class CarbonAllotrope;

class Characteristic : public Object {
  // friend classes & functions

  // members
private:
  Set<DistanceVector> p_dvs;

  // private tools

  // constructors & the destructor
public:
  Characteristic(CarbonAllotrope* ca);
  ~Characteristic();
  Characteristic& operator = (const Characteristic& you); /* dont use */

  // type converters

  // comparators

  // math operators

  // I/O
public:
  void print_detail() const;
  const char* get_summary() const;
  void print_summary(FILE* fptr = stdout) const;

  // class decision

  // member accessing methods
public:
  int number_of_distance_vector() const { return p_dvs.length(); }
  void add(DistanceVector* dv) { p_dvs.add(dv); }
  DistanceVector* get_distance_vector(int index);

};

#endif /* __CHARACTERISTIC_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
