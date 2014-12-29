/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include "Automorphisms.h"
#include "Representations.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "DebugMemory.h"
#include "Config.h"

Automorphisms::Automorphisms(Fullerene* fullerene)
{
  Representations* reps = fullerene->get_representations();
  reps->number_of_automorphisms();
  const Representation* rep = reps->get_representation(0);
  const RepresentationInfo* from_info = rep->get_info(0);
  assert(from_info->clockwise == 1);
  CarbonAllotrope* ca = fullerene->get_carbon_allotrope();
  Carbon* from_carbon = ca->get_carbon_by_sequence_no(from_info->carbon_sequence_no);
  Bond* from_bond = ca->get_bond_by_sequence_no(from_info->bond_sequence_no);
  int len = rep->number_of_infos();
  for (int i = 0; i < len; ++i)
    {
      const RepresentationInfo* to_info = rep->get_info(i);
      if (to_info->clockwise != 1)
        {
          ca->has_reflection_symmetricity(true);
          continue;
        }
      Carbon* to_carbon = ca->get_carbon_by_sequence_no(to_info->carbon_sequence_no);
      Bond* to_bond = ca->get_bond_by_sequence_no(to_info->bond_sequence_no);
      Automorphism* am = new Automorphism(ca, from_bond, from_carbon, to_bond, to_carbon);
      p_automorphisms.add(am);
    }
}

Automorphisms::~Automorphisms()
{
}

void Automorphisms::print_out() const
{
  int len = p_automorphisms.length();
  for (int i = 0; i < len; ++i)
    {
      Automorphism* am = p_automorphisms[i];
      am->print_out();
    }
}

Automorphism* Automorphisms::get_automorphism(int index) const
{
  assert((index >= 0) && (index < number_of_automorphisms()));
  return p_automorphisms[index];
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
