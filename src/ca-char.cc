/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Version.h"
#include "Config.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "FullereneCharacteristic.h"
#include "Representations.h"
#include "Interactives.h"
#include "DistanceMatrix.h"
#include "ReflectionPair.h"
#include <stdio.h>
#include "StringPair.h"
#include "Random.h"
#include "DebugMemory.h"

static void read_in(Set<StringPair>& pair, const char* pair_file)
{
  FILE* fptr = fopen(pair_file, "r");
  if (fptr == NULL)
    {
      fprintf(stderr, "cannot read pair file `%s'\n", pair_file);
      exit(1);
    }
  while (1)
    {
      char buffer[1024];
      if (fgets(buffer, 1024, fptr) == NULL)
        break;
      if (*buffer == '#')
        continue;
      int len = strlen(buffer);
      if (len > 0)
        buffer[len - 1] = '\0';
      char* ptr = buffer;
      while ((*ptr != ' ') && *ptr)
        ++ptr;
      *ptr++ = '\0';
      if (strcmp(buffer, "<not-listed>") == 0)
        continue;
      if (strcmp(ptr, "<not-listed>") == 0)
        continue;
      pair.search_else_add(new StringPair(buffer, ptr));
      pair.search_else_add(new StringPair(ptr, buffer));
    }
  fclose(fptr);
}

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] [file-name.gf]\n", arg0);
  fprintf(stderr, "    file-name.gf ................. gf file name,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --axes-summary ............... show axes summary (default),\n");
  fprintf(stderr, "    --pentagon-distances ......... show distances between pentagons,\n");
  fprintf(stderr, "    --pair=<pair file> ........... option --pentagon-distances allow to specify pair file,\n");
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
  char extension[5] = "";
  char* pair_file = 0;
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
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--pair=", 7) == 0)
        {
          pair_file = argv[0] + 7;
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

  if (pair_file && (pentagon_distances == false))
    usage(arg0);
  setvbuf(stdout, 0, _IONBF, 0);
  Random::initialize();
  CarbonAllotrope* ca = 0;
  Fullerene* fullerene = 0;
  FILE* fptr = 0;
  char buffer[1024];

  if (name)
    fptr = fopen(name, "r");
  else
    fptr = stdin;

  if (extension[0])
    mkdir(extension, 0755);

  if (pentagon_distances)
    {
      Fullerene::s_need_fullerene_characteristic = true;
      CarbonAllotrope::s_need_representations = true;
      CarbonAllotrope::s_need_representations_reflection = true;
    }
  else if (pentagon_matrix)
    Fullerene::s_need_distance_matrix = true;
  else
    {
      CarbonAllotrope::s_need_representations = true;
      CarbonAllotrope::s_need_all_axes = true;
    }
  CarbonAllotrope::s_need_representations = true;
  if (extension[0] == '\0')
    print_version("ca-char", stdout);
  Interactives::s_need_simulation = false;

  Set<StringPair> pair;
  if (pair_file)
    read_in(pair, pair_file);
  while (1)
    {
      if (fgets(buffer, 1024, fptr) == NULL)
        break;
      char* ptr = buffer;
      if (*ptr == '#')
        continue;
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
      if (extension[0])
        {
          char filename[1024];
          sprintf(filename, "%s/C%d=%s.%s", extension, ca->number_of_carbons(),
                  fullerene->get_generator_formula(), extension);
          fout = fopen(filename, "w");
          print_version("ca-char", fout);
        }
      if (pentagon_distances)
        {
          ReflectionPair* ref_pai = new ReflectionPair(ptr, fullerene);
          if (ref_pai->symmetric())
            printf("C%d (NoA=%d,MirrorSymmetric,Pdd=%s) %s\n",
                   fullerene->get_carbon_allotrope()->number_of_carbons(),
                   fullerene->get_representations()->number_of_automorphisms(),
                   fullerene->get_characteristic()->get_summary(),
                   fullerene->get_generator_formula());
          else
            {
              const char* gf = fullerene->get_generator_formula();
              StringPair* work = new StringPair(gf, gf);
              StringPair* sp = pair.search(work);
              delete work;
              if (sp)
                {
                  printf("C%d (NoA=%d,Mirror=%s,Pdd=%s) %s\n",
                         fullerene->get_carbon_allotrope()->number_of_carbons(),
                         fullerene->get_representations()->number_of_automorphisms(),
                         sp->your_string(),
                         fullerene->get_characteristic()->get_summary(),
                         fullerene->get_generator_formula());
                }
              else
                printf("C%d (NoA=%d,Pdd=%s) %s\n",
                       fullerene->get_carbon_allotrope()->number_of_carbons(),
                       fullerene->get_representations()->number_of_automorphisms(),
                       fullerene->get_characteristic()->get_summary(),
                       fullerene->get_generator_formula());
            }
          delete ref_pai;
        }
      else if (pentagon_matrix)
        fullerene->get_distance_matrix()->print_as_table(fout);
      else if (axes_summary)
        ca->print_axes_summary(fout);
      if (fout != stdout)
        fclose(fout);
      delete fullerene;
    }
  if (extension[0])
    printf("generated files into folder `%s'\n", extension);
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
