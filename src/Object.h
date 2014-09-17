/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/10/16 08:46:27 JST
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object {
  // friend classes & functions

  // members
private:
  int p_sequence_no;
private:
  int p_link_counter;

  // private tools

  // constructors & the destructor
public:
  Object(int sequence_no = 0);
  Object(const Object& you);
  virtual ~Object();
  Object& operator = (const Object& you); /* dont use */

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  void link_up() { p_link_counter++; }
  void link_down() { if (--p_link_counter <= 0) delete this; }
  int sequence_no() const { return p_sequence_no; }

};

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
