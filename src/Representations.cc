/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 22:30:13 JST
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

bool Representations::operator == (const Representation& that) const
{
  return (that == p_reps);
}

bool Representations::operator == (const Representations& that) const
{
  assert(p_reps.length() > 0);
  return that == (*(p_reps[0]));
}

void Representations::print() const
{
  int len = p_reps.length();
  for (int i = 0; i < len; ++i)
    p_reps[i]->print();
}

void Representations::add_up_to_isomorphism(Representation* that)
{
  int len = p_reps.length();
  for (int i = 0; i < len; ++i)
    {
      Representation* rep = p_reps[i];
      if ((*rep) == (*that))
        {
          rep->add_info(that->get_info(0));
          delete that;
          return;
        }
    }
  p_reps.add(that);
}

Representation* Representations::get_representation(int index)
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
          if ((p_number_of_automorphisms > 0) &&
              (p_number_of_automorphisms != number_of_infos))
            {
              printf("Warning:%s:%d: p_number_of_automorphisms != number_of_infos\n",
                     __FILE__, __LINE__);
            }
          if ((p_number_of_automorphisms == 0) ||
              (p_number_of_automorphisms > number_of_infos))
            p_number_of_automorphisms = number_of_infos;
        }
    }
  return p_number_of_automorphisms;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
