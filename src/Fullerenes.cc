/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:37:23 JST
 */

#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "Fullerenes.h"
#include "Fullerene.h"
#include "Generator.h"
#include "CarbonAllotrope.h"
#include "BoundaryCarbons.h"
#include "Representations.h"
#include "Debug.h"
#include "DebugMemory.h"

void Fullerenes::p_step_copy_branch(Generator& gen, int maximum_number_of_carbons,
                                    bool symmetric, int close, bool shallow_copy)
{
  Stack<Step> step_stack;
  step_stack.push(new Step(gen, maximum_number_of_carbons, symmetric, close));
  bool need_rollback = false;
  while (1)
    {
      Step* top = step_stack.refer_top();
      if (top == 0)
        return;
      if (shallow_copy && need_rollback)
        top->rollback();
      need_rollback = false;
      top->next_branch();
      Step* target;
      if (top->is_there_next_branch())
        {
          if (shallow_copy)
            target = top->shallow_copy();
          else
            target = top->deep_copy();
          target->link_up();
        }
      else
        {
          target = top;
          target->link_up();
          step_stack.pop();
        }
      Fullerene* fullerene;
      if (target->construction_step(fullerene))
        { /* step is continue */
          step_stack.push(target);
        }
      else if (fullerene)
        { /* step is succeeded */
          add_fullerene(fullerene);
          need_rollback = true;
        }
      else
        { /* step is failed */
          need_rollback = true;
        }
      target->link_down();
    }
}

void Fullerenes::p_step_forward(Generator& gen, int maximum_number_of_carbons,
                                bool symmetric, int close)
{
  Stack<Step> step_stack;
  step_stack.push(new Step(gen, maximum_number_of_carbons, symmetric, close));
  while (1)
    {
      Step* top = step_stack.refer_top();
      if (top == 0)
        return;
      top->next_branch();
      Step* target;
      if (top->is_there_next_branch())
        {
          target = top->deep_copy();
          target->link_up();
        }
      else
        {
          target = top;
          target->link_up();
          step_stack.pop();
        }
      Fullerene* fullerene;
      if (target->construction_step(fullerene))
        { /* step is continue */
          step_stack.push(target);
          target->link_down();
        }
      else if (fullerene)
        { /* step is succeeded */
          add_fullerene(fullerene);
          target->link_down();
        }
      else
        { /* step is failed */
          target->link_down();
        }
    }
}

void Fullerenes::p_step_ei_yah(const char* generator_formula,
                               int maximum_number_of_carbons,
                               bool symmetric, int maximum_vertices_of_polygons,
                               int close, StepAlgorithm step_algorithm)
{
  Generator gen;
  if (generator_formula)
    gen = Generator(generator_formula, maximum_vertices_of_polygons);
  else
    gen = Generator(symmetric, maximum_vertices_of_polygons);
  gen.link_up(); /* delete されないよう保険 */
  switch (step_algorithm)
    {
    case STEP_ALGORITHM_COPY_BRANCH:
      p_step_copy_branch(gen, maximum_number_of_carbons, symmetric, close, false);
      break;
    case STEP_ALGORITHM_FORWARD:
      p_step_forward(gen, maximum_number_of_carbons, symmetric, close);
      break;
    case STEP_ALGORITHM_BACKWARD:
      p_step_copy_branch(gen, maximum_number_of_carbons, symmetric, close, true);
      break;
    }
}

Fullerenes::Fullerenes(const char* generator_formula, int maximum_number_of_carbons,
                       bool symmetric, int maximum_vertices_of_polygons, int close,
                       StepAlgorithm step_algorithm)
{
  Fullerene::s_need_representations = true;
  Interactives::s_need_simulation = false;
  p_step_ei_yah(generator_formula, maximum_number_of_carbons,
                symmetric, maximum_vertices_of_polygons, close, step_algorithm);
  if (symmetric)
    {
      int specified = 0;
      if (generator_formula)
        {
          switch (generator_formula[1])
            {
            case '1':
            default:
              specified = 1;
              break;
            case '2':
              specified = 2;
              break;
            case '3':
              specified = 3;
              break;
            case '4':
              specified = 4;
              break;
            }
        }
      else
        specified = 1;
      switch (specified)
        {
        case 1:
          p_step_ei_yah("S2-", maximum_number_of_carbons, symmetric,
                        maximum_vertices_of_polygons, close, step_algorithm);
          /* continue to next case */
        case 2:
          p_step_ei_yah("S3-", maximum_number_of_carbons, symmetric,
                        maximum_vertices_of_polygons, close, step_algorithm);
          /* continue to next case */
        case 3:
          p_step_ei_yah("S4-", maximum_number_of_carbons, symmetric,
                        maximum_vertices_of_polygons, close, step_algorithm);
          /* continue to next case */
        case 4:
        default:
          break;
        }
    }
}

Fullerenes::~Fullerenes()
{
}

void Fullerenes::add_fullerene(Fullerene* pat)
{
  MinimumRepresentation* min_rep = new MinimumRepresentation(pat);
  if (p_patterns.search_else_add(min_rep))
    {
#if defined(DEBUG_ERRORS)
      printf("************************************************\n");
      printf("* NG same pattern = C%d(number of automorphisms=%d) %s\n",
             pat->get_carbon_allotrope()->number_of_carbons(),
             pat->get_representations()->number_of_automorphisms(),
             pat->get_generator_formula());
      printf("************************************************\n");
#endif
      delete min_rep;
    }
  else
    {
#if defined(DEBUG_ERRORS)
      printf("************************************************\n");
      printf("* OK different pattern = C%d (number of automorphisms=%d) %s\n",
             pat->get_carbon_allotrope()->number_of_carbons(),
             pat->get_representations()->number_of_automorphisms(),
             pat->get_generator_formula());
      printf("************************************************\n");
#else
      printf("C%d (NoA=%d) %s\n",
             pat->get_carbon_allotrope()->number_of_carbons(),
             pat->get_representations()->number_of_automorphisms(),
             pat->get_generator_formula());
#endif
    }
  pat->p_carbon_allotrope = 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
