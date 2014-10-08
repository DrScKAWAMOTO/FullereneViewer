/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include "Object.h"
#include "ShutUp.h"

Object::Object(int sequence_no)
  : p_sequence_no(sequence_no), p_link_counter(0)
{
}

Object::Object(const Object& you)
  : p_sequence_no(you.p_sequence_no), p_link_counter(0)
{
}

Object::~Object()
{
}

Object& Object::operator = (const Object& UNUSED(you))
{
  fprintf(stderr, "%s:%d: internal error dont use.\n", __FILE__, __LINE__);
  exit(1);
  return *this;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/






