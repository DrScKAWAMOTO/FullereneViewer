/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/19 21:44:19 JST
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

  // type converters

  // comparators

  // math operators

  // I/O
public:
  void print_out() const;

  // class decision

  // member accessing methods
public:
  int number_of_automorphisms() const { return p_automorphisms.length(); }
  Automorphism* get_automorphism(int index) const;

};

#endif /* __AUTOMORPHISMS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
