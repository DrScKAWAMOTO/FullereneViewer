/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/17 06:58:15 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include "ErrorCode.h"

void error_handler(ErrorCode result)
{
  switch (result)
    {
    case ERROR_CODE_OK:
      return;
    case ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_CLOSED_CONCAVE_BOUNDARY_SEGMENT:
      printf("Does not fix length of closed concave boundary segment !!\n");
      return;
    case ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_OPEN_CONCAVE_BOUNDARY_SEGMENT:
      printf("Does not fix length of open concave boundary segment !!\n");
      return;
    case ERROR_CODE_TOO_LONG_OPEN_CONCAVE_BOUNDARY_SEGMENT:
      printf("Too long open concave boundary segment !!\n");
      return;
    case ERROR_CODE_BREAKING_ISOLATED_PENTAGON_RULE:
      printf("Breaking by the Isolated Pentagon Rule (IPR) !!\n");
      return;
    case ERROR_CODE_ONLY_ONE_CONCAVE_AND_ONLY_ONE_CONVEX_BOUNDARY_SEGMENT:
      printf("Only one concave and only one convex boundary segment !!\n");
      return;
    case ERROR_CODE_ILLEGAL_GENERATOR_FORMULA:
      printf("Illegal generator formula !!\n");
      return;
  }
  printf("Unknown error code !!\n");
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
