/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <assert.h>
#include "PddGfLines.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "Representations.h"
#include "Fullerene.h"

void PddGfLines::p_refresh()
{
  int len = p_lines.length();
  if (len > 0)
    {
      PddGfLine* line = p_lines[0];
      for (int i = 0; i < len; ++i)
        {
          line = p_lines[i];
          if (p_with_pdd == false)
            {
              printf("C%d (NoA=%d) %s\n",
                     line->get_number_of_carbons(),
                     line->get_number_of_automorphisms(),
                     line->get_generator_formula());
            }
          else if (line->get_mirror_symmetric())
            {
              printf("C%d (NoA=%d,MirrorSymmetric,Pdd=%s) %s\n",
                     line->get_number_of_carbons(),
                     line->get_number_of_automorphisms(),
                     line->get_characteristic(),
                     line->get_generator_formula());
            }
          else
            {
              const char* gf = line->get_generator_formula();
              StringPair work(gf, gf);
              StringPair* sp = p_pairs_by_gf.search(&work);
              if (sp)
                {
                  printf("C%d (NoA=%d,Mirror=%s,Pdd=%s) %s\n",
                         line->get_number_of_carbons(),
                         line->get_number_of_automorphisms(),
                         sp->your_string(),
                         line->get_characteristic(),
                         line->get_generator_formula());
                }
              else
                {
                  printf("C%d (NoA=%d,Pdd=%s) %s\n",
                         line->get_number_of_carbons(),
                         line->get_number_of_automorphisms(),
                         line->get_characteristic(),
                         line->get_generator_formula());
                }
            }
        }
    }
  p_uniqness.clean();
  if (p_with_pdd)
    {
      p_pairs_by_ref.clean();
      p_pairs_by_gf.clean();
    }
  p_lines.clean();
}

PddGfLines::PddGfLines(bool with_pdd)
  : p_with_pdd(with_pdd),
    p_last_number_of_carbons(0), p_last_number_of_automorphisms(0)
{
}

PddGfLines::~PddGfLines()
{
  assert(p_lines.length() == 0);
}

PddGfLineResult PddGfLines::add_PddGfLine_and_refresh(const char* generator_formula)
{
  Fullerene* fullerene = new Fullerene(generator_formula);
  if (fullerene->error_code() != ERROR_CODE_OK)
    {
      delete fullerene;
      return PDD_GF_LINE_RESULT_ILLEGAL_FULLERENE;
    }
  int new_n_o_carbons;
  int new_n_o_automorphisms;
  new_n_o_carbons = fullerene->get_carbon_allotrope()->number_of_carbons();
  new_n_o_automorphisms = fullerene->get_representations()->number_of_automorphisms();
  if ((p_last_number_of_carbons > new_n_o_carbons) ||
      ((p_last_number_of_carbons == new_n_o_carbons) &&
       (new_n_o_automorphisms > p_last_number_of_automorphisms)))
    {
      delete fullerene;
      return PDD_GF_LINE_RESULT_NOT_SORTED;
    }
  if ((new_n_o_carbons > p_last_number_of_carbons) ||
      ((new_n_o_carbons == p_last_number_of_carbons) &&
       (p_last_number_of_automorphisms > new_n_o_automorphisms)))
   p_refresh();
  p_last_number_of_carbons = new_n_o_carbons;
  p_last_number_of_automorphisms = new_n_o_automorphisms;
  MinimumRepresentation* minrep = new MinimumRepresentation(fullerene);
  if (p_uniqness.search_else_add(minrep))
    {
      delete fullerene;
      delete minrep;
      return PDD_GF_LINE_RESULT_OK;
    }
  PddGfLine* line = new PddGfLine(fullerene, p_with_pdd);
  if (p_with_pdd)
    {
      ReflectionPair* refpair = new ReflectionPair(generator_formula, fullerene);
      if (refpair->symmetric())
        {
          delete refpair;
          line->set_mirror_symmetric();
        }
      else
        {
          ReflectionPair* that = p_pairs_by_ref.search_else_add(refpair);
          if (that)
            {
              that->merge(refpair);
              delete refpair;
              StringPair* strpair;
              strpair = new StringPair(that->ones_generator_formula(),
                                       that->the_others_generator_formula());
              StringPair* searched_pair = p_pairs_by_gf.search_else_add(strpair);
              assert(searched_pair == 0);
              strpair = new StringPair(that->the_others_generator_formula(),
                                       that->ones_generator_formula());
              searched_pair = p_pairs_by_gf.search_else_add(strpair);
              assert(searched_pair == 0);
              bool result = p_pairs_by_ref.remove(that);
              assert(result == true);
            }
        }
    }
  p_lines.add(line);
  delete fullerene;
  return PDD_GF_LINE_RESULT_OK;
}

void PddGfLines::refresh_force()
{
  p_refresh();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
