/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <float.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include "Config.h"
#include "Configuration.h"
#include "CarbonAllotrope.h"
#include "BoundaryCarbons.h"
#include "RepresentationInfo.h"
#include "Representation.h"
#include "Representations.h"
#include "InteractivePlane.h"
#include "InteractiveLine.h"
#include "ThreeViewNormal.h"
#include "SymmetryAxisNormal.h"
#include "Automorphism.h"
#include "Statistics.h"
#include "Queue.h"
#include "Utils.h"
#include "OpenGLUtil.h"
#include "Pattern.h"
#include "Debug.h"
#include "DebugMemory.h"

#define MAX_N 10

bool CarbonAllotrope::s_need_representations = false;
bool CarbonAllotrope::s_need_representations_reflection = false;
bool CarbonAllotrope::s_need_all_axes = false;
bool CarbonAllotrope::s_need_major_axes = false;
bool CarbonAllotrope::s_need_principal_component_axes = false;
bool CarbonAllotrope::s_need_tsuzumi_expansion = false;
int CarbonAllotrope::s_flags_displaying_objects = (S_FLAGS_MASK_RINGS |
                                                   S_FLAGS_MASK_BONDS |
                                                   S_FLAGS_MASK_CARBONS);

void CarbonAllotrope::p_make_n_polygon(int n_members)
{
  new Ring(this, n_members);
}

void CarbonAllotrope::p_set_center()
{
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("### p_set_center()\n");
#endif
  p_centers = p_carbons;
}

void CarbonAllotrope::p_list_most_inside_carbons_on_boundary(List<Carbon>& boundary)
{
  assert(p_centers.length());
  boundary.clean();
  calculate_distances_to_set(p_centers);
  int len = p_carbons.length();
  int minimum_distance = INT_MAX;
  int minimum_sequence_no = INT_MAX;
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      if (carbon->number_of_rings() == 3)
        continue;
      int dist = carbon->distance_to_set();
      if (dist < minimum_distance)
        {
          minimum_distance = dist;
          minimum_sequence_no = carbon->sequence_no();
        }
      else if (dist == minimum_distance)
        {
          int seq = carbon->sequence_no();
          if (seq < minimum_sequence_no)
            minimum_sequence_no = seq;
        }
    }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("### p_list_most_inside_carbons_on_boundary() =\n");
#endif
  if (minimum_sequence_no == INT_MAX)
    {
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
      printf(" none\n");
#endif
      assert(0);
      return;
    }
  Carbon* start = get_carbon_by_sequence_no(minimum_sequence_no);
  Carbon* carbon = start;
  assert(carbon);
  Bond* bond = carbon->boundary_bond();
  bond = carbon->boundary_bond(bond);
  int minimum_sequence = INT_MAX;
  while (1)
    { /* 逆向きに辿って minimum_distance なカーボンのうち左端を検索 */
      Carbon* carbon2 = bond->get_carbon_beyond(carbon);
      if (minimum_distance < carbon2->distance_to_set())
        break;
      carbon = carbon2;
      /* もしくはループしている場合は最小のシーケンス番号のものを検索 */
      if (carbon->sequence_no() < minimum_sequence)
        minimum_sequence = carbon->sequence_no();
      else if (carbon->sequence_no() == minimum_sequence)
        break;
      bond = carbon->boundary_bond();
      bond = carbon->boundary_bond(bond);
    }
  bond = carbon->boundary_bond(0);
  bool connected = false;
  start = carbon;
  while (1)
    {
      if (carbon->distance_to_set() == minimum_distance)
        {
          if (!connected)
            boundary.add(carbon);
          connected = true;
        }
      else
        connected = false;
      bond = carbon->boundary_bond();
      carbon = bond->get_carbon_beyond(carbon);
      if (carbon == start)
        break;
    }
  len = boundary.length();
  assert((len == 1) || ((len % p_scrap_order) == 0));
#if defined(CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON)
  p_calculate_distances_to_pentagons();
#endif
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = boundary[i];
      int dist = carbon->distance_to_set();
      if (dist == INT_MAX)
        printf(" C%d(INF)", carbon->sequence_no());
      else
        printf(" C%d(%d)", carbon->sequence_no(), dist);
    }
  printf("\n");
#endif
}

CarbonAllotrope::CarbonAllotrope()
  : p_has_reflection_symmetricity(false),
    p_Eigenvalue1(-1.0), p_Eigenvalue2(-1.0), p_Eigenvalue3(-1.0),
    ring_next_sequence(1),
    carbon_next_sequence(1), carbon_radius(0.3), carbon_color(0x000000),
    bond_next_sequence(1), bond_radius(0.1), bond_color(0xffffff),
    boundary_next_sequence(1),
    print_out_sequence_no(false)
{
}

CarbonAllotrope* CarbonAllotrope::copy() const
{
  CarbonAllotrope* result = new CarbonAllotrope();
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    new Ring(get_ring(i)->number_of_carbons(), result);
  len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    new Carbon(result);
  len = number_of_bonds();
  for (int i = 0; i < len; ++i)
    new Bond(result);
  assert(number_of_boundaries() == 0);
  result->has_reflection_symmetricity(has_reflection_symmetricity());
  assert(number_of_axes() == 0);
  result->p_center_location = p_center_location;
  result->p_Eigenvalue1 = p_Eigenvalue1;
  result->p_Eigenvalue2 = p_Eigenvalue2;
  result->p_Eigenvalue3 = p_Eigenvalue3;
  result->p_Eigenvector1 = p_Eigenvector1;
  result->p_Eigenvector2 = p_Eigenvector2;
  result->p_Eigenvector3 = p_Eigenvector3;
  result->carbon_radius = carbon_radius;
  result->carbon_color = carbon_color;
  result->bond_radius = bond_radius;
  result->bond_color = bond_color;
  result->print_out_sequence_no = print_out_sequence_no;
  len = number_of_rings();
  for (int i = 0; i < len; ++i)
    result->get_ring(i)->copy_from(result, get_ring(i));
  len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    result->get_carbon(i)->copy_from(result, get_carbon(i));
  len = p_centers.length();
  for (int i = 0; i < len; ++i)
    result->p_centers.add(result->get_carbon_by_sequence_no(p_centers[i]->sequence_no()));
  len = number_of_bonds();
  for (int i = 0; i < len; ++i)
    result->get_bond(i)->copy_from(result, get_bond(i));
  return result;
}

CarbonAllotrope::~CarbonAllotrope()
{
}

void CarbonAllotrope::clean()
{
  p_rings.clean();
  p_carbons.clean();
  p_bonds.clean();
}

ErrorCode CarbonAllotrope::p_fill_hexagons_around(Ring* ring)
{
  int len = ring->number_of_carbons();
  while (1)
    {
      int i = 0;
      Carbon* carbon;
      while (1)
        {
          if (i == len)
            {
              for (i = 0; 1; ++i)
                {
                  if (i == len)
                    return ERROR_CODE_OK;
                  Carbon* carbon0 = ring->get_carbon(i);
                  if (carbon0->number_of_rings() == 1)
                    {
                      new Ring(this, 6, ring->get_bond_after(i));
                      break;
                    }
                }
            }
          carbon = ring->get_carbon(i);
          if (carbon->number_of_rings() == 2)
            {
              Bond* end_bond;
              int length;
              ErrorCode result = carbon->concave_boundary_segment(6, length, end_bond);
              if (result != ERROR_CODE_OK)
                return result;
              new Ring(this, 6, end_bond);
              break;
            }
          ++i;
        }
    }
}

ErrorCode CarbonAllotrope::fill_hexagons_around_n_polygons(int n_members)
{
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    if (p_rings[i]->number_of_carbons() == n_members)
      {
        ErrorCode error_code = p_fill_hexagons_around(p_rings[i]);
        if (error_code != ERROR_CODE_OK)
          return error_code;
      }
  return ERROR_CODE_OK;
}

int CarbonAllotrope::p_calculate_period(const List<Carbon>& boundary, int& offset)
{
  int length = boundary.length();
  int* pattern = (int*)malloc(length * sizeof(int));
  for (int i = 0; i < length; ++i)
    pattern[i] = boundary[i]->number_of_rings();
  int period = 0;
  for (period = 1; period < length; ++period)
    {
      int i;
      for (i = 0; i < length; ++i)
        if (pattern[i] != pattern[(i + period) % length])
          break;
      if (i == length)
        break;
    }
  assert((period > 0) && (period < length));
  int min_i;
  if (period == 1)
    {
      min_i = 0;
    }
  else
    {
      min_i = -1;
      int min_sum = INT_MAX;
      for (int i = 0; i < period; ++i)
        {
          int sum = 0;
          for (int j = 0; j < period; ++j)
            sum += pattern[i + j] * j;
          if (min_sum > sum)
            {
              min_i = i;
              min_sum = sum;
            }
        }
      while (pattern[min_i] == 1)
        ++min_i;
      min_i %= period;
    }
  printf("period = %d\n", period);
  printf("number = %d\n", length / period);
  printf("min_i = %d\n", min_i);
  assert((min_i >= 0) && (min_i < period));
  assert(pattern[min_i] == 2);
  offset = min_i;
  delete pattern;
  return period;
}

