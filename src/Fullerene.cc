/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "Config.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "BoundaryCarbons.h"
#include "Representations.h"
#include "Automorphisms.h"
#include "Characteristic.h"
#include "Generator.h"
#include "Clustering.h"
#include "Clusters.h"
#include "Pattern.h"
#include "Utils.h"
#include "Debug.h"
#include "DebugMemory.h"

bool Fullerene::s_need_fullerene_characteristic = false;
bool Fullerene::s_need_clustering = false;
bool Fullerene::s_need_display_clustering = false;

Fullerene::Fullerene()
  : p_carbon_allotrope(0), p_error_code(ERROR_CODE_OK), p_n(0), p_m(0), p_h(0),
    p_representations(0), p_characteristic(0), p_clustering(0)
{
}

Fullerene::Fullerene(const char* generator_formula)
  : p_carbon_allotrope(0), p_error_code(ERROR_CODE_OK), p_n(0), p_m(0), p_h(0),
    p_representations(0), p_characteristic(0), p_clustering(0)
{
  if (generator_formula[0] == 'C')
    {
      while ((generator_formula[0] != ' ') && (generator_formula[0] != '\0'))
        ++generator_formula;
      if (generator_formula[0] == ' ')
        ++generator_formula;
    }
  if (generator_formula[0] == '(')
    {
      while ((generator_formula[0] != ' ') && (generator_formula[0] != '\0'))
        ++generator_formula;
      if (generator_formula[0] == ' ')
        ++generator_formula;
    }
  while ((generator_formula[0] != 'S') &&
         (generator_formula[0] != 'A') &&
         (generator_formula[0] != 'T') &&
         (generator_formula[0] != 'Y') &&
         (generator_formula[0] != '\0'))
    ++generator_formula;
  CarbonAllotrope* ca = new CarbonAllotrope();
  if ((generator_formula[0] != 'S') &&
      (generator_formula[0] != 'A') &&
      (generator_formula[0] != 'T') &&
      (generator_formula[0] != 'Y'))
    {
      p_error_code = ERROR_CODE_ILLEGAL_GENERATOR_FORMULA;
      delete ca;
      ca = 0;
      goto do_nothing;
    }
  p_generator_formula = generator_formula;
  if (strcmp(p_generator_formula, "Y") == 0)
    {
      ca->make_equator_by_chiral_characteristic(6, 6, 5);
      ca->close_normally_once();
      int result_number;
      int array[100];
      array[0] = 7;
      array[1] = 6;
      array[2] = 7;
      array[3] = 6;
      array[4] = 6;
      array[5] = 6;
      array[6] = 7;
      array[7] = 6;
      array[8] = 7;
      array[9] = 6;
      ca->enlarge_cylinder_by_n_polygons(new Pattern(10, array), result_number);
      ca->append_n_polygon_at_carbon(6, 131 + 48);
      ca->append_n_polygon_at_carbon(6, 117 + 48);
      ca->append_n_polygon_at_carbon(6, 132 + 48);
      ca->append_n_polygon_at_carbon(6, 136 + 48);
      ca->append_n_polygon_at_carbon(6, 118 + 48);
      ca->append_n_polygon_at_carbon(6, 123 + 48);
      ca->append_n_polygon_at_carbon(6, 138 + 48);
      ca->append_n_polygon_at_carbon(6, 140 + 48);
      array[0] = 156 + 48;
      array[1] = 155 + 48;
      array[2] = 0;
      array[3] = 0;
      array[4] = 0;
      array[5] = 0;
      array[6] = 0;
      ca->append_n_polygon_at_carbons(7, array);
      array[0] = 154 + 48;
      array[1] = 153 + 48;
      ca->append_n_polygon_at_carbons(7, array);
      array[0] = 160 + 48;
      array[1] = 159 + 48;
      array[2] = 164 + 48;
      array[3] = 163 + 48;
      array[4] = 0;
      array[5] = 0;
      ca->append_n_polygon_at_carbons(6, array);
      ca->append_n_polygon_at_carbon(6, 164 + 48);

      ca->append_n_polygon_at_carbon(6, 121 + 48);
      ca->append_n_polygon_at_carbon(6, 124 + 48);
      ca->append_n_polygon_at_carbon(6, 126 + 48);
      ca->append_n_polygon_at_carbon(6, 129 + 48);
      ca->append_n_polygon_at_carbon(6, 154 + 48);
      ca->append_n_polygon_at_carbon(6, 165 + 48);
      ca->append_n_polygon_at_carbon(6, 158 + 48);
      ca->append_n_polygon_at_carbon(6, 155 + 48);

      ca->append_n_polygon_at_carbon(6, 112 + 48);
      ca->append_n_polygon_at_carbon(6, 115 + 48);
      ca->append_n_polygon_at_carbon(6, 156 + 48);
      ca->append_n_polygon_at_carbon(6, 160 + 48);
      ca->append_n_polygon_at_carbon(6, 163 + 48);
      ca->append_n_polygon_at_carbon(6, 153 + 48);
      ca->append_n_polygon_at_carbon(6, 111 + 48);
      ca->append_n_polygon_at_carbon(6, 109 + 48);

      ca->append_n_polygon_at_carbon(6, 143 + 48);
      ca->append_n_polygon_at_carbon(6, 151 + 48);

      ca->append_n_polygon_at_carbon(6, 149 + 48);
      ca->append_n_polygon_at_carbon(6, 145 + 48);

      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->close_normally_once();

      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
      ca->close_normally_once();
    }
  else
    {
      if (p_generator_formula[0] == 'T')
        {
          const char *ptr = (char*)p_generator_formula + 1;
          p_n = strtol(ptr, (char**)&ptr, 10);
          if (*ptr == ',')
            ++ptr;
          p_m = strtol(ptr, (char**)&ptr, 10);
          if (*ptr == ',')
            ++ptr;
          p_h = strtol(ptr, (char**)&ptr, 10);
          if (*ptr == '\0')
            {
              ca->make_equator_by_chiral_characteristic(p_n, p_m, p_h);
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
              ca->print_detail();
#endif
              goto finish;
            }
        }
      /* 'S' 'A' 'T' */
      Generator gen = Generator(generator_formula, 6);
      if (gen.type() == GENERATOR_TYPE_ILLEGAL)
        {
          delete ca;
          p_error_code = ERROR_CODE_ILLEGAL_GENERATOR_FORMULA;
          ca = 0;
          goto do_nothing;
        }
      if (gen.type() == GENERATOR_TYPE_TUBE)
        ca->make_equator_by_chiral_characteristic(gen.n(), gen.m(), gen.h());
      else
        ca->make_symmetric_scrap(gen.scrap_no());
      BoundaryCarbons boundary;
      bool symmetric = (gen.type() == GENERATOR_TYPE_SYMMETRIC);
      if (!symmetric)
        {
          boundary.clean();
          ca->list_oldest_connected_boundary_carbons(boundary);
        }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
      ca->print_detail();
#endif
      while (1)
        {
          int No = gen.history();
          if (No == -1)
            break;
          int num;
          ErrorCode result;
          if (symmetric)
            result =
              ca->fill_n_polygons_around_carbons_closed_to_center_and_pentagons(No, num);
          else
            {
              Carbon* carbon = boundary.get_two_rings_carbon_of_minimum_sequence_no();
              assert(carbon);
              result = ca->fill_n_polygon_around_carbon(No, carbon, boundary);
            }
          if (result != ERROR_CODE_OK)
            {
              delete ca;
              p_error_code = result;
              ca = 0;
              goto do_nothing;
            }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
          ca->print_detail();
#endif
          if (symmetric)
            {
              boundary.clean();
              ca->list_oldest_connected_boundary_carbons(boundary);
            }
          int number_of_carbons_in_boundary = boundary.length();
          if (!symmetric && (number_of_carbons_in_boundary == 0))
            {
              boundary.clean();
              ca->list_oldest_connected_boundary_carbons(boundary);
            }
        }
    }
 finish:
  set_carbon_allotrope(ca);
 do_nothing:
  ;
}

