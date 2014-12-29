/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __HOST_H__
#define __HOST_H__

#include <stdio.h>
#include "Object.h"
#include "MyString.h"
#include "List.h"
#include "ObjectInt2.h"

class Host : public Object {
  // friend classes & functions

  // members
private:
  MyString p_host_name;
  int p_number_of_processes;
  int p_number_of_enabled_processes;
  List<ObjectInt2> p_enable_waiters;

  // private tools
private:
  void p_search_and_call_enable_waiters(int num);

  // constructors & the destructor
public:
  Host(const char* host_name, int number_of_processes);
  ~Host();

  // comparators
public:
  int compare(const Host* you) const;

  // I/O
public:
  void print(FILE* output) const;

  // member accessing methods
public:
  const char* get_host_name() const { return p_host_name; }
  int number_of_processes() const { return p_number_of_processes; }
  int number_of_enabled_processes() const { return p_number_of_enabled_processes; }
  bool enable_processes(int num);
  void enter_enabled_waiter(int num, int pid);

};

#endif /* __HOST_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
