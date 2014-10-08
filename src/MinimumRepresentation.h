/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __MINIMUMREPRESENTATION_H__
#define __MINIMUMREPRESENTATION_H__

#include "Object.h"

class Fullerene;

class MinimumRepresentation : public Object {
  // friend classes & functions

  // members
private:
  char* p_array;

  // private tools

  // constructors & the destructor
public:
  MinimumRepresentation(const Fullerene* fullerene);
  virtual ~MinimumRepresentation();
  MinimumRepresentation& operator = (const MinimumRepresentation& you); /* dont use */

  // comparators
public:
  int compare(const MinimumRepresentation* you) const;

  // I/O
public:
  void print() const;

  // member accessing methods

};

#endif /* __REPRESENTATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/