Fullerene::~Fullerene()
{
  if (p_carbon_allotrope)
    delete p_carbon_allotrope;
  if (p_representations)
    delete p_representations;
  if (p_characteristic)
    delete p_characteristic;
  if (p_clustering)
    delete p_clustering;
}

int Fullerene::compare(const Fullerene* you) const
{
  return p_representations->compare(you->p_representations);
}

void Fullerene::set_carbon_allotrope(CarbonAllotrope* carbon_allotrope)
{
  p_carbon_allotrope = carbon_allotrope;
  if (p_carbon_allotrope && CarbonAllotrope::s_need_representations)
    {
      int len;
#if defined(DEBUG_FULLERENE_CONSTRUCTION)
      printf("* representations ******************************\n");
#endif
      p_representations = new Representations();
      p_carbon_allotrope->all_representations(p_representations);
#if defined(DEBUG_FULLERENE_CONSTRUCTION)
      len = p_representations->length();
      for (int i = 0; i < len; ++i)
        {
          Representation* rep = p_representations->get_representation(i);
          rep->print();
        }
      printf("************************************************\n");
#endif
      if (CarbonAllotrope::s_need_all_axes || CarbonAllotrope::s_need_major_axes)
        {
          Automorphisms ams = Automorphisms(this);
          p_carbon_allotrope->all_boundaries();
          len = ams.number_of_automorphisms();
          for (int i = 0; i < len; ++i)
            {
              Automorphism* am = ams.get_automorphism(i);
              int order = am->order();
              if (order == 1)
                continue;
              int seq0, seq1, seq2, seq3, seq4, seq5, seq6, seq7;
              int fixed_carbons = am->fixed_carbons(seq0, seq1);
              int fixed_bonds = am->fixed_bonds(seq2, seq3);
              int fixed_rings = am->fixed_rings(seq4, seq5);
              int fixed_boundaries = am->fixed_boundaries(seq6, seq7);
              AxisType type = AXIS_TYPE_CENTER_OF_TWO_CARBONS;
              if ((fixed_carbons == 2) && (fixed_bonds == 0) &&
                  (fixed_rings == 0) && (fixed_boundaries == 0))
                type = AXIS_TYPE_CENTER_OF_TWO_CARBONS;
              else if ((fixed_carbons == 1) && (fixed_bonds == 1) &&
                       (fixed_rings == 0) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_CARBON_AND_BOND;
                  seq1 = seq2;
                }
              else if ((fixed_carbons == 1) && (fixed_bonds == 0) &&
                       (fixed_rings == 1) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_CARBON_AND_RING;
                  seq1 = seq4;
                }
              else if ((fixed_carbons == 1) && (fixed_bonds == 0) &&
                       (fixed_rings == 0) && (fixed_boundaries == 1))
                {
                  type = AXIS_TYPE_CENTER_OF_CARBON_AND_BOUNDARY;
                  seq1 = seq6;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 2) &&
                       (fixed_rings == 0) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_TWO_BONDS;
                  seq0 = seq2;
                  seq1 = seq3;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 1) &&
                       (fixed_rings == 1) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_BOND_AND_RING;
                  seq0 = seq2;
                  seq1 = seq4;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 1) &&
                       (fixed_rings == 0) && (fixed_boundaries == 1))
                {
                  type = AXIS_TYPE_CENTER_OF_BOND_AND_BOUNDARY;
                  seq0 = seq2;
                  seq1 = seq6;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 2) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_TWO_RINGS;
                  seq0 = seq4;
                  seq1 = seq5;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 1) && (fixed_boundaries == 1))
                {
                  type = AXIS_TYPE_CENTER_OF_RING_AND_BOUNDARY;
                  seq0 = seq4;
                  seq1 = seq6;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 0) && (fixed_boundaries == 2))
                {
                  type = AXIS_TYPE_CENTER_OF_TWO_BOUNDARIES;
                  seq0 = seq6;
                  seq1 = seq7;
                }
              else if ((fixed_carbons == 1) && (fixed_bonds == 0) &&
                       (fixed_rings == 0) && (fixed_boundaries == 0))
                type = AXIS_TYPE_CENTER_OF_ONLY_ONE_CARBON;
              else if ((fixed_carbons == 0) && (fixed_bonds == 1) &&
                       (fixed_rings == 0) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_ONLY_ONE_BOND;
                  seq0 = seq2;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 1) && (fixed_boundaries == 0))
                {
                  type = AXIS_TYPE_CENTER_OF_ONLY_ONE_RING;
                  seq0 = seq4;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 0) && (fixed_boundaries == 1))
                {
                  type = AXIS_TYPE_CENTER_OF_ONLY_ONE_BOUNDARY;
                  seq0 = seq6;
                }
              else if ((fixed_carbons == 0) && (fixed_bonds == 0) &&
                       (fixed_rings == 0) && (fixed_boundaries == 0))
                continue;
              else
                {
                  printf("fixed carbons = %d\n", fixed_carbons);
                  printf("fixed bonds = %d\n", fixed_bonds);
                  printf("fixed rings = %d\n", fixed_rings);
                  printf("fixed boundaries = %d\n", fixed_boundaries);
                  assert(0);
                }
              p_carbon_allotrope->register_axis(new SymmetryAxis(type, order,
                                                                 seq0, seq1, am));
            }
        }
    }
  if (p_carbon_allotrope && s_need_fullerene_characteristic)
    p_characteristic = new Characteristic(p_carbon_allotrope);
  if (p_carbon_allotrope && s_need_clustering)
    p_clustering = new Clustering(p_carbon_allotrope);
  if (p_carbon_allotrope && s_need_display_clustering)
    {
      Clusters cl(p_carbon_allotrope);
    }
}

void Fullerene::set_fullerene_name(const char* fullerene_name)
{
  p_fullerene_name = fullerene_name;
}

void Fullerene::set_generator_formula(const char* generator_formula)
{
  p_generator_formula = generator_formula;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
