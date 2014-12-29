/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include "Object.h"
#include "MyString.h"
#include "ObjectString.h"
#include "ObjectInt.h"
#include "Set.h"
#include "List.h"
#include "Range.h"
#include "Process.h"

class Collector : public Object {
  // friend classes & functions

  // members
private:
  List<ObjectString> p_options;
  List<ObjectString> p_formula_segments;
  MyString p_output_filename;
  Set<Range> p_ranges;
  int p_number;
  int p_outfd;
  List<ObjectInt> p_done_waiters;

  // private tools

  // constructors & the destructor
public:
  Collector();
  ~Collector();

  // scheduling
public:
  AssignProcessResult assign_process(Process* process, bool force = true);

  // comparators
public:
  int compare(const Collector* you) const;

  // I/O
public:
  void print(FILE* output) const;
  bool collect(const char* line, int length);

  // member accessing methods
public:
  void add_option(const char* option);
  bool add_formula_segment(const char* start_formula);
  bool setup_formula_segments();
  void add_output_filename(const char* output_filename);
  bool setup_output_file();
  const List<ObjectString>& get_options() const { return p_options; }
  const List<ObjectString>& get_formula_segments() const { return p_formula_segments; }
  const char* get_output_filename() const { return p_output_filename; }
  double get_progress(bool& done) const;
  double get_elapsed_sec() const;
  void add_range(Range* range);
  int number_of_ranges() const { return p_ranges.length(); }
  const Range* get_range(int index) const;
  void enter_done_waiter(int pid);
  void check_done_and_call_done_waiters();

};

#endif /* __COLLECTOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
