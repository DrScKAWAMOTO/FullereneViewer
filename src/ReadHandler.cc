/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include "ReadHandler.h"

ReadHandler::ReadHandler()
  : p_readfd(-1)
{
}

ReadHandler::~ReadHandler()
{
}

int ReadHandler::compare(const ReadHandler* you) const
{
  return p_readfd - you->p_readfd;
}

ReadHandlerResult ReadHandler::call()
{
  return READ_HANDLER_RESULT_CONTINUED;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
