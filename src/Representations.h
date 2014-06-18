/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 22:30:13 JST
 */

#ifndef __REPRESENTATIONS_H__
#define __REPRESENTATIONS_H__

#include "Object.h"
#include "List.h"
#include "Representation.h"

class Representations : public Object {
  // friend classes & functions

  // members
private:
  List<Representation> p_reps;
  int p_number_of_automorphisms;

  // private tools

  // constructors & the destructor
public:
  Representations();
  virtual ~Representations();
  Representations& operator = (const Representations& that); /* dont use */

  // comparators
public:
  bool operator == (const Representation& that) const;
  bool operator == (const Representations& that) const;

  // I/O
public:
  void print() const;

  // member accessing methods
public:
  void add_up_to_isomorphism(Representation* that);
  int length() const { return p_reps.length(); }
  Representation* get_representation(int index);
  int number_of_automorphisms();

};

#endif /* __REPRESENTATIONS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
