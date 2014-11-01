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
#include "MinimumRepresentation.h"
#include "Fullerene.h"
#include "Interactives.h"
#include "CarbonAllotrope.h"
#include "Representations.h"
#include "PddGfLines.h"
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
  CarbonAllotrope::s_need_representations = true;
#if 1 /* TODO これを有効化しても、遅くなるだけで動作はかわらないことを確認する */
  CarbonAllotrope::s_need_representations_reflection = true;
#endif
  Interactives::s_need_simulation = false;
  Set<MinimumRepresentation> patterns;
  int line_count = 0;
  print_version("ca-uniq", stdout);

  PddGfLines lines(false);
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
          while ((*name != ' ') && (*name != '\0'))
            ++name;
          if (*name == ' ')
            ++name;
          while ((*name != ' ') && (*name != '\0'))
            ++name;
          if (*name == ' ')
            ++name;
        }
      if (*name == '\0')
        continue;
      ++line_count;
      if ((line_count % 10000) == 0)
        fprintf(stderr, "%d lines proceeded.\n", line_count);
      PddGfLineResult result = lines.add_PddGfLine_and_refresh(name);
      if (result == PDD_GF_LINE_RESULT_NOT_SORTED)
        {
          fprintf(stderr, "infile is not sorted, use ca-sort output !!\n");
          exit(1);
        }
      else if (result == PDD_GF_LINE_RESULT_ILLEGAL_FULLERENE)
        {
          fprintf(stderr, "fullerene `%s' is not able to be constructed !!\n", name);
          exit(1);
        }
    }
  lines.refresh_force();
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
