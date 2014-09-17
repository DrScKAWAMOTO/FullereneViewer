/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/31 19:48:32 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Version.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "FullereneCharacteristic.h"
#include "Interactives.h"
#include "DistanceMatrix.h"
#include "Random.h"
#include "DebugMemory.h"

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] <generator-formula|generator-formula-list-file]\n",
          arg0);
  fprintf(stderr, "    generator-formula ............ generator-formula for fullerene,\n");
  fprintf(stderr, "    generator-formula-list-file .. file name which contains generator-formulas,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --axes-summary ............... show axes summary (default),\n");
  fprintf(stderr, "    --pentagon-distances ......... show distances between pentagons,\n");
  fprintf(stderr, "    --pentagon-matrix ............ show distance matrix between pentagons,\n");
  fprintf(stderr, "    -v (--version) ............... show version,\n");
  fprintf(stderr, "    -h (--help) .................. show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  const char *name = 0;
  bool axes_summary = true;
  bool pentagon_distances = false;
  bool pentagon_matrix = false;
  char extension[5];

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
          print_version("ca-char");
          exit(0);
        }
      else if (strcmp(argv[0], "--axes-summary") == 0)
        {
          axes_summary = true;
          pentagon_distances = false;
          pentagon_matrix = false;
          strcpy(extension, "axes");
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--pentagon-distances") == 0)
        {
          axes_summary = false;
          pentagon_distances = true;
          pentagon_matrix = false;
          strcpy(extension, "dist");
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--pentagon-matrix") == 0)
        {
          axes_summary = false;
          pentagon_distances = false;
          pentagon_matrix = true;
          strcpy(extension, "dmat");
          argc--;
          argv++;
        }
      else if (argv[0][0] == '-')
        usage(arg0);
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
  FILE* fptr = 0;
  char buffer[1024];

  if (!name)
    fptr = stdin;
  else if (((name[0] == 'S') || (name[0] == 'A')) &&
           (name[1] != '\0') && (name[2] == '-'))
    strcpy(buffer, name);
  else if ((name[0] == 'T') && (name[1] != '\0'))
    strcpy(buffer, name);
  else
    fptr = fopen(name, "r");

  if (fptr)
    mkdir(extension, 0755);

  if (pentagon_distances)
    Fullerene::s_need_fullerene_characteristic = true;
  else if (pentagon_matrix)
    Fullerene::s_need_distance_matrix = true;
  else
    Fullerene::s_need_representations = true;
  if (fptr)
    Fullerene::s_need_representations = true;
  Interactives::s_need_simulation = false;

  while (1)
    {
      if (fptr && (fgets(buffer, 1024, fptr) == NULL))
        break;
      char* ptr = buffer;
      if (*ptr == 'C')
        {
          while (*ptr != ' ')
            ++ptr;
          ++ptr;
          while (*ptr != ' ')
            ++ptr;
          ++ptr;
        }
      int len = strlen(ptr);
      if (ptr[len - 1] == '\n')
        ptr[len - 1] = '\0';
      fullerene = new Fullerene(ptr);
      ca = fullerene->get_carbon_allotrope();
      FILE* fout = stdout;
      if (fptr)
        {
          char filename[1024];
          sprintf(filename, "%s/C%d=%s.%s", extension, ca->number_of_carbons(),
                  fullerene->get_generator_formula(), extension);
          fout = fopen(filename, "w");
        }
      print_version("ca-char", fout);
      if (pentagon_distances)
        fullerene->get_characteristic()->print_summary(fout);
      else if (pentagon_matrix)
        fullerene->get_distance_matrix()->print_as_table(fout);
      else if (axes_summary)
        ca->print_axes_summary(fout);
      fclose(fout);
      delete fullerene;
      if (!fptr)
        break;
    }
  if (fptr)
    printf("generated files into folder `%s'\n", extension);
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
