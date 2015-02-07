/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */ 

#ifndef __SORTBYZ_H__
#define __SORTBYZ_H__

#include "Object.h"
#include "Interactive.h"

class SortByZ : public Object {
  // friend classes & functions

  // members
private:
  double p_z;
  Interactive* p_target;
  
  // private tools

  // constructors & the destructor
public:
  SortByZ(Interactive* target, const Matrix3& mosv);
  ~SortByZ();

  // comparators
public:
  int compare(const SortByZ* you) const;
  
  // I/O

  // member accessing methods
public:
  Interactive* get_target() { return p_target; }

};

#endif /* __SORTBYZ_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
