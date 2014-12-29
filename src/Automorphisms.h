/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __AUTOMORPHISMS_H__
#define __AUTOMORPHISMS_H__

#include "List.h"
#include "Automorphism.h"

class Fullerene;

class Automorphisms {
  // friend classes & functions

  // members
private:
  List<Automorphism> p_automorphisms;

  // private tools

  // constructors & the destructor
public:
  Automorphisms(Fullerene* fullerene);
  ~Automorphisms();

  // comparators

  // I/O
public:
  void print_out() const;

  // member accessing methods
public:
  int number_of_automorphisms() const { return p_automorphisms.length(); }
  Automorphism* get_automorphism(int index) const;

};

#endif /* __AUTOMORPHISMS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
