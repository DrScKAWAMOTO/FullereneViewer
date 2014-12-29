/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __ACCEPTHANDLER_H__
#define __ACCEPTHANDLER_H__

#include <stdio.h>
#include "ReadHandler.h"
#include "MyString.h"

#define BUFFER_SIZE 1024
#define BUFFER2_SIZE 100

class Parallel;
class Collector;
class Process;

enum AcceptStage {
  ACCEPT_STAGE_RECV_COMMAND = 1,
  ACCEPT_STAGE_EXIT_COMMAND = 2,
  ACCEPT_STAGE_LIST_COMMAND = 3,
  ACCEPT_STAGE_ADD_COMMAND = 4,
  ACCEPT_STAGE_ENABLE_COMMAND = 5,
  ACCEPT_STAGE_REMOVE_COMMAND = 6,
  ACCEPT_STAGE_WAIT_COMMAND = 7,
  ACCEPT_STAGE_LIST_HOSTS = 8,
  ACCEPT_STAGE_LIST_PROCESSES = 9,
  ACCEPT_STAGE_LIST_COLLECTORS = 10,
  ACCEPT_STAGE_LIST_RANGES = 11,
  ACCEPT_STAGE_ADD_HOST = 12,
  ACCEPT_STAGE_ADD_COLLECTOR = 13,
  ACCEPT_STAGE_ADD_HOST_NUMBER = 14,
  ACCEPT_STAGE_REMOVE_COLLECTOR = 15,
  ACCEPT_STAGE_ENABLE_PROCESS = 16,
  ACCEPT_STAGE_ENABLE_PROCESS_NUMBER = 17,
  ACCEPT_STAGE_WAIT_HOST = 18,
  ACCEPT_STAGE_WAIT_HOST_ENABLED = 19,
  ACCEPT_STAGE_WAIT_HOST_ENABLED_NUMBER = 20,
  ACCEPT_STAGE_WAIT_COLLECTOR = 21,
  ACCEPT_STAGE_WAIT_COLLECTOR_DONE = 22,
  ACCEPT_STAGE_WAIT_COLLECTOR_DONE_OUT = 23,

};

class AcceptHandler : public ReadHandler {
  // friend classes & functions

  // class methods
public:
  static void call_callback(int pid, const MyString& message);

  // members
private:
  Parallel* p_manager;
  int p_pid;
  FILE* p_stdinput;
  int p_writefd;
  FILE* p_stdoutput;
  char p_buffer[BUFFER_SIZE];
  char p_buffer2[BUFFER2_SIZE];
  AcceptStage p_accept_stage;
  bool p_ca_parallel_is_waiting;
  Collector* p_collector;

  // private tools
private:
  bool p_receive();
  void p_get_output_filename(MyString& output_filename);

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
