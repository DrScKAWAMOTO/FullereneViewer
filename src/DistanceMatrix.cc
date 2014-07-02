/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 07:28:45 JST
 */

#include <limits.h>
#include <assert.h>
#include "DistanceMatrix.h"
#include "CarbonAllotrope.h"
#include "Utils.h"

void DistanceMatrix::p_initialize()
{
  p_number = 0;
  for (int i = 0; i < 12; ++i)
    {
      p_sequence_nos[i] = -1;
      for (int j = 0; j < 12; ++j)
        p_distances[i][j] = -1;
    }
}

int DistanceMatrix::p_seq_no_to_index(int seq_no)
{
  int i;
  for (i = 0; i < p_number; ++i)
    if (p_sequence_nos[i] == seq_no)
      return i;
  if (p_number < 12)
    {
      p_sequence_nos[p_number] = seq_no;
      return p_number++;
    }
  return -1;
}

void DistanceMatrix::p_print_as_table(FILE* fptr) const
{
  for (int i = 0; i < p_number; ++i)
    fprintf(fptr, "+---");
  fprintf(fptr, "+---+\n|   ");
  for (int i = 0; i < p_number; ++i)
    fprintf(fptr, "|%3d", p_sequence_nos[i]);
  fprintf(fptr, "|\n");
  for (int i = 0; i < p_number; ++i)
    fprintf(fptr, "+---");
  fprintf(fptr, "+---+\n");
  for (int i = 0; i < p_number; ++i)
    {
      fprintf(fptr, "|%3d", p_sequence_nos[i]);
      for (int j = 0; j < p_number; ++j)
        fprintf(fptr, "|%3d", p_distances[i][j]);
      fprintf(fptr, "|\n");
    }
  for (int i = 0; i < p_number; ++i)
    fprintf(fptr, "+---");
  fprintf(fptr, "+---+\n");
}

void DistanceMatrix::p_print_as_line(FILE* fptr) const
{
  for (int i = 0; i < p_number; ++i)
    {
      if (i != 0)
        fprintf(fptr, "-");
      for (int j = 0; j < p_number; ++j)
        fprintf(fptr, "%c", No_to_character(p_distances[i][j]));
    }
  fprintf(fptr, "\n");
}

void DistanceMatrix::p_sort(int i, int j)
{
  while (j < p_number)
    {
      int min_offset = -1;
      int min_value = INT_MAX;
      for (int k = j; k < p_number; ++k)
        {
          if (min_value > p_distances[i][k])
            {
              min_offset = k;
              min_value = p_distances[i][k];
            }
        }
      if (min_offset != j)
        {
          p_distances[i][min_offset] = p_distances[i][j];
          p_distances[i][j] = min_value;
        }
      ++j;
    }
}

int DistanceMatrix::p_select_minimum(int top)
{
  for (int i = top; i < p_number; ++i)
    p_sort(i, top);
  int i1 = top;
  for (int i2 = top + 1; i2 < p_number; ++i2)
    {
      for (int j = 0; j < p_number; ++j)
        {
          if (p_distances[i1][j] < p_distances[i2][j])
            break;
          if (p_distances[i1][j] > p_distances[i2][j])
            {
              i1 = i2;
              break;
            }
        }
    }
  return i1;
}

void DistanceMatrix::p_swap(int i1, int i2)
{
  int work = p_sequence_nos[i1];
  p_sequence_nos[i1] = p_sequence_nos[i2];
  p_sequence_nos[i2] = work;
  for (int i = 0; i < p_number; ++i)
    {
      int work = p_distances[i][i1];
      p_distances[i][i1] = p_distances[i][i2];
      p_distances[i][i2] = work;
    }
  for (int i = 0; i < p_number; ++i)
    {
      int work = p_distances[i1][i];
      p_distances[i1][i] = p_distances[i2][i];
      p_distances[i2][i] = work;
    }
}

DistanceMatrix::DistanceMatrix()
{
  p_initialize();
}

DistanceMatrix::DistanceMatrix(CarbonAllotrope* ca)
{
  p_initialize();
  int len = ca->number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      Ring* ring = ca->get_ring(i);
      if (ring->number_of_carbons() != 5)
        continue;
      List<Carbon> set;
      for (int j = 0; j < 5; ++j)
        set.add(ring->get_carbon(j));
      ca->reset_distances_to_set();
      ca->calculate_distances_to_set(set);
      for (int j = 0; j < len; ++j)
        {
          Ring* ring2 = ca->get_ring(j);
          if (ring2->number_of_carbons() != 5)
            continue;
          int min_dist = INT_MAX;
          for (int k = 0; k < 5; ++k)
            {
              Carbon* carbon = ring2->get_carbon(k);
              int dist = carbon->distance_to_set();
              if (dist < min_dist)
                min_dist = dist;
            }
          set_distance(ring->sequence_no(), ring2->sequence_no(), min_dist);
        }
    }
}