ErrorCode CarbonAllotrope::enlarge_cylinder_by_n_polygons(Pattern* n_pattern,
                                                          int& result_number)
{
  List<Carbon> boundary;
  list_newborn_connected_boundary(boundary);
  int len = boundary.length();
  if (len == 0)
    {
      delete n_pattern;
      return ERROR_CODE_OK;
    }
  result_number = 0;
  while (1)
    {
      int min = INT_MAX;
      Carbon* min_c = 0;
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 2)
            {
              int seq = carbon->sequence_no();
              if (min > seq)
                {
                  min = seq;
                  min_c = carbon;
                }
            }
        }
      if (min == INT_MAX)
        break;
      new Ring(this, n_pattern->get_value_then_next(), min_c);
      ++result_number;
    }
  delete n_pattern;
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::make_fullerene(int required_distance)
{
  assert(required_distance > 0);
  ErrorCode result;
  result = make_symmetric_scrap(1);
  if (result != ERROR_CODE_OK)
    return result;
  while (1)
    {
      p_calculate_distances_to_pentagons();
      List<Carbon> boundary;
      p_list_most_inside_carbons_on_boundary(boundary);
      int len = boundary.length();
      int index;
      int satisfied_distance = -1;
      Carbon* target = NULL;
      for (index = 0; index < len; ++index)
        {
          target = boundary[index];
          satisfied_distance = target->distance_to_set();
          if (satisfied_distance <= required_distance)
            break;
        }
      if (index == len)
        break;
      int n_members;
      if (satisfied_distance == required_distance)
        n_members = 5;
      else
        n_members = 6;
      Bond* end_bond;
      int length;
      result = target->concave_boundary_segment(n_members, length, end_bond);
      if (result != ERROR_CODE_OK)
        return result;
      new Ring(this, n_members, end_bond);
    }
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::append_n_polygon_at_carbon(int n_members,
                                                      int carbon_sequence_no)
{
  Carbon* carbon_connection = get_carbon_by_sequence_no(carbon_sequence_no);
  Carbon* carbon = carbon_connection;
  if (!carbon)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_carbon(%d, %d)\n",
              n_members, carbon_sequence_no);
      fprintf(stderr, "Unknown carbon sequence No. Carbon(%d).\n", carbon_sequence_no);
      exit(1);
    }
  if (carbon->number_of_rings() != 2)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_carbon(%d, %d)\n",
              n_members, carbon_sequence_no);
      fprintf(stderr, "Carbon(%d) has not have exact two rings.\n",
              carbon_sequence_no);
      exit(1);
    }
  assert(carbon->number_of_bonds() == 3);
  Bond* bond0 = carbon->get_bond(0);
  Bond* bond1 = carbon->get_bond(1);
  Bond* bond2 = carbon->get_bond(2);
  Bond* bond_connection = ((carbon->ring_between(bond0, bond1)) ? bond2 : bond0);
  Bond* bond = bond_connection;
  int count = 1;
  while (1)
    {
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      carbon = bond->get_carbon_beyond(carbon);
      count++;
      if (bond == bond_connection)
        {
          if (count != n_members)
            {
              fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_carbon(%d, %d)\n",
                      n_members, carbon_sequence_no);
              fprintf(stderr, "A %d-polygon doesn't fit the %d-hole.\n",
                      n_members, count);
              exit(1);
            }
          Bond* end_bond;
          int length;
          ErrorCode result;
          result = carbon_connection->concave_boundary_segment(n_members,
                                                               length, end_bond);
          if (result != ERROR_CODE_OK)
            return result;
          new Ring(this, n_members, end_bond);
          return ERROR_CODE_OK;
        }
      if (count == n_members - 1)
        {
          fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_carbon(%d, %d)\n",
                  n_members, carbon_sequence_no);
          fprintf(stderr, "Carbon(%d) belongs too long ring path to fit a %d-polygon.\n",
                  carbon_sequence_no, n_members);
          exit(1);
        }
    }
  assert(carbon->number_of_rings() == 1);
  carbon = bond->get_carbon_beyond(carbon);
  count = 0;
  while (1)
    {
      Bond* result;
      assert(carbon->open_side_bond(bond, result));
      if (!result)
        break;
      bond = result;
      carbon = bond->get_carbon_beyond(carbon);
      count++;
    }
  if (count >= n_members - 1)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_carbon(%d, %d)\n",
              n_members, carbon_sequence_no);
      fprintf(stderr, "Carbon(%d) belongs too long ring path to fit a %d-polygon.\n",
              carbon_sequence_no, n_members);
      exit(1);
    }
  Bond* end_bond;
  int length;
  ErrorCode result;
  result = carbon_connection->concave_boundary_segment(n_members, length, end_bond);
  if (result != ERROR_CODE_OK)
    return result;
  new Ring(this, n_members, end_bond);
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::append_n_polygon_at_carbons(int n_members,
                                                       const int* carbon_sequence_nos)
{
  new Ring(this, n_members, carbon_sequence_nos);
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::append_n_polygon_at_bond(int n_members, int bond_sequence_no)
{
  Bond* bond = get_bond_by_sequence_no(bond_sequence_no);
  if (!bond)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_bond(%d, %d)\n",
              n_members, bond_sequence_no);
      fprintf(stderr, "Unknown bond sequence No. Bond(%d).\n", bond_sequence_no);
      exit(1);
    }
  Carbon* carbon0 = bond->get_left_carbon();
  if (!carbon0)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_bond(%d, %d)\n",
              n_members, bond_sequence_no);
      fprintf(stderr, "Specified Bond(%d) does not belong to two bonds.\n",
              bond_sequence_no);
      exit(1);
    }
  if (carbon0->number_of_rings() != 1)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_bond(%d, %d)\n",
              n_members, bond_sequence_no);
      fprintf(stderr, "Carbon(%d) does not belong to exact one ring.\n",
              carbon0->sequence_no());
      exit(1);
    }
  Carbon* carbon1 = bond->get_right_carbon();
  if (!carbon1)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_bond(%d, %d)\n",
              n_members, bond_sequence_no);
      fprintf(stderr, "Specified Bond(%d) does not belong to two bonds.\n",
              bond_sequence_no);
      exit(1);
    }
  if (carbon1->number_of_rings() != 1)
    {
      fprintf(stderr, "CarbonAllotrope::append_n_polygon_at_bond(%d, %d)\n",
              n_members, bond_sequence_no);
      fprintf(stderr, "Carbon(%d) does not belong to exact one ring.\n",
              carbon1->sequence_no());
      exit(1);
    }
  new Ring(this, n_members, bond);
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::
fill_n_polygons_around_carbons_closed_to_center_and_pentagons(int n_members,
                                                              int& number_of_results)
{
  List<Carbon> boundary;
  p_list_most_inside_carbons_on_boundary(boundary);
  int len = boundary.length();
#if defined(CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON)
  int step = 1;
  int min = INT_MAX;
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = boundary[i];
      int dist = carbon->distance_to_set();
      if (dist < min)
        min = dist;
    }
#else
  int step = (len == 1) ? 1 : (len / p_scrap_order);
#endif
  for (int i = 0; i < len; i += step)
    {
      Carbon* carbon = boundary[i];
#if defined(CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON)
      if (carbon->distance_to_set() > min)
        continue;
#endif
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
      printf("====> Carbon(%d) %d-polygon\n", carbon->sequence_no(), n_members);
#endif
      assert(carbon->number_of_rings() == 2);
      Bond* end_bond;
      int length;
      ErrorCode result;
      result = carbon->concave_boundary_segment(n_members, length, end_bond);
      if (result != ERROR_CODE_OK)
        return result;
    }
  number_of_results = 0;
  for (int i = 0; i < len; i += step)
    {
      Carbon* carbon = boundary[i];
#if defined(CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON)
      if (carbon->distance_to_set() > min)
        continue;
#endif
      if (carbon->number_of_rings() != 2) /* TODO こうなるケースをデバッグして対策する */
        {
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
          printf("### Carbon(%d) is %d-ring !!\n", carbon->sequence_no(),
                 carbon->number_of_rings());
#endif
          continue;
        }
      Bond* end_bond;
      int length;
      ErrorCode result;
      result = carbon->concave_boundary_segment(n_members, length, end_bond);
      if (result != ERROR_CODE_OK)
        return result;
      new Ring(this, n_members, end_bond);
      number_of_results++;
    }
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::fill_n_polygon_around_carbon(int n_members, Carbon* carbon,
                                                        BoundaryCarbons& boundary)
{
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("====> Carbon(%d) %d-polygon\n", carbon->sequence_no(), n_members);
#endif
  Bond* end_bond;
  int length;
  ErrorCode result;
  result = carbon->concave_boundary_segment(n_members, length, end_bond);
  if (result != ERROR_CODE_OK)
    return result;
  Ring* ring = new Ring(this, n_members, end_bond);
  for (int i = 0; i < n_members; ++i)
    {
      Carbon* carbon2 = ring->get_carbon(i);
      switch (carbon2->number_of_rings())
        {
        case 1:
          boundary.glow_to_one_ring_carbon(carbon2);
          break;
        case 2:
          boundary.glow_to_two_rings_carbon(carbon2);
          break;
        default:
        case 3:
          boundary.glow_to_three_rings_carbon(carbon2);
          break;
        }
    }
  return ERROR_CODE_OK;
}

