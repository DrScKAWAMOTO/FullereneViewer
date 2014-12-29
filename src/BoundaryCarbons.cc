/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "BoundaryCarbons.h"
#include "CarbonAllotrope.h"

BoundaryCarbons::BoundaryCarbons()
{
}

void BoundaryCarbons::copy_from(const CarbonAllotrope* ca, const BoundaryCarbons& you)
{
  int len = you.p_two_rings_carbons.length();
  for (int i = 0; i < len; ++i)
    add(ca->get_carbon_by_sequence_no(you.p_two_rings_carbons[i]->sequence_no()));
  len = you.p_one_ring_carbons.length();
  for (int i = 0; i < len; ++i)
    add(ca->get_carbon_by_sequence_no(you.p_one_ring_carbons[i]->sequence_no()));
}

BoundaryCarbons::~BoundaryCarbons()
{
}

void BoundaryCarbons::clean()
{
  p_two_rings_carbons.clean();
  p_one_ring_carbons.clean();
}

void BoundaryCarbons::add(Carbon* carbon)
{
  int number_of_rings = carbon->number_of_rings();
  if (number_of_rings == 2)
    (void)p_two_rings_carbons.search_else_add(carbon);
  else if (number_of_rings == 1)
    (void)p_one_ring_carbons.search_else_add(carbon);
}

Carbon* BoundaryCarbons::get_two_rings_carbon_of_minimum_sequence_no() const
{
  if (p_two_rings_carbons.length() == 0)
    return 0;
  return p_two_rings_carbons[0];
}

void BoundaryCarbons::glow_to_one_ring_carbon(Carbon* carbon)
{
  (void)p_one_ring_carbons.search_else_add(carbon);
}

void BoundaryCarbons::glow_to_two_rings_carbon(Carbon* carbon)
{
  p_one_ring_carbons.remove(carbon);
  (void)p_two_rings_carbons.search_else_add(carbon);
}

void BoundaryCarbons::glow_to_three_rings_carbon(Carbon* carbon)
{
  p_two_rings_carbons.remove(carbon);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
