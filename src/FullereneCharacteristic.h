/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __FULLERENECHARACTERISTIC_H__
#define __FULLERENECHARACTERISTIC_H__

#include <stdio.h>
#include "DistanceVector.h"
#include "Set.h"
#include "Object.h"

class CarbonAllotrope;

class FullereneCharacteristic : public Object {
  // friend classes & functions

  // members
private:
  Set<DistanceVector> p_dvs;

  // private tools

  // constructors & the destructor
public:
  FullereneCharacteristic(CarbonAllotrope* ca);
  ~FullereneCharacteristic();
  FullereneCharacteristic&
  operator = (const FullereneCharacteristic& you); /* dont use */

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

#endif /* __FULLERENECHARACTERISTIC_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
