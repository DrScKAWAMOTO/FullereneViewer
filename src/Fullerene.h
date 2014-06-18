/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:35:19 JST
 */

#ifndef __FULLERENE_H__
#define __FULLERENE_H__

#include "Object.h"

class CarbonAllotrope;
class Representations;
class FullereneCharacteristic;
class DistanceMatrix;

#define NAME_MAX_SIZE 1024

class Fullerene : public Object {
  // friend classes & functions

  // members
public:
  static bool s_need_representations;
  static bool s_need_fullerene_characteristic;
  static bool s_need_distance_matrix;

private:
  CarbonAllotrope* p_carbon_allotrope;
  char p_fullerene_name[NAME_MAX_SIZE + 1];
  char p_generator_label[NAME_MAX_SIZE + 1];
  int p_n, p_m, p_h;
  Representations* p_representations;
  FullereneCharacteristic* p_characteristic;
  DistanceMatrix* p_distance_matrix;

  // private tools

  // constructors & the destructor
public:
  Fullerene();
  Fullerene(const char* generator_label);
  virtual ~Fullerene();
  Fullerene& operator = (const Fullerene& that); /* dont use */

  // type converters

  // comparators
public:
  bool operator == (const Fullerene& that) const;

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  void set_carbon_allotrope(CarbonAllotrope* carbon_allotrope);
  void set_fullerene_name(const char* fullerene_name);
  void set_generator_label(const char* generator_label);
  CarbonAllotrope* get_carbon_allotrope() { return p_carbon_allotrope; }
  const char* get_fullerene_name() const { return p_fullerene_name; }
  const char* get_generator_label() const { return p_generator_label; }
  Representations* get_representations() { return p_representations; }
  FullereneCharacteristic* get_characteristic() { return p_characteristic; }
  DistanceMatrix* get_distance_matrix() { return p_distance_matrix; }
  int get_n() const { return p_n; }
  int get_m() const { return p_m; }
  int get_h() const { return p_h; }

};

#endif /* __FULLERENE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
