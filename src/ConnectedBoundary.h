/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CONNECTEDBOUNDARY_H__
#define __CONNECTEDBOUNDARY_H__

#include "Object.h"
#include "List.h"
#include "Carbon.h"

class CarbonAllotrope;

class ConnectedBoundary : public Object {
  // friend classes & functions

  // members
private:
  List<Carbon> p_carbons;

  // private tools

  // constructors & the destructor
public:
  ConnectedBoundary(CarbonAllotrope* ca, const List<Carbon>& carbon_list);
  ~ConnectedBoundary();
  ConnectedBoundary& operator = (const ConnectedBoundary& you); /* dont use */

  // comparators

  // I/O

  // member accessing methods
  // attributes
public:
  int length() const { return p_carbons.length(); }
  Carbon* operator[](int offset) const { return p_carbons[offset]; }
  bool contained(Carbon* carbon) const;
  Vector3 get_center_location() const;

};

#endif /* __CONNECTEDBOUNDARY_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
