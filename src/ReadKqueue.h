/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __READKQUEUE_H__
#define __READKQUEUE_H__

#include "ReadSelector.h"

class ReadKqueue : public ReadSelector {
  // friend classes & functions

  // members

  // private tools

  // constructors & the destructor
public:
  ReadKqueue();
  ~ReadKqueue();

  // member accessing methods
public:
  virtual bool join(ReadHandler* handler);
  virtual bool defect(ReadHandler* handler);
  virtual ReadHandlerResult select(int timeout);

};

#endif /* __READKQUEUE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
