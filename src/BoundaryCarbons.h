/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __BOUNDARYCARBONS_H__
#define __BOUNDARYCARBONS_H__

#include "Set.h"
#include "Carbon.h"

class BoundaryCarbons {
  // friend classes & functions

  // members
private:
  Set<Carbon> p_two_rings_carbons;
  Set<Carbon> p_one_ring_carbons;

  // private tools

  // constructors & the destructor
public:
  BoundaryCarbons();
  void copy_from(const CarbonAllotrope* ca, const BoundaryCarbons& you);
  ~BoundaryCarbons();
  BoundaryCarbons& operator = (const BoundaryCarbons& you); /* dont use */

  // comparators

  // I/O

  // member accessing methods
  // attributes
public:
  int length() const
  { return p_one_ring_carbons.length() + p_two_rings_carbons.length(); }
  void clean();
  void add(Carbon* carbon);
  int number_of_two_rings_carbons() const { return p_two_rings_carbons.length(); }
  Carbon* get_two_rings_carbon_of_minimum_sequence_no() const;
  void glow_to_one_ring_carbon(Carbon* carbon);
  void glow_to_two_rings_carbon(Carbon* carbon);
  void glow_to_three_rings_carbon(Carbon* carbon);

};

#endif /* __BOUNDARYCARBONS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
