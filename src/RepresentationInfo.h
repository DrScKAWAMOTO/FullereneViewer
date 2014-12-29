/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
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
  RepresentationInfo& operator = (const RepresentationInfo& you); /* dont use */

};

#endif /* __REPRESENTATIONINFO_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

