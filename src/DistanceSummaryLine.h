/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __DISTANCESUMMARYLINE_H__
#define __DISTANCESUMMARYLINE_H__

#include "Object.h"
#include "MyString.h"

class DistanceSummaryLine : public Object {
  // friend classes & functions

  // members
private:
  static int s_next_sequence;
private:
  MyString p_line;

  // private tools
private:
  static bool p_get_number(const char*& ptr, int& number);

  // constructors & the destructor
public:
  DistanceSummaryLine(const char* line);
  ~DistanceSummaryLine();

  // comparators
public:
  int compare(const DistanceSummaryLine* you) const;

  // I/O
public:
  void print() const;

  // member accessing methods

};

#endif /* __DISTANCESUMMARYLINE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