ErrorCode CarbonAllotrope::make_symmetric_scrap(int scrap_no)
{
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("### make_symmetric_scrap(%d)\n", scrap_no);
#endif
  p_scrap_no = scrap_no;
  ErrorCode result;
  switch (scrap_no)
    {
    case 1: /* only one pentagon */
      p_scrap_order = 5;
      p_make_n_polygon(5);
      p_set_center();
      result = fill_hexagons_around_n_polygons(5);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 2: /* only three pentagons */
      p_scrap_order = 3;
      p_make_n_polygon(6);
      p_set_center();
      result = append_n_polygon_at_bond(5, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_bond(5, 3);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_bond(5, 5);
      if (result != ERROR_CODE_OK)
        return result;
      result = fill_hexagons_around_n_polygons(5);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 3: /* only one hexagon, order = 6 */
      p_scrap_order = 6;
    only_one_hexagon:
      p_make_n_polygon(6);
      p_set_center();
      result = append_n_polygon_at_bond(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 2);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 3);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 4);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 5);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 6);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 4: /* only one hexagon, order = 3 */
      p_scrap_order = 3;
      goto only_one_hexagon;
    case 5: /* only one hexagon, order = 2 */
      p_scrap_order = 2;
      goto only_one_hexagon;
    case 6: /* only three hexagons */
      p_scrap_order = 3;
      p_make_n_polygon(6);
      result = append_n_polygon_at_bond(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 2);
      if (result != ERROR_CODE_OK)
        return result;
      p_set_center();
      result = append_n_polygon_at_carbon(5, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(5, 3);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(5, 7);
      if (result != ERROR_CODE_OK)
        return result;
      result = fill_hexagons_around_n_polygons(5);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 7: /* only six hexagons */
      p_scrap_order = 3;
      p_make_n_polygon(6);
      result = append_n_polygon_at_bond(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 2);
      if (result != ERROR_CODE_OK)
        return result;
      p_set_center();
      result = append_n_polygon_at_carbon(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 3);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 7);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 8: /* only two hexagons */
      p_scrap_order = 2;
      p_make_n_polygon(6);
      result = append_n_polygon_at_bond(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      p_set_center();
      result = append_n_polygon_at_carbon(5, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(5, 2);
      if (result != ERROR_CODE_OK)
        return result;
      result = fill_hexagons_around_n_polygons(5);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    case 9: /* only four hexagons */
      p_scrap_order = 2;
      p_make_n_polygon(6);
      result = append_n_polygon_at_bond(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      p_set_center();
      result = append_n_polygon_at_carbon(6, 1);
      if (result != ERROR_CODE_OK)
        return result;
      result = append_n_polygon_at_carbon(6, 2);
      if (result != ERROR_CODE_OK)
        return result;
      break;
    default:
      assert((scrap_no >= 1) && (scrap_no <= 9));
      break;
    }
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
  printf("### return from make_symmetric_scrap()\n");
#endif
  return ERROR_CODE_OK;
}

char* CarbonAllotrope::p_determine_permutation_of_lattice_basis(int n, int m)
{
  char* perm = new char[n + m];
  char top, sec;
  if (m > n)
    {
      top = 2;
      sec = 1;
      int w = m;
      m = n;
      n = w;
    }
  else
    {
      top = 1;
      sec = 2;
    }
  int ind = 0;
  if (m == 0)
    {
      for (int ofs = 0; ofs < n; ++ofs)
        perm[ind++] = top;
      return perm;
    }
  if (n == m)
    {
      for (int ofs = 0; ofs < n; ++ofs)
        {
          perm[ind++] = top;
          perm[ind++] = sec;
        }
      return perm;
    }
  int count = 0;
  for (int ofs = 0; ofs < n; ++ofs)
    {
      count += m;
      if (count >= n)
        {
          perm[ind++] = sec;
          count -= n;
        }
      perm[ind++] = top;
    }
  return perm;
}


void CarbonAllotrope::p_make_equator_by_chiral_characteristic(int n, int m)
{
  char* perm = p_determine_permutation_of_lattice_basis(n, m);
  int num = n + m;
  p_make_n_polygon(6);
  Carbon* s_c = get_carbon_by_sequence_no(1);
  Carbon* d_c = get_carbon_by_sequence_no(2);
  Ring* rng = get_ring_by_sequence_no(1);
  char last_perm = perm[num - 1];
  for (int ofs = 0; ofs < num; ++ofs)
    {
      int s_i = -1;
      int d_i = -1;
      for (int i = 0; i < 6; ++i)
        {
          Carbon* car = rng->get_carbon(i);
          // swap source and dest
          if (car == s_c)
            d_i = i;
          else if (car == d_c)
            s_i = i;
        }
      int dif = d_i - s_i;
      if (dif < 0)
        dif += 6;
      // find_next_carbons
      if (dif == 1)
        {
          if (perm[ofs] == 1)
            {
              if (last_perm == 1)
                {
                  s_i += 3;
                  d_i += 3;
                }
              else
                {
                  s_i += 2;
                  d_i += 2;
                }
            }
          else
            {
              if (last_perm == 1)
                {
                  s_i += 4;
                  d_i += 4;
                }
              else
                {
                  s_i += 3;
                  d_i += 3;
                }
            }
          if (s_i >= 6)
            s_i -= 6;
          if (d_i >= 6)
            d_i -= 6;
        }
      else
        {
          if (perm[ofs] == 1)
            {
              if (last_perm == 1)
                {
                  s_i -= 3;
                  d_i -= 3;
                }
              else
                {
                  s_i -= 2;
                  d_i -= 2;
                }
            }
          else
            {
              if (last_perm == 1)
                {
                  s_i -= 4;
                  d_i -= 4;
                }
              else
                {
                  s_i -= 3;
                  d_i -= 3;
                }
            }
          if (s_i < 0)
            s_i += 6;
          if (d_i < 0)
            d_i += 6;
        }
      last_perm = perm[ofs];
      s_c = rng->get_carbon(s_i);
      d_c = rng->get_carbon(d_i);
      if (ofs == num - 2)
        break;
      // make_next_ring
      append_n_polygon_at_bond(6, s_c->bond_between(d_c)->sequence_no());
      assert(s_c->number_of_rings() == 2);
      assert(s_c->get_ring(0) == rng);
      rng = s_c->get_ring(1);
    }
  if (perm[num - 2] == 1)
    {
      if (perm[num - 1] == 1)
        {
          int nos[6] = { 2, 1, 0, s_c->sequence_no(), d_c->sequence_no(), 0 };
          new Ring(this, 6, nos);
        }
      else
        {
          int nos[6] = { 2, 1, 0, 0, s_c->sequence_no(), d_c->sequence_no() };
          new Ring(this, 6, nos);
        }
    }
  else
    {
      if (perm[num - 1] == 1)
        {
          int nos[6] = { 2, 1, s_c->sequence_no(), d_c->sequence_no(), 0, 0 };
          new Ring(this, 6, nos);
        }
      else
        {
          int nos[6] = { 2, 1, 0, s_c->sequence_no(), d_c->sequence_no(), 0 };
          new Ring(this, 6, nos);
        }
    }
  delete[] perm;
}

void CarbonAllotrope::make_equator_by_chiral_characteristic(int n, int m, int h)
{
  assert(n >= 0);
  assert(m >= 0);
  assert(h >= 1);
  p_make_equator_by_chiral_characteristic(n, m);
  p_set_center();
  while (--h > 0)
    {
      int result_number = 0;
      enlarge_cylinder_by_n_polygons(new Pattern(6), result_number);
    }
}

void CarbonAllotrope::close_force()
{
  while (1)
    {
      List<Carbon> boundary;
      list_newborn_connected_boundary(boundary);
      int len = boundary.length();
      if (len == 0)
        return;
      int num = 0;
      Carbon* carbon2 = 0;
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 1)
            num++;
          else if (carbon->number_of_rings() == 2)
            carbon2 = carbon;
        }
      assert(carbon2);
      assert(num != 1);
      if (num == 0)
        new Ring(this, len, carbon2);
      else
        {
          int twices = 1;
          Bond* bond3 = carbon2->boundary_bond(0);
          Bond* bond2 = bond3;
          assert(bond3);
          Carbon* carbon3 = carbon2;
          while (1)
            {
              bond3 = carbon3->boundary_bond(bond3);
              assert(bond3);
              carbon3 = bond3->get_carbon_beyond(carbon3);
              assert(carbon3);
              ++twices;
              if (carbon3->number_of_rings() == 1)
                break;
            }
          bond3 = carbon2->boundary_bond(bond2);
          assert(bond3);
          carbon3 = carbon2;
          while (1)
            {
              bond3 = carbon3->boundary_bond(bond3);
              carbon3 = bond3->get_carbon_beyond(carbon3);
              ++twices;
              if (carbon3->number_of_rings() == 1)
                break;
            }
          if (num == 2)
            new Ring(this, twices, carbon2);
          else
            new Ring(this, twices + 1, carbon2);
        }
    }
}

