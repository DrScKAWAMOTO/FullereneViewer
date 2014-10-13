/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __GENERATORLINE_H__
#define __GENERATORLINE_H__

#include "Object.h"
#include "MyString.h"

class GeneratorLine : public Object {
  // friend classes & functions

  // members
private:
  static int s_next_sequence;
private:
  MyString p_line;

  // private tools
private:
  bool p_get_number_NoA_isS_isT_and_isA(int& number, int& NoA, bool& isS,
                                        bool& isT, bool& isA) const;
  void p_get_n_m_and_h(int& n, int& m, int& h) const;
  bool p_get_scrap(int& scrap) const;

  // constructors & the destructor
public:
  GeneratorLine(const char* line);
  ~GeneratorLine();

  // type converters

  // comparators
public:
  int compare(const GeneratorLine* you) const;

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
