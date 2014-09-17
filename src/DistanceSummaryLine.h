/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/06 15:42:58 JST
 */

#ifndef __DISTANCESUMMARYLINE_H__
#define __DISTANCESUMMARYLINE_H__

#include "Object.h"

class DistanceSummaryLine : public Object {
  // friend classes & functions

  // members
private:
  static int s_next_sequence;
private:
  char* p_line;

  // private tools
private:
  static bool p_get_number(const char*& ptr, int& number);

  // constructors & the destructor
public:
  DistanceSummaryLine(const char* line);
  ~DistanceSummaryLine();

  // type converters

  // comparators
public:
  int compare(const DistanceSummaryLine* you) const;

  // math operators

  // I/O
public:
  void print() const;

  // class decision

  // member accessing methods

};

#endif /* __DISTANCESUMMARYLINE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
