/*
 * Project: CarbonAllotrope
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/09/06 08:04:09 JST
 */

#ifndef __BOUNDARY_H__
#define __BOUNDARY_H__

class Boundary : public Object {
  // friend classes & functions

  // members
private:
  List<Carbon> p_carbons;

  // private tools

  // constructors & the destructor
public:
  Boundary(CarbonAllotrope* ca);
  virtual ~Boundary();
  Ring& operator = (const Boundary& that); /* dont use */

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods

};

#endif /* __BOUNDARY_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
