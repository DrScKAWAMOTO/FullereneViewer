/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __FULLERENES_H__
#define __FULLERENES_H__

#include <limits.h>
#include "Set.h"
#include "MinimumRepresentation.h"
#include "Stack.h"
#include "Step.h"

enum StepAlgorithm {
  STEP_ALGORITHM_COPY_BRANCH = 1,
  STEP_ALGORITHM_FORWARD = 2,
  STEP_ALGORITHM_BACKWARD = 3
};

class Fullerenes {
  // friend classes & functions

  // members
private:
  Set<MinimumRepresentation> p_patterns;

  // private tools
  void p_step_copy_branch(Generator& gen, int maximum_number_of_carbons,
                          bool symmetric, int close, bool shallow_copy);
  void p_step_forward(Generator& gen, int maximum_number_of_carbons,
                      bool symmetric, int close);
  void p_step_ei_yah(const char* generator_formula, int maximum_number_of_carbons,
                     bool symmetric, int maximum_vertices_of_polygons, int close,
                     StepAlgorithm step_algorithm = STEP_ALGORITHM_BACKWARD);

  // constructors & the destructor
public:
  Fullerenes(const char* generator_formula, int maximum_number_of_carbons,
             bool symmetric = true, int maximum_vertices_of_polygons = 6,
             int close = INT_MAX,
             StepAlgorithm step_algorithm = STEP_ALGORITHM_BACKWARD);
  ~Fullerenes();

  // type converters

  // comparators

  // math operators

  // I/O

  // class decision

  // member accessing methods
public:
  int number_of_patterns() const;
  const Fullerene* get_pattern(int index) const;
  void add_fullerene(Fullerene* pat);

};

#endif /* __FULLERENES_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
