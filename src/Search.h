/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "Object.h"
#include "MyString.h"
#include "ObjectString.h"
#include "Set.h"
#include "List.h"
#include "Range.h"
#include "Process.h"

class Search : public Object {
  // friend classes & functions

  // members
private:
  List<ObjectString> p_options;
  MyString p_formula_segment;
  MyString p_output_filename;
  Set<Range> p_ranges;
  int p_number;
  int p_outfd;

  // private tools

  // constructors & the destructor
public:
  Search();
  ~Search();

  // scheduling
public:
  AssignProcessResult assign_process(Process* process);
  UnassignProcessResult unassign_process(Process* process);

  // comparators
public:
  int compare(const Search* you) const;

  // I/O
public:
  void print(FILE* output) const;
  bool collect(const char* line, int length);

  // member accessing methods
public:
  void add_option(const char* option);
  bool add_formula_segment(const char* start_formula);
  void add_output_filename(const char* output_filename);
  bool setup();
  const List<ObjectString>& get_options() const { return p_options; }
  const char* get_formula_segment() const { return p_formula_segment; }
  const char* get_output_filename() const { return p_output_filename; }
  double get_progress() const;
  double get_elapsed_sec() const;
  int number_of_ranges() const { return p_ranges.length(); }
  const Range* get_range(int index) const;

};

#endif /* __SEARCH_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
