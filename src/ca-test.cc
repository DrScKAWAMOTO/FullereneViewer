/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/01 01:01:14 JST
 */

#include <stdlib.h>
#include <string.h>
#include "Version.h"
#include "Fullerene.h"
#include "CarbonAllotrope.h"
#include "Pattern.h"
#include "DebugMemory.h"

int main(int argc, char *argv[])
{
  if ((argc == 2) &&
      ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)))
    {
      print_version("ca-test");
      exit(0);
    }
  if (argc != 1)
    return 1;
  Fullerene* fullerene = new Fullerene("T-Y");
  CarbonAllotrope* ca = fullerene->get_carbon_allotrope();
  ca->print_detail();
  delete fullerene;
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
