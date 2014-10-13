/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __ACCEPTHANDLER_H__
#define __ACCEPTHANDLER_H__

#include <stdio.h>
#include "ReadHandler.h"

#define BUFFER_SIZE 1024
#define BUFFER2_SIZE 100

class Parallel;
class Search;
class Process;

enum AcceptStage {
  ACCEPT_STAGE_RECV_COMMAND = 1,
  ACCEPT_STAGE_LIST_COMMAND = 2,
  ACCEPT_STAGE_ADD_COMMAND = 3,
  ACCEPT_STAGE_REMOVE_COMMAND = 4,
  ACCEPT_STAGE_LIST_HOSTS = 5,
  ACCEPT_STAGE_LIST_PROCESSES = 6,
  ACCEPT_STAGE_LIST_SEARCHES = 7,
  ACCEPT_STAGE_LIST_RANGES = 8,
  ACCEPT_STAGE_ADD_HOST = 9,
  ACCEPT_STAGE_ADD_PROCESS = 10,
  ACCEPT_STAGE_ADD_SEARCH = 11,
  ACCEPT_STAGE_REMOVE_HOST = 12,
  ACCEPT_STAGE_REMOVE_PROCESS = 13,
  ACCEPT_STAGE_REMOVE_SEARCH = 14,
  ACCEPT_STAGE_ADD_HOST_NUMBER = 15,
  ACCEPT_STAGE_ADD_PROCESS_NUMBER = 16,
  ACCEPT_STAGE_REMOVE_PROCESS_NUMBER = 17,

};

class AcceptHandler : public ReadHandler {
  // friend classes & functions

  // members
private:
  Parallel* p_manager;
  FILE* p_stdinput;
  int p_writefd;
  FILE* p_stdoutput;
  char p_buffer[BUFFER_SIZE];
  char p_buffer2[BUFFER2_SIZE];
  AcceptStage p_accept_stage;
  Search* p_search;
  bool p_formula_set;

  // private tools
private:
  bool p_receive();
  void p_list_hosts() const;
  bool p_list_ranges(int index, int indent = 0) const;
  void p_list_process(const Process* process) const;
  bool p_list_processes(int index) const;
  void p_list_processes() const;
  void p_list_searches(bool ranges_too = false) const;

  // constructors & the destructor
public:
  AcceptHandler(Parallel* manager, int pid);
  virtual ~AcceptHandler();

  // comparators

  // member accessing methods
protected:
  virtual ReadHandlerResult call();

};

#endif /* __ACCEPTHANDLER_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
