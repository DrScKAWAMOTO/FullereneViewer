/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 22:30:13 JST
 */

#ifndef __REPRESENTATION_H__
#define __REPRESENTATION_H__

#include "Object.h"
#include "List.h"
#include "RepresentationInfo.h"

class Representation : public Object {
  // friend classes & functions

  // members
private:
  List<RepresentationInfo> p_infos;
  char p_last_char;
  int p_last_count;
  int p_array_length;
  int p_offset;
  char* p_array;

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
  const char* get_array() const { return p_array; }

};

#endif /* __REPRESENTATION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/







