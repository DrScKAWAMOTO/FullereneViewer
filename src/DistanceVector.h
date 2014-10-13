/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __DISTANCEVECTOR_H__
#define __DISTANCEVECTOR_H__

#include "Object.h"

class DistanceVector : public Object {
  // friend classes & functions

  // members
private:
  int p_distances[12];

  // private tools

  // constructors & the destructor
public:
  DistanceVector();
  DistanceVector(const DistanceVector& you);
  ~DistanceVector();
  DistanceVector& operator = (const DistanceVector& you);

  // type converters

  // comparators
public:
  int compare(const DistanceVector* you) const;
  bool operator == (const DistanceVector& you) const
  { return (compare(&you) == 0); }
  // math operators

  // I/O
public:
  void print() const;

  // class decision

  // member accessing methods
public:
  void append(int distance);
  int operator [] (int index) const;

};

#endif /* __DISTANCEVECTOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