void CarbonAllotrope::close_normally_once()
{
  int number_of_pentagons = 0;
  while (1)
    {
      List<Carbon> boundary;
#if defined(DEBUG_CARBON_ALLOTROPE_CONSTRUCTION)
      print_detail();
#endif
      list_newborn_connected_boundary(boundary);
      int len = boundary.length();
      assert(len > 0);
      int num = 0;
      Carbon* carbon2 = 0;
      int min_seq = INT_MAX;
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 1)
            num++;
          else if (carbon->number_of_rings() == 2)
            {
              int seq = carbon->sequence_no();
              if (seq < min_seq)
                {
                  min_seq = seq;
                  carbon2 = carbon;
                }
            }
        }
      assert(carbon2);
      assert(num != 1);
      if (num == 0)
        {
          assert((len == 6) || (len == 5));
          assert((len == 6) || (number_of_pentagons == 5));
          assert((len == 5) || (number_of_pentagons == 6));
          if (len == 5)
            {
              Bond* bond3 = carbon2->boundary_bond(0);
              assert(bond3);
              Carbon* carbon3 = carbon2;
              while (1)
                { /* check isolated pentagon rule */
                  assert(carbon3->number_of_rings() == 2);
                  Ring* ring = carbon3->get_ring(0);
                  assert(ring);
                  assert(ring->number_of_carbons() == 6);
                  ring = carbon3->get_ring(1);
                  assert(ring);
                  assert(ring->number_of_carbons() == 6);
                  bond3 = carbon3->boundary_bond(bond3);
                  assert(bond3);
                  carbon3 = bond3->get_carbon_beyond(carbon3);
                  assert(carbon3);
                  if (carbon3 == carbon2)
                    break;
                }
            }
          new Ring(this, len, carbon2);
          return;
        }
      else
        {
          int twices = 1;
          bool neighbours_are_hexagons = true;
          Bond* bond3 = carbon2->boundary_bond(0);
          Bond* bond2 = bond3;
          assert(bond3);
          Carbon* carbon3 = carbon2;
          while (1)
            {
              if (carbon3->has_non_hexagon())
                neighbours_are_hexagons = false;
              bond3 = carbon3->boundary_bond(bond3);
              assert(bond3);
              carbon3 = bond3->get_carbon_beyond(carbon3);
              assert(carbon3);
              ++twices;
              if (carbon3->number_of_rings() == 1)
                break;
            }
          bond3 = carbon2->boundary_bond(bond2);
          assert(bond3);
          carbon3 = carbon2;
          while (1)
            {
              if (carbon3->has_non_hexagon())
                neighbours_are_hexagons = false;
              bond3 = carbon3->boundary_bond(bond3);
              carbon3 = bond3->get_carbon_beyond(carbon3);
              ++twices;
              if (carbon3->number_of_rings() == 1)
                break;
            }
          if (num == 2)
            {
              assert((twices == 5) || (twices == 6));
              if (twices == 5)
                {
                  assert(neighbours_are_hexagons);
                  assert(number_of_pentagons < 6);
                  ++number_of_pentagons;
                }
              new Ring(this, twices, carbon2);
            }
          else
            {
              assert(twices <= 6);
              if ((twices <= 5) && (number_of_pentagons < 6) && neighbours_are_hexagons)
                {
                  ++number_of_pentagons;
                  new Ring(this, 5, carbon2);
                }
              else
                {
                  new Ring(this, 6, carbon2);
                }
            }
        }
    }
}

void CarbonAllotrope::rollback(int a_ring_next_sequence, int a_carbon_next_sequence,
                               int a_bond_next_sequence)
{
  int len = number_of_rings();
  for (int i = len - 1; i >= a_ring_next_sequence - 1; --i)
    p_rings.remove(i);
  ring_next_sequence = a_ring_next_sequence;
  len = number_of_carbons();
  for (int i = len - 1; i >= a_carbon_next_sequence - 1; --i)
    p_carbons.remove(i);
  carbon_next_sequence = a_carbon_next_sequence;
  len = number_of_bonds();
  for (int i = len - 1; i >= a_bond_next_sequence - 1; --i)
    p_bonds.remove(i);
  bond_next_sequence = a_bond_next_sequence;
}

void CarbonAllotrope::p_calculate_distances_to_pentagons()
{
  List<Carbon> set;
  int len = p_rings.length();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = p_rings[i];
      if (ring->number_of_carbons() != 5)
        continue;
      for (int j = 0; j < 5; ++j)
        set.add(ring->get_carbon(j));
    }
  return calculate_distances_to_set(set);
}

void CarbonAllotrope::calculate_distances_to_set(List<Carbon>& set)
{
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    p_carbons[i]->reset_distance_to_set();
  Queue<Carbon> operations;
  len = set.length();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = set[i];
      carbon->set_distance_to_set(0);
      operations.enqueue(carbon);
    }
  while (1)
    {
      Carbon* next = operations.dequeue();
      if (next == 0)
        return;
      next->calculate_distance_around(operations);
    }
}

int CarbonAllotrope::p_maximum_distance_to_set()
{
  int max_distance = 0;
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      int dist = p_carbons[i]->distance_to_set();
      if (dist > max_distance)
        max_distance = dist;
    }
  return max_distance;
}

void CarbonAllotrope::print_distances_between_pentagons()
{
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = get_ring(i);
      if (ring->number_of_carbons() != 5)
        continue;
      List<Carbon> set;
      for (int j = 0; j < 5; ++j)
        set.add(ring->get_carbon(j));
      calculate_distances_to_set(set);
      printf("distance =");
      for (int j = 0; j < len; ++j)
        {
          Ring* ring2 = get_ring(j);
          if (i == j)
            continue;
          if (ring2->number_of_carbons() != 5)
            continue;
          int min_dist = INT_MAX;
          for (int k = 0; k < 5; ++k)
            {
              Carbon* carbon = ring2->get_carbon(k);
              int dist = carbon->distance_to_set();
              if (dist == 0)
                continue;
              if (dist < min_dist)
                min_dist = dist;
            }
          printf(" %d", min_dist);
        }
      printf("\n");
    }
}

void CarbonAllotrope::set_clockwise(int clockwise)
{
  int len = p_carbons.length();
  assert(len > 0);
  for (int i = 0; i < len; ++i)
    p_carbons[i]->reset_clockwise();
  len = p_rings.length();
  assert(len > 0);
  for (int i = 0; i < len; ++i)
    p_rings[i]->reset_clockwise();
  Ring* ring0 = p_rings[0];
  assert(ring0->number_of_carbons() >= 2);
  if (clockwise > 0)
    ring0->set_clockwise(ring0->get_carbon(0), ring0->get_carbon(1));
  else
    ring0->set_clockwise(ring0->get_carbon(1), ring0->get_carbon(0));
}

void CarbonAllotrope::reset_done()
{
  int len = p_carbons.length();
  assert(len > 0);
  for (int i = 0; i < len; ++i)
    p_carbons[i]->reset_done();
}

void CarbonAllotrope::p_reset_interaction()
{
  int carbon_len = p_carbons.length();
  p_center_location = Vector3();
  for (int i = 0; i < carbon_len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      p_center_location += carbon->carbon_location();
    }
  p_center_location /= carbon_len;
}

