/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __REPRESENTATION_H__
#define __REPRESENTATION_H__

#include "Object.h"
#include "List.h"
#include "RepresentationInfo.h"
#include "MyString.h"

class Representation : public Object {
  // friend classes & functions

  // members
private:
  List<RepresentationInfo> p_infos;
  char p_last_char;
  int p_last_count;
  MyString p_repres;

  // private tools
private:
  void p_flush_step();

  // constructors & the destructor
public:
  Representation();
  virtual ~Representation();
  Representation& operator = (const Representation& you); /* dont use */

  // comparators
public:
  int compare(const Representation* you) const;

  // I/O
public:
  void print() const;

  // member accessing methods
public:
  int number_of_infos() const;
  void add_info(RepresentationInfo* info);
  RepresentationInfo* get_info(int index) const;
  void set_step(char step);
  void finish_step();
  operator const char*() const { return p_repres; }

};

#endif /* __REPRESENTATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/







