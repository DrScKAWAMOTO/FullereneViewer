/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "ReadKqueue.h"

ReadKqueue::ReadKqueue()
{
}

ReadKqueue::~ReadKqueue()
{
}

bool ReadKqueue::join(ReadHandler* handler)
{
  return false;
}

bool ReadKqueue::defect(ReadHandler* handler)
{
  return false;
}

ReadHandlerResult ReadKqueue::select(int timeout)
{
  return READ_HANDLER_RESULT_CONTINUED;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
