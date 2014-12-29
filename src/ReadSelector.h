/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __READSELECTOR_H__
#define __READSELECTOR_H__

#include <sys/select.h>
#include "ReadHandler.h"
#include "Set.h"

class ReadSelector {
  // friend classes & functions

  // members
protected:
  Set<ReadHandler> p_handlers;
  int p_last_num;
  int p_last_readfd;
  int p_last_nfds;
  fd_set p_last_readfds[FD_SETSIZE];

  // constructors & the destructor
public:
  ReadSelector();
  virtual ~ReadSelector();

  // comparators

  // member accessing methods
public:
  int get_number_of_handlers() const { return p_handlers.length(); }
  virtual bool join(ReadHandler* handler);
  virtual bool defect(ReadHandler* handler);
  virtual void defect_listen_handler();
  virtual ReadHandlerResult select(int timeout);

};

#endif /* __READSELECTOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
