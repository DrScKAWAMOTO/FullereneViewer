/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "PddGfLine.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "Representations.h"
#include "Characteristic.h"

PddGfLine::PddGfLine(Fullerene* fullerene, bool with_pdd)
{
  p_number_of_carbons = fullerene->get_carbon_allotrope()->number_of_carbons();
  p_number_of_automorphisms = fullerene->get_representations()->number_of_automorphisms();
  p_mirror_symmetric = false;
  if (with_pdd)
    p_characteristic = fullerene->get_characteristic()->get_summary();
  p_generator_formula = fullerene->get_generator_formula();
}

PddGfLine::~PddGfLine()
{
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
