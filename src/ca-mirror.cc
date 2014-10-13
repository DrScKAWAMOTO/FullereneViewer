/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#include "Version.h"
#include "Set.h"
#include "ReflectionPair.h"
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

void print_pattern(const Set<ReflectionPair>& patterns)
{
  int len = patterns.length();
  for (int i = 0; i < len; ++i)
    {
      ReflectionPair* ref_pai = patterns[i];
      const char* ones_formula = ref_pai->ones_generator_formula();
      const char* the_others_formula = ref_pai->the_others_generator_formula();
      if (ones_formula[0])
        {
          if (the_others_formula[0])
            printf("%s %s\n", ones_formula, the_others_formula);
          else
            printf("%s <not-listed>\n", ones_formula);
        }
      else if (the_others_formula[0])
        printf("%s <not-listed>\n", the_others_formula);
    }
}

void do_mirror()
{
  setvbuf(stdout, 0, _IONBF, 0);
  CarbonAllotrope::s_need_representations = true;
  CarbonAllotrope::s_need_representations_reflection = true;
  Interactives::s_need_simulation = false;
  print_version("ca-mirror", stdout);
  int last_number_of_carbons = -1;
  int last_number_of_automorphisms = -1;
  int line_count = 0;
  Set<ReflectionPair> patterns;
  while (1)
    {
      char buffer[1024];
      if (fgets(buffer, 1024, stdin) != buffer)
        break;
      buffer[strlen(buffer) - 1] = '\0';
      if (buffer[0] == '#')
        continue;
      ++line_count;
      if ((line_count % 10000) == 0)
        fprintf(stderr, "%d lines proceeded.\n", line_count);
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
          if (*name == '=')
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
                  fprintf(stderr, "infile is not sorted, use ca-sort !!\n");
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
                      print_pattern(patterns);
                      patterns.clean();
                      last_number_of_automorphisms = number_of_automorphisms;
                    }
                }
              else
                {
                  print_pattern(patterns);
                  patterns.clean();
                  last_number_of_carbons = number_of_carbons;
                  last_number_of_automorphisms = number_of_automorphisms;
                }
            }
        }
      Fullerene* fullerene = new Fullerene(name);
      ReflectionPair* ref_pai = new ReflectionPair(name, fullerene);
      if (ref_pai->symmetric())
        delete ref_pai;
      else
        {
          ReflectionPair* that = patterns.search_else_add(ref_pai);
          if (that)
            {
              that->merge(ref_pai);
              delete ref_pai;
            }
        }
      delete fullerene;
    }
  print_pattern(patterns);
}

int main(int argc, char* argv[])
{
  if ((argc == 2) &&
      ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)))
    {
      print_version("ca-mirror");
      exit(0);
    }
  if (argc != 1)
    usage(argv[0]);
  do_mirror();
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
