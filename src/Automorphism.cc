/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/19 21:13:28 JST
 */

#include <assert.h>
#include "CarbonAllotrope.h"
#include "Automorphism.h"
#include "DebugMemory.h"

Automorphism::Automorphism(CarbonAllotrope* ca)
  : p_ca(ca), p_index(0), p_order(-1),
    p_carbon_map(0), p_bond_map(0), p_ring_map(0), p_boundary_map(0)
{
  int len = p_ca->number_of_carbons();
  p_carbon_map = new int[len];
}

Automorphism::Automorphism(CarbonAllotrope* ca, Bond* from_bond, Carbon* from_carbon,
                           Bond* to_bond, Carbon* to_carbon)
  : p_ca(ca), p_index(0), p_order(-1),
    p_carbon_map(0), p_bond_map(0), p_ring_map(0), p_boundary_map(0)
{
  int len = p_ca->number_of_carbons();
  p_carbon_map = new int[len];
  p_ca->set_clockwise(+1);
  p_ca->reset_done();
  from_carbon->write_automorphism(*this, from_bond);
  int* from_map = p_carbon_map;
  p_carbon_map = new int[len];
  p_index = 0;
  p_ca->reset_done();
  to_carbon->write_automorphism(*this, to_bond);
  int* to_map = p_carbon_map;
  p_carbon_map = new int[len];
  for (int i = 0; i < len; ++i)
    p_carbon_map[from_map[i] - 1] = to_map[i] - 1;
  delete[] from_map;
  delete[] to_map;
  len = p_ca->number_of_bonds();
  p_bond_map = new int[len];
  for (int i = 1; i <= len; ++i)
    {
      Bond* bond = p_ca->get_bond_by_sequence_no(i);
      Carbon* carbon0 = bond->get_left_carbon();
      int carbon_index0 = operator () (carbon0->sequence_no());
      carbon0 = p_ca->get_carbon_by_sequence_no(carbon_index0);
      Carbon* carbon1 = bond->get_right_carbon();
      int carbon_index1 = operator () (carbon1->sequence_no());
      carbon1 = p_ca->get_carbon_by_sequence_no(carbon_index1);
      bond = carbon0->bond_between(carbon1);
      p_bond_map[i - 1] = bond->sequence_no() - 1;
    }
  len = p_ca->number_of_rings();
  p_ring_map = new int[len];
  for (int i = 1; i <= len; ++i)
    {
      Ring* ring = p_ca->get_ring_by_sequence_no(i);
      assert(ring->number_of_carbons() >= 3);
      Carbon* carbon0 = ring->get_carbon(0);
      int carbon_index0 = operator () (carbon0->sequence_no());
      carbon0 = p_ca->get_carbon_by_sequence_no(carbon_index0);
      Carbon* carbon1 = ring->get_carbon(1);
      int carbon_index1 = operator () (carbon1->sequence_no());
      carbon1 = p_ca->get_carbon_by_sequence_no(carbon_index1);
      Carbon* carbon2 = ring->get_carbon(2);
      int carbon_index2 = operator () (carbon2->sequence_no());
      carbon2 = p_ca->get_carbon_by_sequence_no(carbon_index2);
      Bond* bond01 = carbon0->bond_between(carbon1);
      Bond* bond12 = carbon2->bond_between(carbon1);
      ring = carbon1->ring_between(bond01, bond12);
      p_ring_map[i - 1] = ring->sequence_no() - 1;
    }
  len = p_ca->number_of_boundaries();
  p_boundary_map = new int[len];
  for (int i = 1; i <= len; ++i)
    {
      ConnectedBoundary* boundary = p_ca->get_boundary_by_sequence_no(i);
      Carbon* carbon = (*boundary)[0];
      int carbon_index = operator () (carbon->sequence_no());
      carbon = p_ca->get_carbon_by_sequence_no(carbon_index);
      for (int j = 1; j <= len; ++j)
        {
          ConnectedBoundary* boundary2 = p_ca->get_boundary_by_sequence_no(j);
          if (boundary2->contained(carbon))
            {
              p_boundary_map[i - 1] = j - 1;
              break;
            }
        }
    }
}

void Automorphism::initialize(int from_seq, int to_seq)
{
  assert((from_seq >= 1) && (from_seq <= number_of_carbons()));
  assert((to_seq >= 1) && (to_seq <= number_of_carbons()));
  p_carbon_map[from_seq - 1] = to_seq - 1;
}

Automorphism::~Automorphism()
{
  if (p_carbon_map)
    delete[] p_carbon_map;
  if (p_bond_map)
    delete[] p_bond_map;
  if (p_ring_map)
    delete[] p_ring_map;
  if (p_boundary_map)
    delete[] p_boundary_map;
}

bool operator == (const Automorphism& one, const Automorphism& the_other)
{
  assert(one.number_of_carbons() == the_other.number_of_carbons());
  int len = one.number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      if (one(i) != the_other(i))
        return false;
    }
  return true;
}

Automorphism* Automorphism::composition(const Automorphism* you) const
{
  assert(get_carbon_allotrope() == you->get_carbon_allotrope());
  int len = number_of_carbons();
  Automorphism* result = new Automorphism(get_carbon_allotrope());
  for (int i = 0; i < len; ++i)
    result->initialize(i + 1, (*this)((*you)(i + 1)));
  return result;
}

