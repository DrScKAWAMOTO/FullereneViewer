/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PDDGFLINE_H__
#define __PDDGFLINE_H__

#include "Object.h"
#include "MyString.h"

class Fullerene;

class PddGfLine : public Object {
  // friend classes & functions

  // members
private:
  int p_number_of_carbons;
  int p_number_of_automorphisms;
  bool p_mirror_symmetric;
  MyString p_characteristic;
  MyString p_generator_formula;

  // private tools

  // constructors & the destructor
public:
  PddGfLine(Fullerene* fullerene, bool with_pdd);
  ~PddGfLine();

  // comparators

  // I/O

  // member accessing methods
public:
  int get_number_of_carbons() const { return p_number_of_carbons; }
  int get_number_of_automorphisms() const { return p_number_of_automorphisms; }
  void set_mirror_symmetric() { p_mirror_symmetric = true; }
  bool get_mirror_symmetric() const { return p_mirror_symmetric; }
  const char* get_characteristic() const { return (char*)p_characteristic; }
  const char* get_generator_formula() const { return (char*)p_generator_formula; }

};

#endif /* __PDDGFLINE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
