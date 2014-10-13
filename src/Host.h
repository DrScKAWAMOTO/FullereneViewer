/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __HOST_H__
#define __HOST_H__

#include <stdio.h>
#include "Object.h"
#include "MyString.h"

class Host : public Object {
  // friend classes & functions

  // members
private:
  MyString p_host_name;
  int p_number_of_processes;

  // private tools

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
  void add_process(int num);
  bool remove_process(int num);

};

#endif /* __HOST_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
