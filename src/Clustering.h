/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CLUSTERING_H__
#define __CLUSTERING_H__

#include <stdio.h>
#include "Object.h"

class CarbonAllotrope;
class Ring;

class Clustering : public Object {
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
  void p_calculate_distances_to_a_ring(CarbonAllotrope* ca, Ring* target);

  // constructors & the destructor
public:
  Clustering();
  Clustering(CarbonAllotrope* ca);
  Clustering(const Clustering& you);
  Clustering(const Clustering& you, int* group, int no);
  ~Clustering();
  Clustering& operator = (const Clustering& you);

  // type converters
public:
  void normalize();

  // comparators

  // I/O
public:
  void print_as_table(FILE* fptr = stdout) const;
  void print_as_line(FILE* fptr = stdout) const;

  // member accessing methods
public:
  void set_distance(int seq_no1, int seq_no2, int distance);
  int get_distance(int seq_no1, int seq_no2);
  int clustering(int threshold, int* group) const;

};

#endif /* __CLUSTERING_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
