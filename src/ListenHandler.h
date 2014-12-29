/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __LISTENHANDLER_H__
#define __LISTENHANDLER_H__

#include "ReadHandler.h"

class Parallel;

class ListenHandler : public ReadHandler {
  // friend classes & functions

  // members
private:
  Parallel* p_manager;

  // constructors & the destructor
public:
  ListenHandler(Parallel* manager);
  virtual ~ListenHandler();

  // comparators

  // member accessing methods
public:
  virtual bool is_listen_handler() const { return true; }
protected:
  virtual ReadHandlerResult call();

};

#endif /* __LISTENHANDLER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
