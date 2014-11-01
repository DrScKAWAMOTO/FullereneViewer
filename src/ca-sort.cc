/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Version.h"
#include "GeneratorLine.h"
#include "DistanceSummaryLine.h"
#include "Set.h"
#include "DebugMemory.h"

void usage(const char* argv0)
{
  fprintf(stderr, "usage: %s [options] < infile\n", argv0);
  fprintf(stderr, "   infile ............. generator formula lines, or distance summary lines,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "   --divide=<num> ..... divide output upto [num] files,\n");
  fprintf(stderr, "   --out=<filename> ... specify output file name.\n");
  exit(1);
}

int main(int argc, char* argv[])
{
  const char* outfilename = 0;
  int divide = -1;
  const char* arg0 = argv[0];
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
          print_version("ca-sort");
          exit(0);
        }
      else if (strncmp(argv[0], "--divide=", 9) == 0)
        {
          divide = atoi(argv[0] + 9);
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--out=", 6) == 0)
        {
          outfilename = argv[0] + 6;
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }
  setvbuf(stdout, 0, _IONBF, 0);
  char buffer[1024];
  while (1)
    {
      if (fgets(buffer, 1024, stdin) != buffer)
        exit(0);
      if (buffer[0] != '#')
        break;
    }
  if (buffer[0] == 'C')
    {
      Set<GeneratorLine> lines;
      while (1)
        {
          buffer[strlen(buffer) - 1] = '\0';
          if (buffer[0] != '#')
            {
              GeneratorLine* gtl = new GeneratorLine(buffer);
              GeneratorLine* result = lines.search_else_add(gtl);
              assert(result == 0);
            }
          if (fgets(buffer, 1024, stdin) != buffer)
            break;
        }
      if (divide == -1)
        {
          FILE* output = 0;
          if (outfilename)
            {
              output = fopen(outfilename, "w");
            }
          else
            {
              output = stdout;
            }
          print_version("ca-sort", output);
          int len = lines.length();
          for (int i = 0; i < len; ++i)
            lines[i]->print(output);
        }
      else
        {
          int len = lines.length();
          if (len == 0)
            return 0;
          if (outfilename == 0)
            outfilename = "output.gf";
          int width = 1;
          int dec = 10;
          while (dec < divide)
            {
              dec *= 10;
              width++;
            }
          int from = 0;
          for (int i = 0; i < divide; ++i)
            {
              int step = (len - from) / (divide - i);
              int to = from + step;
              assert(to > 0);
              if (to >= len)
                to = len;
              else
                {
                  int low = to - 1;
                  int hi = len - 1;
                  if (lines[low]->same_number_NoA(lines[hi]))
                    to = hi + 1;
                  else
                    {
                      while (low < hi - 1)
                        {
                          int mid = (low + hi) / 2;
                          if (lines[low]->same_number_NoA(lines[mid]))
                            low = mid;
                          else
                            hi = mid;
                        }
                      to = hi;
                    }
                }
              char buffer[1024];
              sprintf(buffer, "%s.%0*d", outfilename, width, i);
              FILE* fptr = fopen(buffer, "w");
              print_version("ca-sort", fptr);
              for (int j = from; j < to; ++j)
                lines[j]->print(fptr);
              fclose(fptr);
              from = to;
            }
        }
    }
  else
    {
      Set<DistanceSummaryLine> lines;
      while (1)
        {
          buffer[strlen(buffer) - 1] = '\0';
          if (buffer[0] != '#')
            {
              DistanceSummaryLine* dsl = new DistanceSummaryLine(buffer);
              DistanceSummaryLine* result = lines.search_else_add(dsl);
              assert(result == 0);
            }
          if (fgets(buffer, 1024, stdin) != buffer)
            break;
        }
      print_version("ca-sort", stdout);
      int len = lines.length();
      for (int i = 0; i < len; ++i)
        lines[i]->print();
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
