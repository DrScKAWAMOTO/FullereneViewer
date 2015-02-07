/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __CLUSTERS_H__
#define __CLUSTERS_H__

#include <stdio.h>

class CarbonAllotrope;

class Clusters {
  // friend classes & functions

  // members
private:
  int p_sequence_nos[12];
  int p_cluster_ids[12];

  // private tools

  // constructors & the destructor
public:
  Clusters(CarbonAllotrope* ca);
  ~Clusters();

  // I/O
public:
  void print(FILE* fptr) const;

  // member accessing methods
public:
  int get_cluster_id(int sequence_no);

};

#endif /* __CLUSTERS_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
