/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/16 17:07:43 JST
 */

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "Object.h"

enum GeneratorType {
  GENERATOR_TYPE_SYMMETRIC = 1,
  GENERATOR_TYPE_TUBE = 2,
  GENERATOR_TYPE_ORDINARY = 3,
  GENERATOR_TYPE_ILLEGAL = 4,
};

enum GeneratorState {
  GENERATOR_STATE_START_SPECIFIED = 1,
  GENERATOR_STATE_START_FROM_MINIMUM = 2,
  GENERATOR_STATE_RUNNING = 3,
};

class Generator : public Object {
  // friend classes & functions

  // members
private:
  GeneratorType p_type;
  int p_scrap_no;
  int p_n;
  int p_m;
  int p_h;
  int p_minimum_polygons;
  int p_maximum_vertices_of_polygons;
  GeneratorState p_state;
  int p_array_length;
  int p_history_length;
  int p_history_offset;
  char* p_history;

  // private tools
private:
  void p_enlarge();
  void p_glow(int value);

  // constructors & the destructor
public:
  Generator(bool symmetric = true, int maximum_vertices_of_polygons = 6);
  Generator(int n, int m, int h, int maximum_vertices_of_polygons = 6);
  Generator(const char* generator_formula, int maximum_vertices_of_polygons = 6);
  Generator(const Generator& you);
  Generator& operator = (const Generator& you);
  ~Generator();

  // I/O
public:
  void print_detail() const;
  void print_progress(int length) const;

  // member accessing methods
public:
  GeneratorType type() const { return p_type; }
  int scrap_no() const { return p_scrap_no; }
  int n() const { return p_n; }
  int m() const { return p_m; }
  int h() const { return p_h; }
  int glow_step();
  int history();
  bool is_there_next_branch() const;
  void next_branch();
  bool next_tree();
  bool next_by_rollback();
  void initialize_next_sequences();
  void get_generator_formula(char* buffer, int length) const;

};

#endif /* __GENERATOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
