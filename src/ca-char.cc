/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include "Version.h"
#include "Config.h"
#include "PddGfLines.h"
#include "CarbonAllotrope.h"
#include "Fullerene.h"
#include "Characteristic.h"
#include "Representations.h"
#include "Interactives.h"
#include "Clustering.h"
#include "ReflectionPair.h"
#include "StringPair.h"
#include "Random.h"
#include "DebugMemory.h"

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] [file-name.pddgf]\n", arg0);
  fprintf(stderr, "    file-name.pddgf .............. pddgf file name,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --axes-summary ............... show axes summary (default),\n");
  fprintf(stderr, "    --pentagon-distances ......... show distances between pentagons,\n");
  fprintf(stderr, "    --pentagon-clustering ........ show clustering between pentagons,\n");
  fprintf(stderr, "    -v (--version) ............... show version,\n");
  fprintf(stderr, "    -h (--help) .................. show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  MyString name;
  bool axes_summary = true;
  bool pentagon_distances = false;
  bool pentagon_clustering = false;
  bool no_header = false;
  MyString extension;
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
          pentagon_clustering = false;
          extension = "axes";
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--pentagon-distances") == 0)
        {
          axes_summary = false;
          pentagon_distances = true;
          pentagon_clustering = false;
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--pentagon-clustering") == 0)
        {
          axes_summary = false;
          pentagon_distances = false;
          pentagon_clustering = true;
          extension = "clustering";
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--no-header") == 0)
        {
          no_header = true;
          argc--;
          argv++;
        }
      else if (argv[0][0] == '-')
        usage(arg0);
      else if (name.length() == 0)
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

  if (name.length() > 0)
    fptr = fopen((char*)name, "r");
  else
    fptr = stdin;

  if (extension.length() > 0)
    mkdir((char*)extension, 0755);

  if (pentagon_distances)
    {
      Fullerene::s_need_fullerene_characteristic = true;
      CarbonAllotrope::s_need_representations = true;
      CarbonAllotrope::s_need_representations_reflection = true;
    }
  else if (pentagon_clustering)
    Fullerene::s_need_clustering = true;
  else
    {
      CarbonAllotrope::s_need_representations = true;
      CarbonAllotrope::s_need_all_axes = true;
    }
  if ((extension.length() == 0) && (no_header == false))
    print_version("ca-char", stdout);
  Interactives::s_need_simulation = false;

  PddGfLines lines(true);
  while (1)
    {
      int number_of_carbons;
      if (fgets(buffer, 1024, fptr) == NULL)
        break;
      char* ptr = buffer;
      if (*ptr == '#')
        continue;
      if (*ptr == 'C')
        {
          ++ptr;
          number_of_carbons = atoi(ptr);
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
      FILE* fout = stdout;
      if (extension.length() > 0)
        {
          MyString filename;
          filename.append_string(extension);
          filename.append_string("/C");
          filename.append_int(number_of_carbons);
          filename.append_char('=');
          filename.append_string(ptr);
          filename.append_char('.');
          filename.append_string(extension);
          fout = fopen((char*)filename, "w");
          if (no_header == false)
            print_version("ca-char", fout);
        }
      if (pentagon_distances)
        {
          PddGfLineResult result = lines.add_PddGfLine_and_refresh(ptr);
          if (result == PDD_GF_LINE_RESULT_NOT_SORTED)
            {
              fprintf(stderr, "infile is not sorted, use ca-sort output !!\n");
              exit(1);
            }
          else if (result == PDD_GF_LINE_RESULT_ILLEGAL_FULLERENE)
            {
              fprintf(stderr, "fullerene `%s' is not able to be constructed !!\n", ptr);
              exit(1);
            }
        }
      else if (pentagon_clustering)
        {
          fullerene = new Fullerene(ptr);
          fullerene->get_clustering()->print_as_table(fout);
          delete fullerene;
        }
      else if (axes_summary)
        {
          fullerene = new Fullerene(ptr);
          ca = fullerene->get_carbon_allotrope();
          ca->print_axes_summary(fout);
          delete fullerene;
        }
      if (fout != stdout)
        fclose(fout);
    }
  lines.refresh_force();
  if (extension.length() > 0)
    printf("generated files into folder `%s'\n", (char*)extension);
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
