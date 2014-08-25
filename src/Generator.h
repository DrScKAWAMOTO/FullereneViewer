/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/16 17:07:43 JST
 */

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

class Generator {
  // friend classes & functions

  // members
private:
  bool p_is_tube;
  int p_scrap_no;
  bool p_symmetric;
  int p_n;
  int p_m;
  int p_h;
  const int p_minimum_polygons;
  int p_maximum_vertices_of_polygons;
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
  ~Generator();

  // I/O

  // member accessing methods
public:
  bool next_pattern();
  int scrap_no() const { return p_scrap_no; }
  bool symmetric() const { return p_symmetric; }
  bool is_tube() const { return p_is_tube; }
  int n() const { return p_n; }
  int m() const { return p_m; }
  int h() const { return p_h; }
  int glow();
  void get_generator_formula(char* buffer, int length);

};

#endif /* __GENERATOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
