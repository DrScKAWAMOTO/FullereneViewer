/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 07:28:45 JST
 */

#ifndef __DISTANCEMATRIX_H__
#define __DISTANCEMATRIX_H__

#include <stdio.h>
#include "Object.h"

class CarbonAllotrope;

class DistanceMatrix : public Object {
  // friend classes & functions

  // members
private:
  int p_number;
  int p_distances[12][12];
  int p_sequence_nos[12];

  // private tools
private:
  void p_initialize();
  int p_seq_no_to_index(int seq_no);
  void p_print_as_table(FILE* fptr = stdout) const;
  void p_print_as_line(FILE* fptr = stdout) const;
  void p_sort(int i, int j);
  int p_select_minimum(int top);
  void p_swap(int i1, int i2);

  // constructors & the destructor
public:
  DistanceMatrix();
  DistanceMatrix(CarbonAllotrope* ca);
  DistanceMatrix(const DistanceMatrix& you);
  DistanceMatrix(const DistanceMatrix& you, int* group, int no);
  ~DistanceMatrix();
  DistanceMatrix& operator = (const DistanceMatrix& you);

  // type converters
public:
  void normalize();

  // comparators

  // math operators

  // I/O
public:
  void print_as_table(FILE* fptr = stdout) const;
  void print_as_line(FILE* fptr = stdout) const;

  // class decision

  // member accessing methods
public:
  void set_distance(int seq_no1, int seq_no2, int distance);
  int get_distance(int seq_no1, int seq_no2);
  int grouping(int max_distance, int* group) const;

};

#endif /* __DISTANCEVECTOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