Automorphism* Automorphism::inverse() const
{
  int len = number_of_carbons();
  Automorphism* result = new Automorphism(get_carbon_allotrope());
  for (int i = 0; i < len; ++i)
    result->initialize((*this)(i + 1), i + 1);
  return result;
}

bool Automorphism::is_unit() const
{
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    if ((*this)(i + 1) != i + 1)
      return false;
  return true;
}

void Automorphism::print_out()
{
  if (order() == 1)
    {
      printf("order = 1\n");
      return;
    }
  printf("order = %d, ", order());
  int seq0, seq1;
  int num = fixed_carbons(seq0, seq1);
  assert((num >= 0) && (num <= 2));
  if (num == 2)
    printf("fixed carbons = %d,%d, ", seq0, seq1);
  else if (num == 1)
    printf("fixed carbon = %d, ", seq0);
  num = fixed_bonds(seq0, seq1);
  assert((num >= 0) && (num <= 2));
  if (num == 2)
    printf("fixed bonds = %d,%d, ", seq0, seq1);
  else if (num == 1)
    printf("fixed bond = %d, ", seq0);
  num = fixed_rings(seq0, seq1);
  assert((num >= 0) && (num <= 2));
  if (num == 2)
    printf("fixed rings = %d,%d, ", seq0, seq1);
  else if (num == 1)
    printf("fixed ring = %d, ", seq0);
  printf("\n");
}

void Automorphism::print_orbit(Carbon* carbon)
{
  Carbon* work = carbon;
  printf("C%d", work->sequence_no());
  while (1)
    {
      int next = operator () (work->sequence_no());
      work = p_ca->get_carbon_by_sequence_no(next);
      printf("->C%d", work->sequence_no());
      if (work == carbon)
        break;
    }
  printf("\n");
}

int Automorphism::number_of_carbons() const
{
  return p_ca->number_of_carbons();
}

int Automorphism::number_of_bonds() const
{
  return p_ca->number_of_bonds();
}

int Automorphism::number_of_rings() const
{
  return p_ca->number_of_rings();
}

int Automorphism::number_of_boundaries() const
{
  return p_ca->number_of_boundaries();
}

int Automorphism::operator () (int sequence_no) const
{
  assert((sequence_no >= 1) && (sequence_no <= number_of_carbons()));
  return p_carbon_map[sequence_no - 1] + 1;
}

int Automorphism::carbon_map(int sequence_no) const
{
  assert((sequence_no >= 1) && (sequence_no <= number_of_carbons()));
  return p_carbon_map[sequence_no - 1] + 1;
}

int Automorphism::bond_map(int sequence_no) const
{
  assert((sequence_no >= 1) && (sequence_no <= number_of_bonds()));
  return p_bond_map[sequence_no - 1] + 1;
}

int Automorphism::ring_map(int sequence_no) const
{
  assert((sequence_no >= 1) && (sequence_no <= number_of_rings()));
  return p_ring_map[sequence_no - 1] + 1;
}

int Automorphism::boundary_map(int sequence_no) const
{
  assert((sequence_no >= 1) && (sequence_no <= number_of_boundaries()));
  return p_boundary_map[sequence_no - 1] + 1;
}

void Automorphism::set_step(int sequence_no)
{
  p_carbon_map[p_index++] = sequence_no;
}

int Automorphism::order()
{
  if (p_order != -1)
    return p_order;
  if (is_unit())
    {
      p_order = 1;
      return p_order;
    }
  p_order = 2;
  Automorphism* seki = composition(this);
  while (1)
    {
      if (seki->is_unit())
        {
          delete seki;
          return p_order;
        }
      Automorphism* seki2 = composition(seki);
      delete seki;
      seki = seki2;
      ++p_order;
    }
}

int Automorphism::fixed_carbons(int& sequence_no0, int& sequence_no1)
{
  int num = 0;
  int len = p_ca->number_of_carbons();
  for (int i = 1; i <= len; ++i)
    {
      if (i == carbon_map(i))
        {
          if (num == 0)
            sequence_no0 = i;
          else if (num == 1)
            sequence_no1 = i;
          num++;
        }
    }
  return num;
}

int Automorphism::fixed_bonds(int& sequence_no0, int& sequence_no1)
{
  int num = 0;
  int len = p_ca->number_of_bonds();
  for (int i = 1; i <= len; ++i)
    {
      if (i == bond_map(i))
        {
          if (num == 0)
            sequence_no0 = i;
          else if (num == 1)
            sequence_no1 = i;
          num++;
        }
    }
  return num;
}

int Automorphism::fixed_rings(int& sequence_no0, int& sequence_no1)
{
  int num = 0;
  int len = p_ca->number_of_rings();
  for (int i = 1; i <= len; ++i)
    {
      if (i == ring_map(i))
        {
          if (num == 0)
            sequence_no0 = i;
          else if (num == 1)
            sequence_no1 = i;
          num++;
        }
    }
  return num;
}

int Automorphism::fixed_boundaries(int& sequence_no0, int& sequence_no1)
{
  int num = 0;
  int len = p_ca->number_of_boundaries();
  for (int i = 1; i <= len; ++i)
    {
      if (i == boundary_map(i))
        {
          if (num == 0)
            sequence_no0 = i;
          else if (num == 1)
            sequence_no1 = i;
          num++;
        }
    }
  return num;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