void CarbonAllotrope::register_interactions()
{
  int carbon_len = p_carbons.length();
  for (int i = 0; i < carbon_len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      carbon->fix_radius_length(1.0);
      int bond_len = carbon->number_of_bonds();
      for (int j = 0; j < bond_len; ++j)
        {
          Bond* bond = carbon->get_bond(j);
          Carbon* beyond = bond->get_carbon_beyond(carbon);
          p_register_interaction(LOCATION_FORCE_TYPE_ATTRACTIVE,
                                 carbon, ACTION_LOCATION_VERTEX + j,
                                 beyond, ACTION_LOCATION_CENTER);
          p_register_interaction(NORMAL_FORCE_TYPE_OUTSIDE_ARRANGEMENT, carbon, beyond);
        }
      for (int j = i + 1; j < carbon_len; ++j)
        {
          Carbon* the_other = p_carbons[j];
          p_register_interaction(LOCATION_FORCE_TYPE_REPULSIVE,
                                 carbon, ACTION_LOCATION_CENTER,
                                 the_other, ACTION_LOCATION_CENTER);
        }
    }
  if (s_need_principal_component_axes)
    {
      ThreeViewNormal* first_axis = new ThreeViewNormal(this, 1);
      p_register_interaction(ORIGINAL_FORCE_TYPE_ORIGINAL, first_axis);
      ThreeViewNormal* second_axis = new ThreeViewNormal(this, 2);
      p_register_interaction(ORIGINAL_FORCE_TYPE_ORIGINAL, second_axis);
      ThreeViewNormal* third_axis = new ThreeViewNormal(this, 3);
      p_register_interaction(ORIGINAL_FORCE_TYPE_ORIGINAL, third_axis);
    }
  if (s_need_major_axes)
    {
      List<SymmetryAxis> major_axes;
      get_major_axes(major_axes);
      int list_len = major_axes.length();
      for (int j = 0; j < list_len; ++j)
        {
          SymmetryAxis* major_axis = major_axes[j];
          SymmetryAxisNormal* axis_normal = new SymmetryAxisNormal(this, major_axis);
          p_register_interaction(ORIGINAL_FORCE_TYPE_ORIGINAL, axis_normal);
        }
    }
  if (s_need_all_axes)
    {
      int len = number_of_axes();
      for (int i = 0; i < len; ++i)
        {
          SymmetryAxis* axis = get_axis(i);
          SymmetryAxisNormal* axis_normal = new SymmetryAxisNormal(this, axis);
          p_register_interaction(ORIGINAL_FORCE_TYPE_ORIGINAL, axis_normal);
        }
    }
}

void CarbonAllotrope::p_all_representations_half(Representations* results, int clockwise)
{
  int len = p_carbons.length();
  set_clockwise(clockwise);
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      if (carbon->number_of_bonds() != 3)
        continue;
      for (int j = 0; j < 3; ++j)
        {
          Bond* bond = carbon->get_bond(j);
          reset_done();
          RepresentationInfo* info = new RepresentationInfo();
          Representation* rep = new Representation();
          info->clockwise = clockwise;
          info->carbon_sequence_no = carbon->sequence_no();
          info->bond_sequence_no = bond->sequence_no();
          rep->add_info(info);
          carbon->write_representation(*rep, bond);
          rep->finish_step();
          results->add_up_to_isomorphism(rep);
        }
    }
}

void CarbonAllotrope::all_representations(Representations* results)
{
  p_all_representations_half(results, +1);
  if (s_need_representations_reflection)
    p_all_representations_half(results, -1);
}

void CarbonAllotrope::reset_three_axes()
{
  p_Eigenvalue1 = -1.0;
  p_Eigenvalue2 = -1.0;
  p_Eigenvalue3 = -1.0;
  p_Eigenvector1 = Vector3();
  p_Eigenvector2 = Vector3();
  p_Eigenvector3 = Vector3();
}

void CarbonAllotrope::calculate_three_axes()
{
  if (p_Eigenvalue1 >= 0.0)
    return;
  Statistics st;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      Vector3* loc = new Vector3(carbon->carbon_location());
      st.sample(loc);
    }
  st.average_to_zero();
  Matrix3 vcm = st.variance_covariance_matrix();
  Matrix3::Eigenvalues_and_Eigenvectors(vcm, p_Eigenvalue1, p_Eigenvector1,
                                        p_Eigenvalue2, p_Eigenvector2,
                                        p_Eigenvalue3, p_Eigenvector3);
}

Matrix3 CarbonAllotrope::generate_matrix_of_six_views()
{
  reset_three_axes();
  calculate_three_axes();
  Matrix3 m1 = Matrix3(Quaternion(p_Eigenvector1, Vector3(1.0, 0.0, 0.0)));
  Vector3 new2 = p_Eigenvector2 * m1;
  Matrix3 m2 = Matrix3(Quaternion(p_Eigenvector2 * m1, Vector3(0.0, 1.0, 0.0)));
  return m1 * m2;
}

void CarbonAllotrope::get_range_of_six_views(const Matrix3& mosv,
                                             Vector3& minimum, Vector3& maximum) const
{
  minimum = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
  maximum = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      Vector3 val = carbon->carbon_location() * mosv;
      if (val.x() < minimum.x())
        minimum.x() = val.x();
      if (val.x() > maximum.x())
        maximum.x() = val.x();
      if (val.y() < minimum.y())
        minimum.y() = val.y();
      if (val.y() > maximum.y())
        maximum.y() = val.y();
      if (val.z() < minimum.z())
        minimum.z() = val.z();
      if (val.z() > maximum.z())
        maximum.z() = val.z();
    }
}

int CarbonAllotrope::Euler_characteristic() const
{
  return p_carbons.length() - p_bonds.length() + p_rings.length();
}

void CarbonAllotrope::count_carbons(int& number_of_carbons_with_one_ring,
                                    int& number_of_carbons_with_two_rings,
                                    int& number_of_carbons_with_three_rings) const
{
  number_of_carbons_with_one_ring = 0;
  number_of_carbons_with_two_rings = 0;
  number_of_carbons_with_three_rings = 0;
  int len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      if (carbon->number_of_rings() == 1)
        number_of_carbons_with_one_ring++;
      else if (carbon->number_of_rings() == 2)
        number_of_carbons_with_two_rings++;
      else if (carbon->number_of_rings() == 3)
        number_of_carbons_with_three_rings++;
    }
}

void CarbonAllotrope::print_detail()
{
  printf("* detail ***************************************\n");
  printf("number of bonds = %d\n", number_of_bonds());
  printf("number of carbons = %d\n", number_of_carbons());
  int num1, num2, num3;
  count_carbons(num1, num2, num3);
  printf("number of carbons with one ring = %d\n", num1);
  printf("number of carbons with two rings = %d\n", num2);
  printf("number of carbons with three rings = %d\n", num3);
  int nums[MAX_N + 2];
  for (int i = 0; i < MAX_N + 2; ++i)
    nums[i] = 0;
  int len = p_rings.length();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = p_rings[i];
      int num = ring->number_of_carbons();
      if (num > MAX_N + 1)
        num = MAX_N + 1;
      nums[num]++;
    }
  for (int i = 0; i < MAX_N + 1; ++i)
    if (nums[i])
      printf("number of %d-membered rings = %d\n", i, nums[i]);
  if (nums[MAX_N + 1])
    printf("number of many-membered rings = %d\n", nums[MAX_N + 1]);
  printf("Euler characteristic = %d\n", Euler_characteristic());
  List<Carbon> already;
  int index = 0;
  while (1)
    {
      List<Carbon> boundary;
      list_oldest_connected_boundary(boundary, already);
      len = boundary.length();
      if (len == 0)
        break;
      printf("boundary[%d] =\n", index++);
      printf("  dist ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          int distance = carbon->distance_to_set();
          printf("%c  ", No_to_character(distance));
        }
      printf("\n");
      printf("  carb ");
      for (int i = 0; i < len; ++i)
        printf("C--");
      printf("\n");
      printf("   NO. ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          int no = carbon->sequence_no();
          if (carbon->number_of_rings() == 2)
            {
              if (no < 100)
                printf("|%2d", no);
              else
                printf("%3d", no);
            }
          else
            printf("%3d", no);
        }
      printf("\n");
      printf("  ring ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 2)
            {
              Carbon* carbon_after = boundary[(i + 1) % len];
              Bond* bond_1 = carbon->bond_between(carbon_after);
              Bond* bond_2 = carbon->inner_bond();
              Ring* ring_12 = carbon->ring_between(bond_1, bond_2);
              printf("|%dR", ring_12->number_of_carbons());
            }
          else
            printf("   ");
        }
      printf("\n");
      printf("   NO. ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 2)
            {
              Carbon* carbon_after = boundary[(i + 1) % len];
              Bond* bond_1 = carbon->bond_between(carbon_after);
              Bond* bond_2 = carbon->inner_bond();
              Ring* ring_12 = carbon->ring_between(bond_1, bond_2);
              int no = ring_12->sequence_no();
              if (no < 100)
                printf("|%2d", no);
              else
                printf("%3d", no);
            }
          else
            printf("   ");
        }
      printf("\n");
      printf("  carb ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 2)
            printf("C  ");
          else
            printf("   ");
        }
      printf("\n");
      printf("   NO. ");
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 2)
            {
              Bond* bond = carbon->inner_bond();
              Carbon* carbon2 = bond->get_carbon_beyond(carbon);
              printf("%3d", carbon2->sequence_no());
            }
          else
            printf("   ");
        }
      printf("\n");
      already.add(boundary);
    }
