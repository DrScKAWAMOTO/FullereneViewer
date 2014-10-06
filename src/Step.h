/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/18 00:37:23 JST
 */

#ifndef __STEP_H__
#define __STEP_H__

#include <limits.h>
#include <pthread.h>
#include "Generator.h"
#include "BoundaryCarbons.h"
#include "Object.h"

class Fullerene;
class CarbonAllotrope;

class Step : public Object {
  // friend classes & functions

  // members
private:
  static int s_progress_level;
  static pthread_mutex_t s_mutex;
  bool p_symmetric;
  int p_close;
  bool p_is_tube;
  int p_maximum_number_of_carbons;
  Generator* p_gen;
  CarbonAllotrope* p_ca;
  BoundaryCarbons p_boundary;
  int p_number_of_rest_pentagons;
  bool p_all_pentagons;
  int p_close_count;
  int p_ring_next_sequence;
  int p_carbon_next_sequence;
  int p_bond_next_sequence;

  // private tools
private:
  Step* p_copy(CarbonAllotrope* ca) const;

  // constructors & the destructor
public:
  Step();
  Step(Generator& gen, int maximum_number_of_carbons,
       bool symmetric = true, int close = INT_MAX);
  Step* shallow_copy() const;
  Step* deep_copy() const;
  ~Step();

  // step operation
  bool construction_step(Fullerene*& fullerene);
  bool is_there_next_branch() const;
  void next_branch();
  void rollback();

  // progress level
public:
  static void initiate_progress_level(int level);
  static int get_progress_level();
  static void set_progress_level(int level);

};

#endif /* __STEP_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
