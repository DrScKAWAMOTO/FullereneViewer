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
#include "List.h"
#include "Fullerene.h"
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
  {
    Fullerene::s_need_representations = true;
    List<Fullerene> patterns;
    int last_number_of_carbons = -1;
    int last_number_of_automorphisms = -1;
    try
      {
        while (1)
          {
            char buffer[1024];
            if (fgets(buffer, 1024, stdin) != buffer)
              break;
            buffer[strlen(buffer) - 1] = '\0';
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
            Fullerene* fullerene = new Fullerene(name);
            int len = patterns.length();
            int i = 0;
            while (1)
              {
                if (i == len)
                  {
                    printf("C%d (NoA=%d) %s\n",
                           fullerene->get_carbon_allotrope()->number_of_carbons(),
                           fullerene->get_representations()->number_of_automorphisms(),
                           fullerene->get_generator_formula());
                    patterns.add(fullerene);
                    break;
                  }
                Fullerene* pati = patterns[i];
                if ((*fullerene) == (*pati))
                  {
                    delete fullerene;
                    break;
                  }
                ++i;
              }
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
  }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
