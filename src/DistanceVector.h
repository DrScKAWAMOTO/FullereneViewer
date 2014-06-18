/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 07:28:45 JST
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
  DistanceVector(const DistanceVector& that);
  ~DistanceVector();
  DistanceVector& operator = (const DistanceVector& that);

  // type converters

  // comparators
public:
  int compare(const DistanceVector* that) const;
  bool operator > (const DistanceVector& that) const
  { return (compare(&that) > 0); }
  bool operator < (const DistanceVector& that) const
  { return (compare(&that) < 0); }
  bool operator <= (const DistanceVector& that) const
  { return (compare(&that) <= 0); }
  bool operator >= (const DistanceVector& that) const
  { return (compare(&that) >= 0); }
  bool operator == (const DistanceVector& that) const
  { return (compare(&that) == 0); }
  bool operator != (const DistanceVector& that) const
  { return (compare(&that) != 0); }

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
