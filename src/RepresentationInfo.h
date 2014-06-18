/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 18:11:15 JST
 */

#ifndef __REPRESENTATIONINFO_H__
#define __REPRESENTATIONINFO_H__

#include "Object.h"

struct RepresentationInfo : public Object {
  // members
public:
  int clockwise;
  int carbon_sequence_no;
  int bond_sequence_no;

  // constructors & the destructor
public:
  RepresentationInfo();
  virtual ~RepresentationInfo();
  RepresentationInfo& operator = (const RepresentationInfo& that); /* dont use */

};

#endif /* __REPRESENTATIONINFO_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