#if defined(CONFIG_SYMMETRIC_GENERATOR_DECIDES_NEXT_FILL_POINT_BY_DISTANCES_TO_PENTAGON)
  printf("distances to pentagons = ");
#else
  printf("distances to center carbons = ");
#endif
  len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      int distance = carbon->distance_to_set();
      printf("%c", No_to_character(distance));
    }
  printf("\n");
  printf("clockwise = ");
  len = p_carbons.length();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      int clockwise = carbon->get_clockwise();
      printf("%c", (clockwise > 0) ? '+' : ((clockwise < 0) ? '-' : 'X'));
    }
  printf("\n");
  printf("************************************************\n");
}

void CarbonAllotrope::print_boundary_representations()
{
  bool print_header_already = false;
  List<Carbon> already;
  while (1)
    {
      List<Carbon> boundary;
      list_oldest_connected_boundary(boundary, already);
      int len = boundary.length();
      if (len == 0)
        break;
      bool has_pentagon = false;
      for (int i = 0; i < len; ++i)
        {
          Carbon* carbon = boundary[i];
          if (carbon->number_of_rings() == 1)
            {
              Ring* ring = carbon->get_ring(0);
              if (ring->number_of_carbons() == 5)
                {
                  has_pentagon = true;
                  break;
                }
            }
        }
      if (has_pentagon == false)
        {
          if (print_header_already == false)
            printf("* boundary_representations =");
          print_header_already = true;
          int edges = 0;
          printf(" ");
          for (int i = 0; i < len; ++i)
            {
              Carbon* carbon = boundary[i];
              if (carbon->number_of_rings() == 2)
                {
                  if (edges > 0)
                    printf("%1d", edges);
                  edges = 1;
                }
              else
                edges++;
            }
          if (edges > 0)
            printf("%1d", edges);
        }
      already.add(boundary);
    }
  if (print_header_already)
    printf("\n");
}

void CarbonAllotrope::print_axes() const
{
  int len = p_axes.length();
  for (int i = 0; i < len; ++i)
    {
      SymmetryAxis* axis = p_axes[i];
      axis->print_out();
    }
}

