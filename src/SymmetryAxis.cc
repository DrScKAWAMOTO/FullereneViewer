/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/23 00:16:18 JST
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
  if (p_type == AXIS_TYPE_CENTER_OF_RING)
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

bool SymmetryAxis::operator <= (const SymmetryAxis& that) const
{
  if (get_type() != that.get_type())
    return false;
  if ((get_north_sequence_no() == that.get_north_sequence_no()) &&
      (get_south_sequence_no() == that.get_south_sequence_no()) ||
      (get_north_sequence_no() == that.get_south_sequence_no()) &&
      (get_south_sequence_no() == that.get_north_sequence_no()))
    {
      int div = that.get_order() / get_order();
      assert(get_order() * div == that.get_order());
      return true;
    }
  else
    return false;
}

bool SymmetryAxis::operator >= (const SymmetryAxis& that) const
{
  return that <= (*this);
}

bool SymmetryAxis::operator == (const SymmetryAxis& that) const
{
  if (get_type() != that.get_type())
    return false;
  if ((get_north_sequence_no() == that.get_north_sequence_no()) &&
      (get_south_sequence_no() == that.get_south_sequence_no()) ||
      (get_north_sequence_no() == that.get_south_sequence_no()) &&
      (get_south_sequence_no() == that.get_north_sequence_no()))
    return (get_order() == that.get_order());
  else
    return false;
}

bool SymmetryAxis::operator != (const SymmetryAxis& that) const
{
  return !((*this) == that);
}

void SymmetryAxis::print_out() const
{
  switch (p_type)
    {
    case AXIS_TYPE_CENTER_OF_RING:
      printf("Ring Axis order=%d at Ring(%d) and Ring(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_BOND:
      printf("Bond Axis order=%d at Bond(%d) and Bond(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_CARBON:
      printf("Carbon Axis order=%d at Carbon(%d) and Carbon(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_BOND:
      printf("Ring-Bond Axis order=%d at Ring(%d) and Bond(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    case AXIS_TYPE_CENTER_OF_RING_AND_CARBON:
      printf("Ring-Carbon Axis order=%d at Ring(%d) and carbon(%d)",
             p_order, p_north_sequence_no, p_south_sequence_no);
      break;
    default:
      assert(0);
    }
  if (p_type == AXIS_TYPE_CENTER_OF_RING)
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
