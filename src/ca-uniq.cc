/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 20:29:25 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#include "Version.h"
#include "Set.h"
#include "MinimumRepresentation.h"
#include "Fullerene.h"
#include "Interactives.h"
#include "CarbonAllotrope.h"
#include "Representations.h"
#include "DebugMemory.h"

void usage(char* argv0)
{
  fprintf(stderr, "usage: %s < infile > outfile\n", argv0);
  exit(1);
}

int main(int argc, char* argv[])
{
  if ((argc == 2) &&
      ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)))
    {
      print_version("ca-uniq");
      exit(0);
    }
  if (argc != 1)
    usage(argv[0]);
  setvbuf(stdout, 0, _IONBF, 0);
  Fullerene::s_need_representations = true;
  Interactives::s_need_simulation = false;
  Set<MinimumRepresentation> patterns;
  int last_number_of_carbons = -1;
  int last_number_of_automorphisms = -1;
  int line_count = 0;
  print_version("ca-uniq", stdout);
  while (1)
    {
      char buffer[1024];
      if (fgets(buffer, 1024, stdin) != buffer)
        break;
      buffer[strlen(buffer) - 1] = '\0';
      if (buffer[0] == '#')
        continue;
      const char* name = buffer;
      if (*name == 'C')
        {
          ++name;
          int number_of_carbons;
          int number_of_automorphisms;
          sscanf(name, "%d", &number_of_carbons);
          while ((*name != ' ') && (*name != '\0'))
            ++name;
          if (*name == ' ')
            ++name;
          while ((*name != '=') && (*name != '\0'))
            ++name;
          sscanf(name, "%d", &number_of_automorphisms);
          while ((*name != ' ') && (*name != '\0'))
            ++name;
          if (*name == ' ')
            ++name;
          if ((number_of_carbons != last_number_of_carbons) ||
              (number_of_automorphisms != last_number_of_automorphisms))
            {
              if (number_of_carbons < last_number_of_carbons)
                {
                not_sorted_error:
                  fprintf(stderr, "infile is not sorted, use ca-sort output !!\n");
                  exit(1);
                }
              else if (number_of_carbons == last_number_of_carbons)
                {
                  if (number_of_carbons < last_number_of_carbons)
                    goto not_sorted_error;
                  else if (number_of_carbons == last_number_of_carbons)
                    ;
                  else
                    {
                      patterns.clean();
                      last_number_of_automorphisms = number_of_automorphisms;
                    }
                }
              else
                {
                  patterns.clean();
                  last_number_of_carbons = number_of_carbons;
                  last_number_of_automorphisms = number_of_automorphisms;
                }
            }
        }
      if (*name == '\0')
        continue;
      ++line_count;
      if ((line_count % 10000) == 0)
        fprintf(stderr, "%d lines proceeded.\n", line_count);
      Fullerene* fullerene = new Fullerene(name);
      MinimumRepresentation* min_rep = new MinimumRepresentation(fullerene);
      if (patterns.search_else_add(min_rep))
        delete min_rep;
      else
        printf("C%d (NoA=%d) %s\n",
               fullerene->get_carbon_allotrope()->number_of_carbons(),
               fullerene->get_representations()->number_of_automorphisms(),
               fullerene->get_generator_formula());
      delete fullerene;
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
