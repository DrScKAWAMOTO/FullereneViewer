/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <stdio.h>
#include "Representations.h"

Representations::Representations()
  : p_number_of_automorphisms(0)
{
}

Representations::~Representations()
{
}

int Representations::compare(const Representations* you) const
{
  assert(p_reps.length() > 0);
  assert(you->p_reps.length() > 0);
  return p_reps[0]->compare(you->p_reps[0]);
}

void Representations::print() const
{
  int len = p_reps.length();
  for (int i = 0; i < len; ++i)
    p_reps[i]->print();
}

void Representations::add_up_to_isomorphism(Representation* you)
{
  Representation* rep = p_reps.search_else_add(you);
  if (rep)
    {
      rep->add_info(you->get_info(0));
      delete you;
    }
}

Representation* Representations::get_representation(int index)
{
  assert((index >= 0) && (index < p_reps.length()));
  return p_reps[index];
}

const Representation* Representations::get_representation(int index) const
{
  assert((index >= 0) && (index < p_reps.length()));
  return p_reps[index];
}

int Representations::number_of_automorphisms()
{
  if (p_number_of_automorphisms == 0)
    {
      int len = p_reps.length();
      for (int i = 0; i < len; ++i)
        {
          Representation* rep = get_representation(i);
          int number_of_infos = rep->number_of_infos();
          if (p_number_of_automorphisms > 0)
            assert(p_number_of_automorphisms == number_of_infos);
          else
            p_number_of_automorphisms = number_of_infos;
        }
    }
  return p_number_of_automorphisms;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
