/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __REPRESENTATIONS_H__
#define __REPRESENTATIONS_H__

#include "Object.h"
#include "Set.h"
#include "Representation.h"

class Representations : public Object {
  // friend classes & functions

  // members
private:
  Set<Representation> p_reps;
  int p_number_of_automorphisms;

  // private tools

  // constructors & the destructor
public:
  Representations();
  virtual ~Representations();
  Representations& operator = (const Representations& youd); /* dont use */

  // comparators
public:
  int compare(const Representations* you) const;

  // I/O
public:
  void print() const;

  // member accessing methods
public:
  void add_up_to_isomorphism(Representation* you);
  int length() const { return p_reps.length(); }
  Representation* get_representation(int index);
  const Representation* get_representation(int index) const;
  int number_of_automorphisms();

};

#endif /* __REPRESENTATIONS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
