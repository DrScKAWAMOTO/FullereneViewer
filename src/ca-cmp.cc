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

void compare(const char* file1, const char* file2)
{
  FILE* fptr1 = fopen(file1, "r");
  assert(fptr1);
  FILE* fptr2 = fopen(file2, "r");
  assert(fptr2);
  while (1)
    {
      int c1 = fgetc(fptr1);
      int c2 = fgetc(fptr2);
      if ((c1 == EOF) && (c2 == EOF))
        break;
      if ((c1 == EOF) || (c2 == EOF) || (c1 != c2))
        {
          fclose(fptr1);
          fclose(fptr2);
          return;
        }
    }
  fclose(fptr1);
  fclose(fptr2);
  printf("%s %s\n", file1, file2);
}

int main(int argc, char* argv[])
{
  --argc;
  ++argv;
  if ((argc == 1) &&
      ((strcmp(argv[0], "-v") == 0) || (strcmp(argv[0], "--version") == 0)))
    {
      print_version("ca-cmp Ver 1.0");
      exit(0);
    }
  setvbuf(stdout, 0, _IONBF, 0);
  for (int i = 0; i < argc; ++i)
    for (int j = i + 1; j < argc; ++j)
      compare(argv[i], argv[j]);
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
