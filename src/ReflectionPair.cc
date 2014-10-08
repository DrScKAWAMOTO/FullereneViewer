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
  : p_symmetric(true), p_my_array(0), p_my_generator_formula(0),
    p_your_array(0), p_your_generator_formula(0)
{
  const Representations* reps = fullerene->get_representations();
  assert(reps);
  int len = reps->length();
  assert(len > 0);
  const Representation* rep1 = reps->get_representation(0);
  assert(rep1);
  const Representation* rep2 = reps->get_representation(len / 2);
  assert(rep2);
  const char* array1 = rep1->get_array();
  assert(array1);
  const char* array2 = rep2->get_array();
  assert(array2);
  int result = strcmp(array1, array2);
  if (result < 0)
    {
      p_symmetric = false;
      p_my_array = copy_string(array1);
      p_my_generator_formula = copy_string(generator_formula);
      p_your_array = copy_string(array2);
    }
  else if (result > 0)
    {
      p_symmetric = false;
      p_my_array = copy_string(array2);
      p_my_generator_formula = 0;
      p_your_array = copy_string(array1);
      p_your_generator_formula = copy_string(generator_formula);
    }
}

ReflectionPair::~ReflectionPair()
{
  if (p_my_array)
    delete[] p_my_array;
  if (p_my_generator_formula)
    delete[] p_my_generator_formula;
  if (p_your_array)
    delete[] p_your_array;
  if (p_your_generator_formula)
    delete[] p_your_generator_formula;
}

int ReflectionPair::compare(const ReflectionPair* you) const
{
  return strcmp(p_my_array, you->p_my_array);
}

void ReflectionPair::merge(const ReflectionPair* with)
{
  assert(!p_symmetric);
  assert(!with->p_symmetric);
  assert(strcmp(p_my_array, with->p_my_array) == 0);
  assert(strcmp(p_your_array, with->p_your_array) == 0);
  if (!p_my_generator_formula && with->p_my_generator_formula)
    p_my_generator_formula = copy_string(with->p_my_generator_formula);
  else if (!p_your_generator_formula && with->p_your_generator_formula)
    p_your_generator_formula = copy_string(with->p_your_generator_formula);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
