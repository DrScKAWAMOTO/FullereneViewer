/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/09/11 21:52:51 JST
 */

#ifndef __REFLECTIONPAIR_H__
#define __REFLECTIONPAIR_H__

#include "Object.h"

class Fullerene;

class ReflectionPair : public Object {
  // friend classes & functions

  // members
private:
  bool p_symmetric;
  const char* p_my_array;
  const char* p_my_generator_formula;
  const char* p_your_array;
  const char* p_your_generator_formula;

  // private tools

  // constructors & the destructor
public:
  ReflectionPair(const char* generator_formula, const Fullerene* fullerene);
  virtual ~ReflectionPair();
  ReflectionPair& operator = (const ReflectionPair& you); /* dont use */

  // type converters

  // comparators
public:
  int compare(const ReflectionPair* you) const;

  // math operators

  // I/O

  // class decision

  // member accessing methods
  void merge(const ReflectionPair* with);
  bool symmetric() const { return p_symmetric; }
  const char* ones_generator_formula() { return p_my_generator_formula; }
  const char* the_others_generator_formula() { return p_your_generator_formula; }

};

#endif /* __REFLECTIONPAIR_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
