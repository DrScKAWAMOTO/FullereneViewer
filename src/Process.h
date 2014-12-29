/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdio.h>
#include <unistd.h>
#include "Object.h"
#include "MyString.h"

class Range;
class Host;
class Parallel;
class PipeHandler;

enum ProcessState {
  PROCESS_STATE_RANGE_UNASSIGNED = 1,
  PROCESS_STATE_RANGE_ASSIGNED = 2
};

enum AssignProcessResult {
  /* プロセス割り当て成功 */
  ASSIGN_PROCESS_RESULT_SUCCESS,
  /* 収集中だがプロセス割り当て失敗 */
  ASSIGN_PROCESS_RESULT_FAIL,
  /* 収集完了で、プロセスを割り当てなかった */
  ASSIGN_PROCESS_RESULT_COLLECT_DONE
};

enum UnassignProcessResult {
  /* プロセス解除成功 */
  UNASSIGN_PROCESS_RESULT_SUCCESS,
  /* そのプロセスはどのレンジにも割り当たっていない */
  UNASSIGN_PROCESS_RESULT_FAIL
};

class Process : public Object {
  // friend classes & functions

  // members
private:
  Host* p_host;
  int p_process_id;
  Parallel* p_manager;
  MyString p_command_line;
  PipeHandler* p_pipe_handler;
  ProcessState p_process_state;
  bool p_disabled;
  Range* p_assigned_range;
  pid_t p_forkpid;
  int p_readfd;
  int p_writefd;

  // private tools
private:
  void p_kill_process();

  // constructors & the destructor
public:
  Process(Host* host, int process_id, Parallel* manager);
  ~Process();

  // scheduling
  bool assign_range(Range* range);
  void unassign_range();
  void disable();
  void enable();

  // comparators
public:
  int compare(const Process* you) const;

  // I/O
public:
  void print(FILE* output) const;

  // member accessing methods
public:
  const Host* get_host() const { return p_host; }
  Range* get_assigned_range() { return p_assigned_range; }
  int process_id() const { return p_process_id; }
  ProcessState get_process_state() const { return p_process_state; }
  bool is_disabled() const { return p_disabled; }
  pid_t get_forkpid() const { return p_forkpid; }
  int get_readfd() const { return p_readfd; }
  void send_level_command(int level);
  void send_exit_command();

};

#endif /* __PROCESS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