void CarbonAllotrope::print_axes_summary(FILE* fptr) const
{
  if (has_reflection_symmetricity())
    fprintf(fptr, "Reflection symmetric\n");
  int summary[7][6];
  for (int i = 0; i < 7; ++i)
    for (int j = 0; j < 6; ++j)
      summary[i][j] = 0;
  int len = p_axes.length();
  for (int i = 0; i < len; ++i)
    {
      SymmetryAxis* axis = p_axes[i];
      int order = axis->get_order();
      assert((order > 1) && (order <= 6));
      int type = (int)axis->get_type();
      assert((type >= 1) && (type <= 14));
      summary[order][type]++;
    }
  for (int i = 6; i >= 0; --i)
    {
      int sum = 0;
      for (int j = 0; j < 6; ++j)
        sum += summary[i][j];
      if (sum == 0)
        continue;
      fprintf(fptr, "order(%d)=%d axes (", i, sum);
      bool first = true;
      for (int j = 0; j < 6; ++j)
        if (summary[i][j] > 0)
          {
            if (!first)
              fprintf(fptr, ", ");
            if (j == AXIS_TYPE_CENTER_OF_TWO_CARBONS)
              fprintf(fptr, "Carbons=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_CARBON_AND_BOND)
              fprintf(fptr, "Carbon-Bond=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_CARBON_AND_RING)
              fprintf(fptr, "Carbon-Ring=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_CARBON_AND_BOUNDARY)
              fprintf(fptr, "Carbon-Boundary=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_TWO_BONDS)
              fprintf(fptr, "Bonds=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_BOND_AND_RING)
              fprintf(fptr, "Bond-Ring=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_BOND_AND_BOUNDARY)
              fprintf(fptr, "Bond-Boundary=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_TWO_RINGS)
              fprintf(fptr, "Rings=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_RING_AND_BOUNDARY)
              fprintf(fptr, "Ring-Boundary=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_TWO_BOUNDARIES)
              fprintf(fptr, "Boundaries=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_ONLY_ONE_CARBON)
              fprintf(fptr, "Carbon=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_ONLY_ONE_BOND)
              fprintf(fptr, "Bond=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_ONLY_ONE_RING)
              fprintf(fptr, "Ring=%d", summary[i][j]);
            else if (j == AXIS_TYPE_CENTER_OF_ONLY_ONE_BOUNDARY)
              fprintf(fptr, "Boundary=%d", summary[i][j]);
            first = false;
          }
      fprintf(fptr, ")\n");
    }
}

void CarbonAllotrope::draw_by_POVRay(const MyString& file_name_base,
                                     double delta, int steps, int divisions)
{
  assert(steps > 0);
  assert(divisions > 0);

  register_interactions();
  MyString file_name_base_test("test");
  if (file_name_base.length() > 0)
    file_name_base_test = file_name_base;
  MyString work(file_name_base_test);
  work.append_string(".pov");
  FILE* fptr = fopen((char*)work, "w");

  fprintf(fptr, "#include \"colors.inc\"\n");
  fprintf(fptr, "#include \"stones.inc\"\n");
  fprintf(fptr, "camera {location z*24 angle 40 look_at <0,0,0>}\n");
  fprintf(fptr, "light_source {<50,100,100> color rgb <2,2,2>}\n");
  fprintf(fptr, "background {color rgb <0.3,0.4,1.2>}\n");
  fprintf(fptr, "plane {<0,-1,0>,14 texture {T_Stone3}}\n");
  fprintf(fptr, "#if (clock < 0.5)\n");
  print_POVRay_scene_description(fptr);
  fprintf(fptr, "#end\n");
  for (int time = 1; time < steps; ++time)
    {
      //randomized_force();
      for (int time2 = 0; time2 < divisions; ++time2)
        operate_interactions(delta / divisions);
      fprintf(fptr, "#if ((clock > %d.5)&(clock < %d.5))\n", time - 1, time);
      print_POVRay_scene_description(fptr);
      fprintf(fptr, "#end\n");
      printf("%d/%d\r", time, steps);
      fflush(stdout);
    }
  fclose(fptr);

  fptr = fopen("carbons.ini", "w");
  fprintf(fptr, "-KFI0\n");
  fprintf(fptr, "-KFF%d\n", steps - 1);
  fprintf(fptr, "-KI0.0\n");
  fprintf(fptr, "-KF%d.0\n", steps - 1);
  fprintf(fptr, "%s.pov\n", (char*)file_name_base_test);
  fclose(fptr);

  work = file_name_base_test;
  work.append_string(".ini");
  fptr = fopen((char*)work, "w");
  fprintf(fptr, "-K%d.0\n", steps - 1);
  fprintf(fptr, "-H960\n");
  fprintf(fptr, "-W1280\n");
  fprintf(fptr, "%s.pov\n", (char*)file_name_base_test);
  fclose(fptr);
}

void CarbonAllotrope::OpenGL_to_POVRay(const MyString& file_name_base,
                                       int view, const Quaternion& rotation)
{
  Matrix3 mosv = rotation;
  Vector3 minimum;
  Vector3 maximum;
  get_range_of_six_views(mosv, minimum, maximum);
  MyString work;
  if (file_name_base.length() > 0)
    work = file_name_base;
  else
    {
      work = "C";
      work.append_int(number_of_carbons());
      work.append_string("-test");
    }
  work.append_string("-snapshot");
  MyString pov_name(work);
  pov_name.append_char('-');
  int number = OpenGLUtil::find_unused_file_number(work);
  pov_name.append_int(number);
  pov_name.append_string(".pov");
  FILE* fptr = fopen((char*)pov_name, "w");

  fprintf(fptr, "#include \"colors.inc\"\n");
  fprintf(fptr, "#include \"stones.inc\"\n");
  fprintf(fptr,
          "camera {location z * %d angle 40 look_at <0,0,0> right <-1.33333,0,0>}\n",
          view);
  fprintf(fptr, "light_source {<50,100,100> color rgb <2,2,2>}\n");
#if defined(CONFIG_PAINT_TRANSPARENT_COLOR_TO_BACKGROUND_IN_POVRAY_SNAPSHOT)
  fprintf(fptr, "#if (1)\n");
#else
  fprintf(fptr, "#if (0)\n");
#endif
  fprintf(fptr, "  background {color rgbt <0.0,0.0,0.0,1.0>}\n");
  fprintf(fptr, "#else\n");
  fprintf(fptr, "  background {color rgb <0.3,0.4,1.2>}\n");
  fprintf(fptr, "  plane {<0,1,0>,%g texture {T_Stone3}}\n", minimum.y() * 2.5);
  fprintf(fptr, "#end\n");
  Vector3 min, max;
  // front
  min = minimum;
  max = maximum;
  print_POVRay_scene_description(fptr, mosv, Vector3(-(min.x() +max.x())*0.5,
                                                     -(min.y() +max.y())*0.5,
                                                     -(min.z() +max.z())*0.5),
                                 false);
  fclose(fptr);
  execute_POVRay(pov_name);
}

void CarbonAllotrope::draw_six_views_by_POVRay(const MyString& file_name_base)
{
  Matrix3 mosv = generate_matrix_of_six_views();
  Vector3 minimum;
  Vector3 maximum;
  get_range_of_six_views(mosv, minimum, maximum);

  MyString work;
  if (file_name_base.length() > 0)
    work = file_name_base;
  else
    {
      work = "C";
      work.append_int(number_of_carbons());
      work.append_string("-test");
    }
  work.append_string("-six-view");
  MyString pov_name(work);
  pov_name.append_char('-');
  int number = OpenGLUtil::find_unused_file_number(work);
  pov_name.append_int(number);
  pov_name.append_string(".pov");
  FILE* fptr = fopen((char*)pov_name, "w");

  fprintf(fptr, "#include \"colors.inc\"\n");
  fprintf(fptr, "#include \"stones.inc\"\n");
  fprintf(fptr,
          "camera {location z * 100.0 angle 40 look_at <0,0,0> right <-1.33333,0,0>}\n");
  fprintf(fptr, "background {color rgb <0.3,0.4,1.2>}\n");
  fprintf(fptr, "light_source {<50,100,100> color rgb <2,2,2>}\n");
#if 0
  fprintf(fptr, "plane {<0,-1,0>,30 texture {T_Stone3}}\n");
#endif
  Matrix3 rot;
  Vector3 min, max;
  // front
  min = minimum;
  max = maximum;
  print_POVRay_scene_description(fptr, mosv, Vector3(-(min.x() +max.x())*0.5,
                                                     -max.y(),
                                                     -(min.z() +max.z())*0.5),
                                 true);
  // upward
  rot = Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Vector3(0.0, -1.0, 0.0)));
  min = minimum * rot;
  max = maximum * rot;
  print_POVRay_scene_description(fptr, mosv * rot, Vector3(-(min.x() +max.x())*0.5,
                                                           -max.y() +1.0,
                                                           -(min.z() +max.z())*0.5),
                                 true);
  // back
  rot = Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 1.0, 0.0)));
  Matrix3 rot2 = Matrix3(Quaternion(Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, -1.0)));
  rot = rot * rot2;
  min = minimum * rot;
  max = maximum * rot;
  print_POVRay_scene_description(fptr, mosv * rot,
                                 Vector3(-(min.x() +max.x())*0.5,
                                         maximum.z() -minimum.z() -max.y() +2.0,
                                         -(min.z() +max.z())*0.5),
                                 true);
  // downward
  rot = Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 1.0, 0.0)));
  min = minimum * rot;
  max = maximum * rot;
  print_POVRay_scene_description(fptr, mosv * rot,
                                 Vector3(-(min.x() +max.x())*0.5,
                                         minimum.y() -maximum.y() -max.y() -1.0,
                                         -(min.z() +max.z())*0.5),
                                 true);
  // leftside
  rot = Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Vector3(1.0, 0.0, 0.0)));
  min = minimum * rot;
  max = maximum * rot;
  print_POVRay_scene_description(fptr, mosv * rot,
                                 Vector3(-(maximum.x() -minimum.x())*0.5 -max.x() -1.0,
                                         -max.y(),
                                         -(min.z() +max.z())*0.5),
                                 true);
  // rightside
  rot = Matrix3(Quaternion(Vector3(0.0, 0.0, 1.0), Vector3(-1.0, 0.0, 0.0)));
  min = minimum * rot;
  max = maximum * rot;
  print_POVRay_scene_description(fptr, mosv * rot,
                                 Vector3((maximum.x() -minimum.x())*0.5 -max.x() +1.0,
                                         -max.y(),
                                         -(min.z() +max.z())*0.5),
                                 true);
  fclose(fptr);
  execute_POVRay(pov_name);
}

void CarbonAllotrope::execute_POVRay(const MyString& pov_name)
{
  const char* format = configuration->get_povray_command_line();
  char command_line[1024];
  snprintf(command_line, 1024, format, (char*)pov_name);
  int result = system(command_line);
  if (result != 0)
    {
      MyString message(__FILE__);
      message.append_char(':');
      message.append_int(__LINE__);
      message.append_string(": execution error `");
      message.append_string(command_line);
      message.append_string("'");
      if (OpenGLUtil::alert_dialog_callback)
        (*OpenGLUtil::alert_dialog_callback)((char*)message);
      else
        fprintf(stderr, "%s\n", (char*)message);
    }
}

void CarbonAllotrope::force_to_circle(List<Carbon>& cutend_list)
{
  int count = cutend_list.length();
  if (count > 0)
    {
      /* 周辺(cutend_list) からの距離を計算する。*/
      calculate_distances_to_set(cutend_list);
      /* 一番遠かったのが、中心。*/
      {
        int center_distance = p_maximum_distance_to_set();
        List<Carbon> centers;
        int len = number_of_carbons();
        for (int i = 0; i < len; ++i)
          {
            Carbon* each = get_carbon(i);
            if (each->distance_to_set() == center_distance)
              centers.add(each);
          }
        /* 逆に中心からの距離を計算する。*/
        calculate_distances_to_set(centers);
      }
      /* 周辺(cutend_list) の Carbon を、中心からの距離に応じて配置する。*/
      int cutend_max = p_maximum_distance_to_set();
      double radius = sqrt(number_of_carbons()) * 1.42;
      if (print_out_sequence_no)
        radius *= 1.05;
      double radius_delta = 0.8 / (cutend_max + 1);
      for (int i = 0; i < count; ++i)
        {
          Carbon* each = cutend_list[i];
          int dist = each->distance_to_set();
          double each_radius = radius * (1.0 - radius_delta * (cutend_max - dist));
          double x = -each_radius * sin(i * PAI * 2.0 / count);
          double y = each_radius * cos(i * PAI * 2.0 / count);
          each->fix_center_location(Vector3(x, y, 0.0));
        }
    }
}

void CarbonAllotrope::
draw_force_to_circle_by_POVRay(const MyString& file_name_base, List<Carbon>& cutend_list,
                               double delta, int steps, int divisions)
{
  force_to_circle(cutend_list);
  draw_by_POVRay(file_name_base, delta, steps, divisions);
}

void CarbonAllotrope::
draw_force_to_circle_by_POVRay(const MyString& file_name_base,
                               double delta, int steps, int divisions)
{
  List<Carbon> cutend_list;
  list_oldest_connected_boundary(cutend_list);
  force_to_circle(cutend_list);
  draw_by_POVRay(file_name_base, delta, steps, divisions);
}

void CarbonAllotrope::
draw_development_view_by_POVRay(const MyString& file_name_base, Ring* cutend_ring,
                                double delta, int steps, int divisions)
{
  List<Carbon> cutend_list;
  int count = cutend_ring->number_of_carbons();
  for (int i = 0; i < count; ++i)
    cutend_list.add(cutend_ring->get_carbon(i));
  remove_ring(cutend_ring);
  force_to_circle(cutend_list);
  draw_by_POVRay(file_name_base, delta, steps, divisions);
}

void CarbonAllotrope::print_POVRay_scene_description(FILE* fptr) const
{
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    p_carbons[i]->print_POVRay_scene_description(this, fptr);
  len = number_of_bonds();
  for (int i = 0; i < len; ++i)
    p_bonds[i]->print_POVRay_scene_description(this, fptr);
  len = number_of_rings();
  for (int i = 0; i < len; ++i)
    p_rings[i]->print_POVRay_scene_description(this, fptr);
}

void CarbonAllotrope::print_POVRay_scene_description(FILE* fptr, const Matrix3& rot,
                                                     const Vector3& move,
                                                     bool clipped_by_Z_non_negative) const
{
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    p_carbons[i]->print_POVRay_scene_description(this, fptr, rot, move,
                                                 clipped_by_Z_non_negative);
  len = number_of_bonds();
  for (int i = 0; i < len; ++i)
    p_bonds[i]->print_POVRay_scene_description(this, fptr, rot, move,
                                               clipped_by_Z_non_negative);
  len = number_of_rings();
  for (int i = 0; i < len; ++i)
    p_rings[i]->print_POVRay_scene_description(this, fptr, rot, move,
                                               clipped_by_Z_non_negative);

}

void CarbonAllotrope::memory_shape(const MyString& file_name_base) const
{
  MyString file_name("C");
  file_name.append_int(number_of_carbons());
  file_name.append_char('-');
  file_name.append_string(file_name_base);
  file_name.append_string(".shape");
  FILE* fptr = fopen((char*)file_name, "w");
  if (!fptr)
    return;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = get_carbon(i);
      fprintf(fptr, "%d\n", carbon->sequence_no());
      carbon->memory_shape(fptr);
    }
  fclose(fptr);
}

