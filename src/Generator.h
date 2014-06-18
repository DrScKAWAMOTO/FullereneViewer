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
  int p_scrap_no;
  bool p_symmetric;
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
  Generator(const char* generator_label, int maximum_vertices_of_polygons = 6);
  ~Generator();

  // I/O

  // member accessing methods
public:
  bool next_pattern();
  int scrap_no() const { return p_scrap_no; }
  bool symmetric() const { return p_symmetric; }
  int glow();
  void get_generator_label(char* buffer, int length);

};

#endif /* __GENERATOR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
