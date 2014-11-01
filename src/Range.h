/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __RANGE_H__
#define __RANGE_H__

#include <stdio.h>
#include "Object.h"
#include "MyString.h"

class Collector;
class Process;

enum RangeState {
  RANGE_STATE_COLLECT_DONE = 1,
  RANGE_STATE_PROCESS_ASSIGNED = 2,
  RANGE_STATE_PROCESS_UNASSIGNED = 3
};

class Range : public Object {
  // friend classes & functions

  // members
private:
  Collector* p_collector;
  MyString p_start_formula;
  MyString p_generator_formula;
  MyString p_last_formula;
  MyString p_progress_formula;
  MyString p_ending_formula;
  MyString p_check_formula;
  int p_level;
  RangeState p_range_state;
  Process* p_assigned_process;
  double p_occupation_rate;
  double p_progress;
  double p_elapsed_sec;

  // private tools
private:
  void p_decide_generator_check_and_level();

  // constructors & the destructor
public:
  Range(Collector* collector, const char* start_formula, const char* ending_formula);
  ~Range();

  // scheduling
public:
  void assign_process(Process* process);
  void unassign_process();
  Range* divide();

  // comparators
public:
  int compare(const Range* you) const;

  // I/O
public:
  void print(FILE* output, bool verbose = true) const;

  // member accessing methods
public:
  Collector* get_collector() const { return p_collector; }
  const char* get_start_formula() const { return p_start_formula; }
  const char* get_generator_formula() const { return p_generator_formula; }
  bool set_last_formula(const char* buffer);
  const char* get_last_formula() const { return p_last_formula; }
  bool set_progress_formula(const char* progress_formula);
  const char* get_progress_formula() const { return p_progress_formula; }
  const char* get_ending_formula() const { return p_ending_formula; }
  const char* get_check_formula() const { return p_check_formula; }
  int get_level() const { return p_level; }
  RangeState get_range_state() const { return p_range_state; }
  const Process* get_assigned_process() const { return p_assigned_process; }
  Process* get_assigned_process() { return p_assigned_process; }
  void set_occupation_rate(double rate) { p_occupation_rate = rate; }
  double get_occupation_rate() const { return p_occupation_rate; }
  double get_progress() const { return p_progress; }
  void set_elapsed_sec(double sec) { p_elapsed_sec = sec; }
  double get_elapsed_sec() const { return p_elapsed_sec; }

};

#endif /* __RANGE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