void CarbonAllotrope::recall_shape(const MyString& file_name_base)
{
  MyString file_name("C");
  file_name.append_int(number_of_carbons());
  file_name.append_char('-');
  file_name.append_string(file_name_base);
  file_name.append_string(".shape");
  FILE* fptr = fopen((char*)file_name, "r");
  if (!fptr)
    return;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      int no;
      fscanf(fptr, "%d", &no);
      Carbon* carbon = get_carbon_by_sequence_no(no);
      carbon->recall_shape(fptr);
    }
  fclose(fptr);
  resume_simulation();
}

void CarbonAllotrope::register_ring(Ring* ring)
{
  ring->set_clockwise_locally();
  p_rings.add(ring);
}

void CarbonAllotrope::remove_ring(Ring* ring)
{
  p_rings.remove(ring);
  unregister_interactive(ring);
}

Ring* CarbonAllotrope::get_ring(int index) const
{
  assert((index >= 0) && (index < number_of_rings()));
  return p_rings[index];
}

Ring* CarbonAllotrope::get_ring_by_sequence_no(int sequence_no) const
{
  int len = number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = p_rings[i];
      if (ring->sequence_no() == sequence_no)
        return ring;
    }
  return 0;
}

void CarbonAllotrope::remove_the_far_most_ring(const Quaternion& rotation)
{
  Matrix3 rotmat = rotation;
  int len = number_of_rings();
  int the_far_most_ring = -1;
  double the_far_most_center = 0.0;
  int i = -1;
  for (i = 0; i < len; ++i)
    {
      Vector3 center = p_rings[i]->get_center_location();
      center *= rotmat;
      if (center.z() < the_far_most_center)
        {
          the_far_most_ring = i;
          the_far_most_center = center.z();
        }
    }
  if (the_far_most_ring != -1)
    {
      List<Carbon> cutend_list;
      Ring* ring = p_rings[the_far_most_ring];
      int count = ring->number_of_carbons();
      for (int i = 0; i < count; ++i)
        cutend_list.add(ring->get_carbon(i));
      remove_ring(ring);
      force_to_circle(cutend_list);
      resume_simulation();
    }
}

void CarbonAllotrope::register_carbon(Carbon* carbon)
{
  p_carbons.add(carbon);
}

Carbon* CarbonAllotrope::get_carbon(int index) const
{
  assert((index >= 0) && (index < number_of_carbons()));
  return p_carbons[index];
}

Carbon* CarbonAllotrope::get_carbon_by_sequence_no(int sequence_no) const
{
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    {
      Carbon* carbon = p_carbons[i];
      if (carbon->sequence_no() == sequence_no)
        return carbon;
    }
  return 0;
}

void CarbonAllotrope::register_bond(Bond* bond)
{
  p_bonds.add(bond);
}

Bond* CarbonAllotrope::get_bond(int index) const
{
  assert((index >= 0) && (index < number_of_bonds()));
  return p_bonds[index];
}

Bond* CarbonAllotrope::get_bond_by_sequence_no(int sequence_no) const
{
  int len = number_of_bonds();
  for (int i = 0; i < len; ++i)
    {
      Bond* bond = p_bonds[i];
      if (bond->sequence_no() == sequence_no)
        return bond;
    }
  return 0;
}

int CarbonAllotrope::list_oldest_connected_boundary(List<Carbon>& result) const
{
  Carbon* start = 0;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    if (p_carbons[i]->number_of_rings() <= 2)
      {
        start = p_carbons[i];
        break;
      }
  if (!start)
    return 0;
  Carbon* carbon = start;
  while (1)
    {
      result.add(carbon);
      Bond* bond = carbon->boundary_bond();
      carbon = bond->get_carbon_beyond(carbon);
      if (carbon == start)
        return result.length();
    }
}

int CarbonAllotrope::list_oldest_connected_boundary_carbons(BoundaryCarbons& result) const
{
  Carbon* start = 0;
  int len = number_of_carbons();
  for (int i = 0; i < len; ++i)
    if (p_carbons[i]->number_of_rings() <= 2)
      {
        start = p_carbons[i];
        break;
      }
  if (!start)
    return 0;
  Carbon* carbon = start;
  while (1)
    {
      result.add(carbon);
      Bond* bond = carbon->boundary_bond();
      carbon = bond->get_carbon_beyond(carbon);
      if (carbon == start)
        return result.length();
    }
}

int CarbonAllotrope::list_newborn_connected_boundary(List<Carbon>& result) const
{
  Carbon* start = 0;
  int len = number_of_carbons();
  for (int i = len - 1; i >= 0; --i)
    if (p_carbons[i]->number_of_rings() <= 2)
      {
        start = p_carbons[i];
        break;
      }
  if (!start)
    return 0;
  Carbon* carbon = start;
  while (1)
    {
      result.add(carbon);
      Bond* bond = carbon->boundary_bond();
      carbon = bond->get_carbon_beyond(carbon);
      if (carbon == start)
        return result.length();
    }
}

int CarbonAllotrope::list_oldest_connected_boundary(List<Carbon>& result,
                                                    const List<Carbon>& ignores) const
{
  Carbon* start = 0;
  int len = number_of_carbons();
  int ignlen = ignores.length();
  for (int i = 0; i < len; ++i)
    if (p_carbons[i]->number_of_rings() <= 2)
      {
        start = p_carbons[i];
        for (int j = 0; j < ignlen; ++j)
          {
            if (start == ignores[j])
              {
                start = 0;
                break;
              }
          }
        if (start)
          break;
      }
  if (!start)
    return 0;
  Carbon* carbon = start;
  while (1)
    {
      result.add(carbon);
      Bond* bond = carbon->boundary_bond();
      carbon = bond->get_carbon_beyond(carbon);
      if (carbon == start)
        return result.length();
    }
}

void CarbonAllotrope::all_boundaries()
{
  List<Carbon> already;
  while (1)
    {
      List<Carbon> boundary;
      list_oldest_connected_boundary(boundary, already);
      if (boundary.length() == 0)
        return;
      new ConnectedBoundary(this, boundary);
      already.add(boundary);
    }
}

void CarbonAllotrope::register_boundary(ConnectedBoundary* boundary)
{
  p_boundaries.add(boundary);
}

ConnectedBoundary* CarbonAllotrope::get_boundary(int index) const
{
  assert((index >= 0) && (index < number_of_boundaries()));
  return p_boundaries[index];
}

ConnectedBoundary* CarbonAllotrope::get_boundary_by_sequence_no(int sequence_no) const
{
  int len = number_of_boundaries();
  for (int i = 0; i < len; ++i)
    {
      ConnectedBoundary* boudary = p_boundaries[i];
      if (boudary->sequence_no() == sequence_no)
        return boudary;
    }
  return 0;
}

SymmetryAxis* CarbonAllotrope::get_axis(int index) const
{
  assert((index >= 0) && (index < number_of_axes()));
  return p_axes[index];
}

void CarbonAllotrope::get_major_axes(List<SymmetryAxis>& result) const
{
  int len = number_of_axes();
  int max_order = 0;
  for (int i = 0; i < len; ++i)
    {
      SymmetryAxis* axis = get_axis(i);
      int order = axis->get_order();
      if (order > max_order)
        max_order = order;
    }
  for (int i = 0; i < len; ++i)
    {
      SymmetryAxis* axis = get_axis(i);
      int order = axis->get_order();
      if (order == max_order)
        result.add(axis);
    }
}

void CarbonAllotrope::register_axis(SymmetryAxis* axis)
{
  int len = number_of_axes();
  for (int i = 0; i < len; ++i)
    {
      SymmetryAxis* work = p_axes[i];
      if ((*work) != (*axis))
        continue;
      AxisType type = work->get_type();
      if (type == AXIS_TYPE_CENTER_OF_TWO_RINGS)
        {
          int work_generator_step = work->get_generator_step();
          int axis_generator_step = axis->get_generator_step();
          if (work_generator_step <= axis_generator_step)
            {
              delete axis;
              return;
            }
          work->set_order(axis->get_order());
          work->set_generator(axis->get_generator(), axis_generator_step);
          delete axis;
          return;
        }
      else
        {
          int work_order = work->get_order();
          int axis_order = axis->get_order();
          if (work_order >= axis_order)
            {
              delete axis;
              return;
            }
          work->set_order(axis_order);
          work->set_generator(axis->get_generator(), axis->get_generator_step());
          delete axis;
          return;
        }
    }
  p_axes.add(axis);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
