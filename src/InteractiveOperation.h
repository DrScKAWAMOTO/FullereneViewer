/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 18:11:15 JST
 */

#ifndef __INTERACTIVEOPERATION_H__
#define __INTERACTIVEOPERATION_H__

#include "Object.h"

class Interactive;

struct InteractiveOperation : public Object {
  // members
public:
  int force_type;
  Interactive* one;
  int one_index;
  Interactive* the_other;
  int the_other_index;

  // constructors & the destructor
public:
  InteractiveOperation();
  virtual ~InteractiveOperation();
  InteractiveOperation& operator = (const InteractiveOperation& you); /* dont use */

};

#endif /* __INTERACTIVEOPERATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

