/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PIPEHANDLER_H__
#define __PIPEHANDLER_H__

#include "ReadHandler.h"

#define BUFFER_SIZE 1024

class Process;
class Range;
class Collector;

class PipeHandler : public ReadHandler {
  // friend classes & functions

  // members
private:
  char p_buffer[BUFFER_SIZE];
  int p_data_len;
  int p_line_top;
  int p_line_len;
  Process* p_process;
  Range* p_assigned_range;
  Collector* p_assigned_collector;

  // private tools
private:
  bool p_receive();
  bool p_read_line();

  // constructors & the destructor
public:
  PipeHandler(Process* process);
  virtual ~PipeHandler();

  // comparators

  // member accessing methods
protected:
  virtual ReadHandlerResult call();

};

#endif /* __PIPEHANDLER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
