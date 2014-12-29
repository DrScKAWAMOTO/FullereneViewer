/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __READHANDLER_H__
#define __READHANDLER_H__

#include "Object.h"

enum ReadHandlerResult {
  READ_HANDLER_RESULT_CONTINUED = 0,
  READ_HANDLER_RESULT_TERMINATE = -1,
  READ_HANDLER_RESULT_SCHEDULE = -2,
  READ_HANDLER_RESULT_SCHEDULE_FORCE = -3,
  READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE = -4,
  READ_HANDLER_RESULT_TIMEOUT = -5,
  READ_HANDLER_RESULT_EXIT = -6,
};

class ReadHandler : public Object {
  // friend classes & functions
  friend class ReadSelector;

  // members
protected:
  int p_readfd;

  // constructors & the destructor
public:
  ReadHandler();
  virtual ~ReadHandler();

  // comparators
public:
  int compare(const ReadHandler* you) const;

  // member accessing methods
public:
  virtual bool is_listen_handler() const { return false; }
protected:
  virtual ReadHandlerResult call();

};

#endif /* __READHANDLER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
