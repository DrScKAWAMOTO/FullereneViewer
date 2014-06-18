/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:37:23 JST
 */

#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#include "List.h"
#include "Fullerene.h"

class Fullerenes {
  // friend classes & functions

  // members
private:
  List<Fullerene> p_patterns;

  // private tools

  // constructors & the destructor
public:
  Fullerenes(const char* generator_label, int maximum_number_of_carbons,
             bool symmetric = true, int maximum_vertices_of_polygons = 6);
  ~Fullerenes();

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  int number_of_patterns() const;
  const Fullerene* get_pattern(int index) const;
  void add_fullerene(Fullerene* pat);

};

#endif /* __PATTERN_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
