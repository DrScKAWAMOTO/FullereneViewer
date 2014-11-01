/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PDDGFLINES_H__
#define __PDDGFLINES_H__

#include "Set.h"
#include "List.h"
#include "MinimumRepresentation.h"
#include "ReflectionPair.h"
#include "StringPair.h"
#include "PddGfLine.h"

enum PddGfLineResult {
  PDD_GF_LINE_RESULT_OK = 0,
  PDD_GF_LINE_RESULT_NOT_SORTED = -1,
  PDD_GF_LINE_RESULT_ILLEGAL_FULLERENE = -2,
};

class PddGfLines {
  // friend classes & functions

  // members
private:
  bool p_with_pdd;
  Set<MinimumRepresentation> p_uniqness;
  Set<ReflectionPair> p_pairs_by_ref;
  Set<StringPair> p_pairs_by_gf;
  List<PddGfLine> p_lines;
  int p_last_number_of_carbons;
  int p_last_number_of_automorphisms;

  // private tools
private:
  void p_refresh();

  // constructors & the destructor
public:
  PddGfLines(bool with_pdd);
  ~PddGfLines();

  // comparators

  // I/O
public:
  void print() const;

  // member accessing methods
public:
  PddGfLineResult add_PddGfLine_and_refresh(const char* generator_formula);
  void refresh_force();

};

#endif /* __PDDGFLINES_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
