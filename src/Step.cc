/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:37:23 JST
 */

#include <assert.h>
#include "Step.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "Debug.h"
#include "DebugMemory.h"

Step::Step()
  : p_symmetric(false), p_close(false), p_is_tube(false),
    p_maximum_number_of_carbons(60),
    p_gen(0), p_ca(0), p_boundary(), p_number_of_rest_pentagons(0),
    p_all_pentagons(false), p_close_count(0),
    p_ring_next_sequence(0), p_carbon_next_sequence(0), p_bond_next_sequence(0)
{
}

Step::Step(Generator& gen, int maximum_number_of_carbons, bool symmetric, int close)
  : Object(0), p_symmetric(symmetric), p_close(close), p_is_tube(false),
    p_maximum_number_of_carbons(maximum_number_of_carbons), p_gen(&gen),
    p_ca(new CarbonAllotrope()), p_boundary(), p_number_of_rest_pentagons(0),
    p_all_pentagons(true), p_close_count(close),
    p_ring_next_sequence(0), p_carbon_next_sequence(0), p_bond_next_sequence(0)
{
  p_gen->link_up();
  p_ca->link_up();
  if (p_gen->type() == GENERATOR_TYPE_TUBE)
    {
      p_ca->make_equator_by_chiral_characteristic(p_gen->n(), p_gen->m(), p_gen->h());
      p_is_tube = true;
    }
  else
    p_ca->make_symmetric_scrap(p_gen->scrap_no());
  p_ca->set_clockwise(+1);
  if (!p_symmetric)
    {
      p_boundary.clean();
      p_ca->list_oldest_connected_boundary_carbons(p_boundary);
      p_all_pentagons = true;
      if (p_is_tube)
        p_number_of_rest_pentagons = p_gen->n() + p_gen->m();
    }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  p_ca->print_detail();
#endif
}

Step* Step::p_copy(CarbonAllotrope* ca) const
{
  Step* result = new Step();
  result->p_symmetric = p_symmetric;
  result->p_close = p_close;
  result->p_is_tube = p_is_tube;
  result->p_maximum_number_of_carbons = p_maximum_number_of_carbons;
  result->p_gen = new Generator(*p_gen);
  result->p_gen->link_up();
  result->p_ca = ca;
  result->p_ca->link_up();
  result->p_boundary.copy_from(result->p_ca, p_boundary);
  result->p_number_of_rest_pentagons = p_number_of_rest_pentagons;
  result->p_all_pentagons = p_all_pentagons;
  result->p_close_count = p_close_count;
  result->p_ring_next_sequence = p_ring_next_sequence;
  result->p_carbon_next_sequence = p_carbon_next_sequence;
  result->p_bond_next_sequence = p_bond_next_sequence;
  return result;
}

Step* Step::shallow_copy() const
{
  return p_copy(p_ca);
}

Step* Step::deep_copy() const
{
  return p_copy(p_ca->copy());
}

Step::~Step()
{
  if (p_gen)
    p_gen->link_down();
  if (p_ca)
    p_ca->link_down();
}

bool Step::construction_step(Fullerene*& fullerene)
{
  int No = p_gen->glow_step();
  int num;
  ErrorCode result;
  p_ca->set_clockwise(+1);
  if (p_symmetric)
    result =
      p_ca->fill_n_polygons_around_carbons_closed_to_center_and_pentagons(No, num);
  else
    {
      if (No == 6)
        p_number_of_rest_pentagons--;
      else
        p_all_pentagons = false;
      Carbon* carbon = p_boundary.get_two_rings_carbon_of_minimum_sequence_no();
      assert(carbon);
      result = p_ca->fill_n_polygon_around_carbon(No, carbon, p_boundary);
    }
  p_ca->set_clockwise(+1);
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  p_ca->print_detail();
#endif
  int number_of_carbons = p_ca->number_of_carbons();
  if (p_symmetric)
    {
      p_boundary.clean();
      p_ca->list_oldest_connected_boundary_carbons(p_boundary);
    }
  int number_of_carbons_in_boundary = p_boundary.length();
  if (!p_symmetric && (number_of_carbons_in_boundary == 0))
    {
      if (--p_close_count > 0)
        {
          p_boundary.clean();
          p_ca->list_oldest_connected_boundary_carbons(p_boundary);
          number_of_carbons_in_boundary = p_boundary.length();
          p_all_pentagons = true;
          if (p_is_tube)
            p_number_of_rest_pentagons = p_gen->n() + p_gen->m();
        }
    }
  if ((result != ERROR_CODE_OK) ||
      (number_of_carbons > p_maximum_number_of_carbons) ||
      (p_is_tube && p_all_pentagons && (p_number_of_rest_pentagons <= 0)) ||
      (number_of_carbons_in_boundary == 0))
    {
      char buffer[1024];
#if defined(DEBUG_ERRORS)
      printf("************************************************\n");
#endif
      p_gen->get_generator_formula(buffer, 1024);
      if (result != ERROR_CODE_OK)
        {
#if defined(DEBUG_ERRORS) || defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("* ERROR %s ", buffer);
          error_handler(result);
#endif
#if defined(DEBUG_ERRORS)
          printf("************************************************\n");
#endif
          fullerene = 0;
          return false;
        }
      else if (number_of_carbons > p_maximum_number_of_carbons)
        {
#if defined(DEBUG_ERRORS) || defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("* ERROR %s Too many carbons %d\n", buffer, number_of_carbons);
#endif
#if defined(DEBUG_ERRORS)
          printf("************************************************\n");
#endif
          fullerene = 0;
          return false;
        }
      else if (p_is_tube && p_all_pentagons && (p_number_of_rest_pentagons <= 0))
        {
#if defined(DEBUG_ERRORS) || defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("* ERROR %s Tube height is enlarged\n", buffer);
#endif
#if defined(DEBUG_ERRORS)
          printf("************************************************\n");
#endif
          fullerene = 0;
          return false;
        }
      else if (number_of_carbons_in_boundary > 0)
        {
#if defined(DEBUG_ERRORS) || defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("* ERROR %s Unknown error\n", buffer);
#endif
#if defined(DEBUG_ERRORS)
          printf("************************************************\n");
#endif
          fullerene = 0;
          return false;
        }
      else
        {
#if defined(DEBUG_ERRORS) || defined(DEBUG_CONSTRUCTION_ALGORITHM)
          printf("* OK    %s generated carbons %d\n", buffer, number_of_carbons);
#endif
#if defined(DEBUG_ERRORS)
          printf("************************************************\n");
#endif
          fullerene = new Fullerene();
          fullerene->set_carbon_allotrope(p_ca);
          fullerene->set_generator_formula(buffer);
          return false;
        }
    }
  return true;
}

bool Step::is_there_next_branch() const
{
  return p_gen->is_there_next_branch();
}

void Step::next_branch()
{
  p_ring_next_sequence = p_ca->ring_next_sequence;
  p_carbon_next_sequence = p_ca->carbon_next_sequence;
  p_bond_next_sequence = p_ca->bond_next_sequence;
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
  printf("@@@ Step::next_branch() = %d, %d, %d\n",
         p_ring_next_sequence, p_carbon_next_sequence, p_bond_next_sequence);
#endif
  p_gen->next_branch();
}

void Step::rollback()
{
#if defined(DEBUG_CONSTRUCTION_ALGORITHM)
  printf("@@@ Step::rollback() = %d, %d, %d\n",
         p_ring_next_sequence, p_carbon_next_sequence, p_bond_next_sequence);
#endif
  p_ca->rollback(p_ring_next_sequence, p_carbon_next_sequence, p_bond_next_sequence);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
