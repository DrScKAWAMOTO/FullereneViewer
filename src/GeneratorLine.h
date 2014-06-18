/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/15 10:35:57 JST
 */

#ifndef __GENERATORLINE_H__
#define __GENERATORLINE_H__

#include "Object.h"

class GeneratorLine : public Object {
  // friend classes & functions

  // members
private:
  static int s_next_sequence;
private:
  char* p_line;

  // private tools
private:
  bool p_get_number_and_NoA(int& number, int& NoA) const;

  // constructors & the destructor
public:
  GeneratorLine(const char* line);
  ~GeneratorLine();

  // type converters

  // comparators
public:
  int compare(const GeneratorLine* that) const;

  // math operators

  // I/O
public:
  void print() const;

  // class decision

  // member accessing methods

};

#endif /* __GENERATORLINE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
