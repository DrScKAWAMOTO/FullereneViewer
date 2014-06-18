/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:35:19 JST
 */

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "Representations.h"
#include "Automorphisms.h"
#include "FullereneCharacteristic.h"
#include "Generator.h"
#include "DistanceMatrix.h"
#include "Pattern.h"
#include "Utils.h"
#include "Debug.h"
#include "DebugMemory.h"

bool Fullerene::s_need_representations = false;
bool Fullerene::s_need_fullerene_characteristic = false;
bool Fullerene::s_need_distance_matrix = false;

Fullerene::Fullerene()
  : p_carbon_allotrope(0), p_n(0), p_m(0), p_h(0),
    p_representations(0), p_characteristic(0), p_distance_matrix(0)
{
  strcpy(p_generator_label, "");
}

Fullerene::Fullerene(const char* generator_label)
  : p_carbon_allotrope(0), p_n(0), p_m(0), p_h(0),
    p_representations(0), p_characteristic(0), p_distance_matrix(0)
{
  strcpy(p_generator_label, generator_label);
  CarbonAllotrope* ca = new CarbonAllotrope();
  if (strcmp(p_generator_label, "T-Y") == 0)
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
  else if (p_generator_label[0] == 'T')
    {
      p_n = 10;
      p_m = 0;
      p_h = 4;
      sscanf(p_generator_label, "T%d,%d,%d", &p_n, &p_m, &p_h);
      sprintf(p_generator_label, "T%d,%d,%d", p_n, p_m, p_h);
      ca->make_equator_by_chiral_characteristic(p_n, p_m, p_h);
      // TODO ca->close_force();
#ifdef DEBUG_CARBON_ALLOTROPE_CONSTRUCTION
      ca->print_detail();
#endif
    }
  else
    {
      Generator gen = Generator(generator_label, 6);
      bool symmetric = gen.symmetric();
      ca->make_symmetric_scrap(gen.scrap_no());
#ifdef DEBUG_CARBON_ALLOTROPE_CONSTRUCTION
      ca->print_detail();
#endif
      while (1)
        {
          int No = gen.glow();
          int num;
          ErrorCode result;
          if (symmetric)
            result =
              ca->fill_n_polygons_around_carbons_closed_to_center_and_pentagons(No, num);
          else
            result = ca->fill_n_polygon_around_oldest_carbon(No);
          assert(result == ERROR_CODE_OK);
#ifdef DEBUG_CARBON_ALLOTROPE_CONSTRUCTION
          ca->print_detail();
#endif
          List<Carbon> boundary;
          ca->list_boundary_carbons(boundary);
          int number_of_carbons_in_boundary = boundary.length();
          if (number_of_carbons_in_boundary == 0)
            break;
        }
    }
  set_carbon_allotrope(ca);
}

Fullerene::~Fullerene()
{
  if (p_carbon_allotrope)
    delete p_carbon_allotrope;
  if (p_representations)
    delete p_representations;
  if (p_characteristic)
    delete p_characteristic;
  if (p_distance_matrix)
    delete p_distance_matrix;
}

bool Fullerene::operator == (const Fullerene& that) const
{
  return ((*p_representations) == (*that.p_representations));
}

void Fullerene::set_carbon_allotrope(CarbonAllotrope* carbon_allotrope)
{
  int len;
  p_carbon_allotrope = carbon_allotrope;
  if (p_carbon_allotrope &&
      (s_need_fullerene_characteristic || s_need_distance_matrix ||
       s_need_representations))
    {
#ifdef DEBUG_FULLERENE_CONSTRUCTION
      printf("* representations ******************************\n");
#endif
      p_representations = new Representations();
      p_carbon_allotrope->all_representations(p_representations);
#ifdef DEBUG_FULLERENE_CONSTRUCTION
      len = p_representations->length();
      for (int i = 0; i < len; ++i)
        {
          Representation* rep = p_representations->get_representation(i);
          rep->print();
        }
      printf("************************************************\n");
#endif
      Automorphisms ams = Automorphisms(this);
      len = ams.number_of_automorphisms();
      for (int i = 0; i < len; ++i)
        {
          Automorphism* am = ams.get_automorphism(i);
          int order = am->order();
          if (order == 1)
            continue;
          int seq0, seq1, seq2, seq3, seq4, seq5;
          int fixed_carbons = am->fixed_carbons(seq0, seq1);
          int fixed_bonds = am->fixed_bonds(seq2, seq3);
          int fixed_rings = am->fixed_rings(seq4, seq5);
          AxisType type;
          if (fixed_carbons == 2)
            type = AXIS_TYPE_CENTER_OF_CARBON;
          else if (fixed_bonds == 2)
            {
              type = AXIS_TYPE_CENTER_OF_BOND;
              seq0 = seq2;
              seq1 = seq3;
            }
          else if (fixed_rings == 2)
            {
              type = AXIS_TYPE_CENTER_OF_RING;
              seq0 = seq4;
              seq1 = seq5;
            }
          else if ((fixed_rings == 1) && (fixed_bonds == 1))
            {
              type = AXIS_TYPE_CENTER_OF_RING_AND_BOND;
              seq0 = seq4;
              seq1 = seq2;
            }
          else if ((fixed_rings == 1) && (fixed_carbons == 1))
            {
              type = AXIS_TYPE_CENTER_OF_RING_AND_CARBON;
              seq1 = seq0;
              seq0 = seq4;
            }
          else
            assert(0);
          p_carbon_allotrope->register_axis(new SymmetryAxis(type, order,
                                                             seq0, seq1, am));
        }
      if (s_need_fullerene_characteristic)
        p_characteristic = new FullereneCharacteristic(p_carbon_allotrope);
      if (s_need_distance_matrix)
        p_distance_matrix = new DistanceMatrix(p_carbon_allotrope);
    }
}

void Fullerene::set_fullerene_name(const char* fullerene_name)
{
  assert(strlen(fullerene_name) <= NAME_MAX_SIZE);
  strcpy(p_fullerene_name, fullerene_name);
}

void Fullerene::set_generator_label(const char* generator_label)
{
  assert(strlen(generator_label) <= NAME_MAX_SIZE);
  strcpy(p_generator_label, generator_label);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
