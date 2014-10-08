/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PATTERN_H__
#define __PATTERN_H__

class Pattern {
  // friend classes & functions

  // members
  int p_length;
  int p_offset;
  int* p_pattern;

  // private tools

  // constructors & the destructor
public:
  Pattern(int length, int* values);
  Pattern(int value0);
  Pattern(int value0, int value1);
  Pattern(int value0, int value1, int value2);
  Pattern(int value0, int value1, int value2, int value3);
  Pattern(int value0, int value1, int value2, int value3, int value4);
  Pattern(int value0, int value1, int value2, int value3, int value4, int value5);
  ~Pattern();

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  int get_value_then_next();

};

#endif /* __PATTERN_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
