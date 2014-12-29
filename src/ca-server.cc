/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdlib.h>
#include <string.h>
#include "Version.h"
#include "Parallel.h"
#include "Process.h"

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options]\n", arg0);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
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
          print_version("ca-server");
          exit(0);
        }
      else
        usage(arg0);
    }
  const char* home_env = getenv("HOME");
  if (home_env == 0)
    {
      fprintf(stderr, "set environment HOME\n");
      exit(1);
    }
  MyString home = home_env;
  Parallel manager(home);
  manager.command_loop();
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
