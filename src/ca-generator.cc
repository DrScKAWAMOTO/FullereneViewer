/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 20:29:25 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Version.h"
#include "Fullerenes.h"
#include "Random.h"
#include "DebugMemory.h"
#include <new>

#define MAXIMUM_VERTICES_OF_POLYGON 6

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] [generator label]\n", arg0);
  fprintf(stderr, "    generator label ....... if specified, list up from this,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --symmetric=[num] ..... list up all symmetric fullerenes up to [num] carbons,\n");
  fprintf(stderr, "    --ordinary=[num] ...... list up all ordinary fullerenes up to [num] carbons,\n");
  fprintf(stderr, "    -v (--version) ........ show version,\n");
  fprintf(stderr, "    -h .................... show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  int symmetric = -1;
  int ordinary = -1;

  const char* arg0 = argv[0];
  const char* generator_label = 0;
  if (argc == 1)
    usage(arg0);
  argc--;
  argv++;
  while (argc > 0)
    {
      if (strcmp(argv[0], "-h") == 0)
        {
          usage(arg0);
        }
      else if ((strcmp(argv[0], "-v") == 0) || (strcmp(argv[0], "--version") == 0))
        {
          print_version("ca-generator Ver 1.0");
          exit(0);
        }
      else if (strncmp(argv[0], "--symmetric=", 12) == 0)
        {
          symmetric = atoi(argv[0] + 12);
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--ordinary=", 11) == 0)
        {
          ordinary = atoi(argv[0] + 11);
          argc--;
          argv++;
        }
      else if (!generator_label)
        {
          generator_label = argv[0];
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }

  setvbuf(stdout, 0, _IONBF, 0);
  Random::initialize();
  try
    {
      if (symmetric >= 1)
        {
          if (symmetric < 60)
            symmetric = 60;
          if (generator_label && (generator_label[0] != 'S'))
            usage(arg0);
          Fullerenes ap = Fullerenes(generator_label, symmetric, true,
                                     MAXIMUM_VERTICES_OF_POLYGON);
        }
      else if (ordinary >= 1)
        {
          if (ordinary < 60)
            ordinary = 60;
          if (generator_label && (generator_label[0] != 'A'))
            usage(arg0);
          Fullerenes ap = Fullerenes(generator_label, ordinary, false,
                                     MAXIMUM_VERTICES_OF_POLYGON);
        }
    }
  catch (const std::bad_alloc& err)
    {
      printf("std::bad_alloc exception\n");
      exit(10);
    }
  catch (...)
    {
      printf("unknown exception\n");
      exit(11);
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
