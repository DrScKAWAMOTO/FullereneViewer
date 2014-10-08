/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "MinimumRepresentation.h"
#include "Fullerene.h"
#include "Representations.h"
#include "Representation.h"
#include "RepresentationInfo.h"
#include "DebugMemory.h"
#include "Config.h"
#include "Utils.h"

MinimumRepresentation::MinimumRepresentation(const Fullerene* fullerene)
  : p_array(0)
{
  const Representations* reps = fullerene->get_representations();
  assert(reps);
  int len = reps->length();
  assert(len > 0);
  const Representation* rep = reps->get_representation(0);
  assert(rep);
  int num_infos = rep->number_of_infos();
  assert(num_infos > 0);
  const RepresentationInfo* repinfo = rep->get_info(0);
  assert(repinfo);
  assert(repinfo->clockwise == 1);
  p_array = copy_string(rep->get_array());
}

MinimumRepresentation::~MinimumRepresentation()
{
  if (p_array)
    delete[] p_array;
}

int MinimumRepresentation::compare(const MinimumRepresentation* you) const
{
  return strcmp(p_array, you->p_array);
}

void MinimumRepresentation::print() const
{
  printf("%s\n", p_array);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
