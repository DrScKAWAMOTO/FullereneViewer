/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Version.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "FullereneCharacteristic.h"
#include "DistanceMatrix.h"
#include "Random.h"
#include "DebugMemory.h"

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] generator-name\n", arg0);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --development-view .... draw northem hemisphere side,\n");
  fprintf(stderr, "    --sequence-no ......... draw sequence No of carbons,\n");
  fprintf(stderr, "    -v (--version) ........ show version,\n");
  fprintf(stderr, "    -h (--help) ........... show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  const char *name = 0;
  bool development_view = false;
  bool print_out_sequence_no = false;

  const char* arg0 = argv[0];
  if (argc == 1)
    usage(arg0);
  argc--;
  argv++;
  while (argc > 0)
    {
      if ((strcmp(argv[0], "-h") == 0) || (strcmp(argv[0], "--help") == 0))
        {
          usage(arg0);
        }
      else if ((strcmp(argv[0], "-v") == 0) || (strcmp(argv[0], "--version") == 0))
        {
          print_version("ca-modeling");
          exit(0);
        }
      else if (strcmp(argv[0], "--development-view") == 0)
        {
          development_view = true;
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--sequence-no") == 0)
        {
          print_out_sequence_no = true;
          argc--;
          argv++;
        }
      else if (!name)
        {
          name = argv[0];
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }

  setvbuf(stdout, 0, _IONBF, 0);
  Random::initialize();
  CarbonAllotrope* ca = 0;
  Fullerene* fullerene = 0;

  if (!name)
    usage(arg0);

  if (!(((name[0] == 'S') || (name[0] == 'A')) && (name[1] != '\0') && (name[2] == '-')))
    usage(arg0);

  fullerene = new Fullerene(name);
  ca = fullerene->get_carbon_allotrope();
  if (print_out_sequence_no)
    ca->print_out_sequence_no = true;
  if (development_view)
    ca->draw_development_view_by_POVRay(name, ca->get_ring(ca->number_of_rings() - 1),
                                        1.0, 100, 10);
  else
    ca->draw_force_to_circle_by_POVRay(name, 1.0, 100, 10);
  delete fullerene;
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
