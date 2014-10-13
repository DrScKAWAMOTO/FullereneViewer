/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __SCHEDULEHANDLER_H__
#define __SCHEDULEHANDLER_H__

#include "ReadHandler.h"

#define SCHEDULE_INTERVAL 30

class ScheduleHandler : public ReadHandler {
  // friend classes & functions

  // members
private:
  int p_writefd;

  // private tools
private:
  bool p_receive();

  // constructors & the destructor
public:
  ScheduleHandler();
  virtual ~ScheduleHandler();

  // comparators

  // member accessing methods
protected:
  virtual ReadHandlerResult call();
public:
  int get_writefd() const { return p_writefd; }

};

#endif /* __SCHEDULEHANDLER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
