/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __REFLECTIONPAIR_H__
#define __REFLECTIONPAIR_H__

#include "Object.h"
#include "MyString.h"

class Fullerene;

class ReflectionPair : public Object {
  // friend classes & functions

  // members
private:
  bool p_symmetric;
  MyString p_my_repres;
  MyString p_my_generator_formula;
  MyString p_your_repres;
  MyString p_your_generator_formula;

  // private tools

  // constructors & the destructor
public:
  ReflectionPair(const char* generator_formula, const Fullerene* fullerene);
  virtual ~ReflectionPair();
  ReflectionPair& operator = (const ReflectionPair& you); /* dont use */

  // comparators
public:
  int compare(const ReflectionPair* you) const;

  // I/O

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
