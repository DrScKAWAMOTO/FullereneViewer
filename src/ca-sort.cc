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
#include "Version.h"
#include "GeneratorLine.h"
#include "DistanceSummaryLine.h"
#include "SortedList.h"
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
      print_version("ca-sort Ver 1.0");
      exit(0);
    }
  if (argc != 1)
    usage(argv[0]);
  setvbuf(stdout, 0, _IONBF, 0);
  char buffer[1024];
  if (fgets(buffer, 1024, stdin) != buffer)
    usage(argv[0]);
  if (buffer[0] == 'C')
    {
      SortedList<GeneratorLine> lines;
      while (1)
        {
          buffer[strlen(buffer) - 1] = '\0';
          GeneratorLine* gtl = new GeneratorLine(buffer);
          lines.add(gtl);
          if (fgets(buffer, 1024, stdin) != buffer)
            break;
        }
      int len = lines.length();
      for (int i = 0; i < len; ++i)
        lines[i]->print();
    }
  else
    {
      SortedList<DistanceSummaryLine> lines;
      while (1)
        {
          buffer[strlen(buffer) - 1] = '\0';
          DistanceSummaryLine* dsl = new DistanceSummaryLine(buffer);
          lines.add(dsl);
          if (fgets(buffer, 1024, stdin) != buffer)
            break;
        }
      int len = lines.length();
      for (int i = 0; i < len; ++i)
        lines[i]->print();
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