DistanceMatrix::DistanceMatrix(const DistanceMatrix& that)
  : Object(that), p_number(that.p_number)
{
  for (int i = 0; i < 12; ++i)
    {
      p_sequence_nos[i] = that.p_sequence_nos[i];
      for (int j = 0; j < 12; ++j)
        p_distances[i][j] = that.p_distances[i][j];
    }
}

DistanceMatrix::DistanceMatrix(const DistanceMatrix& that, int* group, int no)
{
  p_initialize();
  for (int i = 0; i < 12; ++i)
    {
      if (group[i] != no)
        continue;
      for (int j = 0; j < 12; ++j)
        {
          if (group[j] != no)
            continue;
          set_distance(that.p_sequence_nos[i], that.p_sequence_nos[j],
                       that.p_distances[i][j]);
        }
    }
}

DistanceMatrix::~DistanceMatrix()
{
}

DistanceMatrix& DistanceMatrix::operator = (const DistanceMatrix& that)
{
  if (this != &that)
    {
      p_number = that.p_number;
      for (int i = 0; i < 12; ++i)
        {
          p_sequence_nos[i] = that.p_sequence_nos[i];
          for (int j = 0; j < 12; ++j)
            p_distances[i][j] = that.p_distances[i][j];
        }
    }
  return *this;
}

void DistanceMatrix::normalize()
{
  for (int top = 0; top < p_number; ++top)
    {
      DistanceMatrix copy = *this;
      int sel = copy.p_select_minimum(top);
      p_swap(top, sel);
    }
}

void DistanceMatrix::print_as_table(FILE* fptr) const
{
  p_print_as_table(fptr);
  for (int md = 1; 1; ++md)
    {
      int group[12];
      int count = grouping(md, group);
      if (count <= 1)
        return;
      if (count < 12)
        {
          for (int i = 0; i < count; ++i)
            {
              DistanceMatrix dm = DistanceMatrix(*this, group, i);
              dm.normalize();
              dm.p_print_as_table(fptr);
            }
          return;
        }
    }
}

void DistanceMatrix::print_as_line(FILE* fptr) const
{
  p_print_as_line(fptr);
  for (int md = 1; 1; ++md)
    {
      int group[12];
      int count = grouping(md, group);
      if (count <= 1)
        return;
      if (count < 12)
        {
          for (int i = 0; i < count; ++i)
            {
              DistanceMatrix dm = DistanceMatrix(*this, group, i);
              dm.normalize();
              dm.p_print_as_line(fptr);
            }
          return;
        }
    }
}

void DistanceMatrix::set_distance(int seq_no1, int seq_no2, int distance)
{
  int index1 = p_seq_no_to_index(seq_no1);
  int index2 = p_seq_no_to_index(seq_no2);
  if (index1 > index2)
    {
      int tmp = index1;
      index1 = index2;
      index2 = tmp;
    }
  if (p_distances[index1][index2] != -1)
    assert(p_distances[index1][index2] == distance);
  p_distances[index1][index2] = distance;
  p_distances[index2][index1] = distance;
}

int DistanceMatrix::get_distance(int seq_no1, int seq_no2)
{
  int index1 = p_seq_no_to_index(seq_no1);
  int index2 = p_seq_no_to_index(seq_no2);
  assert(p_distances[index1][index2] != -1);
  return p_distances[index1][index2];
}

int DistanceMatrix::grouping(int max_distance, int* group) const
{
  int count = 0;
  for (int i = 0; i < 12; ++i)
    group[i] = -1;
  while (1)
    {
      for (int i = 0; i < 12; ++i)
        if (group[i] == -1)
          {
            group[i] = count;
            goto lets_go;
          }
      return count;
    lets_go:
      while (1)
        {
          for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 12; ++j)
              {
                if ((p_distances[i][j] <= max_distance) &&
                    (group[i] == count) && (group[j] == -1))
                  {
                    group[j] = count;
                    goto found;
                  }
              }
          break;
        found:
          continue;
        }
      ++count;
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
