/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <assert.h>
#include "ReflectionPair.h"
#include "Representations.h"
#include "Fullerene.h"
#include "DebugMemory.h"
#include "Utils.h"

ReflectionPair::ReflectionPair(const char* generator_formula, const Fullerene* fullerene)
  : p_symmetric(true)
{
  const Representations* reps = fullerene->get_representations();
  assert(reps);
  int len = reps->length();
  assert(len > 0);
  const Representation* rep1 = reps->get_representation(0);
  assert(rep1);
  const Representation* rep2 = reps->get_representation(len / 2);
  assert(rep2);
  const char* array1 = (const char*)*rep1;
  assert(array1);
  const char* array2 = (const char*)*rep2;
  assert(array2);
  int result = strcmp(array1, array2);
  if (result < 0)
    {
      p_symmetric = false;
      p_my_repres = array1;
      p_my_generator_formula = generator_formula;
      p_your_repres = array2;
      p_your_generator_formula = "";
    }
  else if (result > 0)
    {
      p_symmetric = false;
      p_my_repres = array2;
      p_my_generator_formula = "";
      p_your_repres = array1;
      p_your_generator_formula = generator_formula;
    }
}

ReflectionPair::~ReflectionPair()
{
}

int ReflectionPair::compare(const ReflectionPair* you) const
{
  return strcmp((char*)p_my_repres, (char*)you->p_my_repres);
}

void ReflectionPair::merge(const ReflectionPair* with)
{
  assert(!p_symmetric);
  assert(!with->p_symmetric);
  assert(strcmp((char*)p_my_repres, (char*)with->p_my_repres) == 0);
  assert(strcmp((char*)p_your_repres, (char*)with->p_your_repres) == 0);
  if ((p_my_generator_formula.length() == 0) &&
      (with->p_my_generator_formula.length() > 0))
    p_my_generator_formula = with->p_my_generator_formula;
  else if ((p_your_generator_formula.length() == 0) &&
           (with->p_your_generator_formula.length() > 0))
    p_your_generator_formula = with->p_your_generator_formula;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
