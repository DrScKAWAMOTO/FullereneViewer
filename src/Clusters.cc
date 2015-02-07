/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "Clusters.h"
#include "Clustering.h"
#include "CarbonAllotrope.h"

Clusters::Clusters(CarbonAllotrope* ca)
{
  Clustering cl(ca);
  for (int md = 1; 1; ++md)
    {
      int clusters[12];
      int count = cl.clustering(md, clusters);
      if (count < 12)
        {
          int other_pentagon = count;
          int others = count + 1;
          if (others >= 12)
            others -= 12;
          int num = ca->number_of_rings();
          for (int i = 0; i < num; ++i)
            {
              Ring* ring = ca->get_ring(i);
              int sequence_no = ring->sequence_no();
              int num_of_carbons = ring->number_of_carbons();
              if (num_of_carbons == 6)
                continue;
              else if (num_of_carbons > 6)
                ring->set_pentagon_cellophane_color_by_index(others);
              else
                {
                  int j = 0;
                  while (1)
                    {
                      if (cl.p_sequence_nos[j] == sequence_no)
                        {
                          ring->set_pentagon_cellophane_color_by_index(clusters[j]);
                          break;
                        }
                      if (++j == 12)
                        {
                          ring->set_pentagon_cellophane_color_by_index(other_pentagon);
                          break;
                        }
                    }
                }
            }
          break;
        }
    }
}

Clusters::~Clusters()
{
}

void Clusters::print(FILE* fptr) const
{
  for (int i = 0; i < 12; ++i)
    {
      fprintf(fptr, "(%d,%d)", p_sequence_nos[i], p_cluster_ids[i]);
      if (i == 11)
        fprintf(fptr, "\n");
      else
        fprintf(fptr, ", ");
    }
}

int Clusters::get_cluster_id(int sequence_no)
{
  for (int i = 0; i < 12; ++i)
    {
      if (p_sequence_nos[i] == sequence_no)
        return p_cluster_ids[i];
    }
  return -1;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
