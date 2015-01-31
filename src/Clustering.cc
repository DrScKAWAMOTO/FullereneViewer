/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <limits.h>
#include <assert.h>
#include "Clustering.h"
#include "CarbonAllotrope.h"
#include "Utils.h"

void Clustering::p_initialize()
{
  p_number = 0;
  for (int i = 0; i < 12; ++i)
    {
      p_sequence_nos[i] = -1;
      for (int j = 0; j < 12; ++j)
        p_distances[i][j] = -1;
    }
}

int Clustering::p_seq_no_to_index(int seq_no)
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

void Clustering::p_print_as_table(FILE* fptr) const
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

void Clustering::p_print_as_line(FILE* fptr) const
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

void Clustering::p_sort(int i, int j)
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

int Clustering::p_select_minimum(int top)
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

void Clustering::p_swap(int i1, int i2)
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

Clustering::Clustering()
{
  p_initialize();
}

Clustering::Clustering(CarbonAllotrope* ca)
{
  p_initialize();
  int len = ca->number_of_rings();
  for (int i = 0; i < len; ++i)
    {
      Ring* target = ca->get_ring(i);
      if (target->number_of_carbons() != 5)
        continue;
      p_calculate_distances_to_a_ring(ca, target);
    }
}

void Clustering::p_calculate_distances_to_a_ring(CarbonAllotrope* ca, Ring* target)
{
  int len = ca->number_of_rings();
  int target_seq_no = target->sequence_no();
  int distances[len];
  for (int i = 0; i < len; ++i)
    distances[i] = INT_MAX;
  Queue<Ring> operations;
  distances[target_seq_no - 1] = 0;
  set_distance(target_seq_no, target_seq_no, 0);
  operations.enqueue(target);
  while (1)
    {
      Ring* next = operations.dequeue();
      if (next == 0)
        return;
      int next_seq_no = next->sequence_no();
      int next_distance = distances[next_seq_no - 1];
      int num_c = next->number_of_carbons();
      for (int j = 0; j < num_c; ++j)
        {
          Carbon* carbon = next->get_carbon(j);
          int num_r = carbon->number_of_rings();
          for (int k = 0; k < num_r; ++k)
            {
              Ring* ring = carbon->get_ring(k);
              int ring_seq_no = ring->sequence_no();
              if (distances[ring_seq_no - 1] == INT_MAX)
                {
                  int distance = next_distance + 1;
                  distances[ring_seq_no - 1] = distance;
                  if (ring->number_of_carbons() == 5)
                    set_distance(target_seq_no, ring_seq_no, distance);
                  operations.enqueue(ring);
                }
            }
        }
    }
}

Clustering::Clustering(const Clustering& you)
  : Object(you), p_number(you.p_number)
{
  for (int i = 0; i < 12; ++i)
    {
      p_sequence_nos[i] = you.p_sequence_nos[i];
      for (int j = 0; j < 12; ++j)
        p_distances[i][j] = you.p_distances[i][j];
    }
}

Clustering::Clustering(const Clustering& you, int* group, int no)
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
          set_distance(you.p_sequence_nos[i], you.p_sequence_nos[j],
                       you.p_distances[i][j]);
        }
    }
}

Clustering::~Clustering()
{
}

Clustering& Clustering::operator = (const Clustering& you)
{
  if (this != &you)
    {
      p_number = you.p_number;
      for (int i = 0; i < 12; ++i)
        {
          p_sequence_nos[i] = you.p_sequence_nos[i];
          for (int j = 0; j < 12; ++j)
            p_distances[i][j] = you.p_distances[i][j];
        }
    }
  return *this;
}

void Clustering::normalize()
{
  for (int top = 0; top < p_number; ++top)
    {
      Clustering copy = *this;
      int sel = copy.p_select_minimum(top);
      p_swap(top, sel);
    }
}

void Clustering::print_as_table(FILE* fptr) const
{
  int count_down = 12;
  fprintf(fptr, "Clustering to 12 clusters (threshold=0) is trivial, so omitted.\n");
  for (int md = 1; 1; ++md)
    {
      int clusters[12];
      int count = clustering(md, clusters);
      if (count < count_down)
        {
          if (count == 1)
            fprintf(fptr, "Clustering to one cluster (threshold=%d),\n", md);
          else
            fprintf(fptr, "Clustering to %d clusters (threshold=%d),\n", count, md);
          for (int i = 0; i < count; ++i)
            {
              Clustering dm = Clustering(*this, clusters, i);
              dm.normalize();
              dm.p_print_as_table(fptr);
            }
          count_down = count;
        }
      if (count <= 1)
        return;
    }
}

void Clustering::print_as_line(FILE* fptr) const
{
  int count_down = 12;
  fprintf(fptr, "Clustering to 12 clusters (threshold=0) is trivial, so omitted.\n");
  for (int md = 1; 1; ++md)
    {
      int clusters[12];
      int count = clustering(md, clusters);
      if (count < count_down)
        {
          if (count == 1)
            fprintf(fptr, "Clustering to one cluster (threshold=%d),\n", md);
          else
            fprintf(fptr, "Clustering to %d clusters (threshold=%d),\n", count, md);
          for (int i = 0; i < count; ++i)
            {
              Clustering dm = Clustering(*this, clusters, i);
              dm.normalize();
              dm.p_print_as_line(fptr);
            }
          count_down = count;
        }
      if (count <= 1)
        return;
    }
}

void Clustering::set_distance(int seq_no1, int seq_no2, int distance)
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

int Clustering::get_distance(int seq_no1, int seq_no2)
{
  int index1 = p_seq_no_to_index(seq_no1);
  int index2 = p_seq_no_to_index(seq_no2);
  assert(p_distances[index1][index2] != -1);
  return p_distances[index1][index2];
}

int Clustering::clustering(int threshold, int* clusters) const
{
  int count = 0;
  for (int i = 0; i < 12; ++i)
    clusters[i] = -1;
  while (1)
    {
      for (int i = 0; i < 12; ++i)
        if (clusters[i] == -1)
          {
            clusters[i] = count;
            goto lets_go;
          }
      return count;
    lets_go:
      while (1)
        {
          for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 12; ++j)
              {
                if ((p_distances[i][j] <= threshold) &&
                    (clusters[i] == count) && (clusters[j] == -1))
                  {
                    clusters[j] = count;
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
