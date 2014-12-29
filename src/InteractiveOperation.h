/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
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

