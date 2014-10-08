/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include "SymmetryAxis.h"
#include "Automorphism.h"
#include "Ring.h"
#include "CarbonAllotrope.h"

SymmetryAxis::SymmetryAxis(AxisType type, int order,
                           int north_sequence_no, int south_sequence_no,
                           Automorphism* generator)
  : p_type(type), p_order(order), p_north_sequence_no(north_sequence_no),
    p_south_sequence_no(south_sequence_no), p_generator(generator), p_generator_step(0)
{
  if (p_generator)
    p_generator->link_up();
  if (p_type == AXIS_TYPE_CENTER_OF_TWO_RINGS)
    {
      CarbonAllotrope* ca = p_generator->get_carbon_allotrope();
      Ring* ring = ca->get_ring_by_sequence_no(p_north_sequence_no);
      Carbon* carbon0 = ring->get_carbon(0);
      int seq1 = p_generator->carbon_map(carbon0->sequence_no());
      Carbon* carbon1 = ca->get_carbon_by_sequence_no(seq1);
      int len = ring->number_of_carbons();
      for (p_generator_step = 0; p_generator_step < len; ++p_generator_step)
        {
          if (ring->get_carbon(p_generator_step) == carbon1)
            break;
        }
      if (p_generator_step > (len/2))
        p_generator_step = len - p_generator_step;
    }
}

SymmetryAxis::~SymmetryAxis()
{
  if (p_generator)
    p_generator->link_down();
}

bool SymmetryAxis::operator <= (const SymmetryAxis& you) const
{
  if (get_type() != you.get_type())
    return false;
  if (((get_north_sequence_no() == you.get_north_sequence_no()) &&
      (get_south_sequence_no() == you.get_south_sequence_no())) ||
      ((get_north_sequence_no() == you.get_south_sequence_no()) &&
      (get_south_sequence_no() == you.get_north_sequence_no())))
    {
      int div = you.get_order() / get_order();
      assert(get_order() * div == you.get_order());
      return true;
    }
  else
    return false;
}

bool SymmetryAxis::operator >= (const SymmetryAxis& you) const
{
  return you <= (*this);
}

bool SymmetryAxis::operator == (const SymmetryAxis& you) const
{
  if (get_type() != you.get_type())
    return false;
  if (((get_north_sequence_no() == you.get_north_sequence_no()) &&
      (get_south_sequence_no() == you.get_south_sequence_no())) ||
      ((get_north_sequence_no() == you.get_south_sequence_no()) &&
      (get_south_sequence_no() == you.get_north_sequence_no())))
    return (get_order() == you.get_order());
  else
    return false;
}

bool SymmetryAxis::operator != (const SymmetryAxis& you) const
{
  return !((*this) == you);
}

void SymmetryAxis::print_out() const
{
  switch (p_type)
    {
    case AXIS_TYPE_CENTER_OF_TWO_CARBONS:
      printf("Carbon Axis order=%d at Carbon(%d) and Carbon(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_BOND:
      printf("Carbon-Bond Axis order=%d at Carbon(%d) and Bond(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_RING:
      printf("Carbon-Ring Axis order=%d at Carbon(%d) and Ring(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_CARBON_AND_BOUNDARY:
      printf("Carbon-Boundary Axis order=%d at Carbon(%d) and Boundary(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_TWO_BONDS:
      printf("Bond Axis order=%d at Bond(%d) and Bond(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_BOND_AND_RING:
      printf("Bond-Ring Axis order=%d at Bond(%d) and Ring(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_BOND_AND_BOUNDARY:
      printf("Bond-Boundary Axis order=%d at Bond(%d) and Boundary(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_TWO_RINGS:
      printf("Ring Axis order=%d at Ring(%d) and Ring(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_BOUNDARY:
      printf("Ring-Boundary Axis order=%d at Ring(%d) and Boundary(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_TWO_BOUNDARIES:
      printf("Boundary Axis order=%d at Boundary(%d) and Boundary(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_CARBON:
      printf("Carbon Axis order=%d at only one Carbon(%d)",
             p_order, p_north_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_BOND:
      printf("Bond Axis order=%d at only one Bond(%d)",
             p_order, p_north_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_RING:
      printf("Ring Axis order=%d at only one Ring(%d)",
             p_order, p_north_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_ONLY_ONE_BOUNDARY:
      printf("Boundary Axis order=%d at only one Boundary(%d)",
             p_order, p_north_sequence_no);
      break;
    default:
      assert(0);
    }
  if (p_type == AXIS_TYPE_CENTER_OF_TWO_RINGS)
    printf(", generator step=%d", p_generator_step);
  printf("\n");
}

void SymmetryAxis::set_generator(Automorphism* generator, int generator_step)
{
  p_generator = generator;
  if (p_generator)
    p_generator->link_up();
  p_generator_step = generator_step;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
