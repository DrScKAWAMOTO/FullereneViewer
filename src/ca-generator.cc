/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/02/05 20:29:25 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Version.h"
#include "Fullerenes.h"
#include "Random.h"
#include "DebugMemory.h"
#include <new>

#define MAXIMUM_VERTICES_OF_POLYGON 6

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] [generator-formula]\n", arg0);
  fprintf(stderr, "    generator-formula .... if specified, list up from this formula,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    default options are --symmetric=100 --step-backward,\n");
  fprintf(stderr, "    --symmetric=[num] .... list up all symmetric fullerenes up to [num] carbons,\n");
  fprintf(stderr, "            generator-formula looks like 'S1-5b6b...',\n");
  fprintf(stderr, "    --ordinary=[num] ..... list up all ordinary fullerenes up to [num] carbons,\n");
  fprintf(stderr, "            generator-formula looks like 'A1-5b6b...',\n");
  fprintf(stderr, "    --tube=[num] ......... list up all carbon nano tubes up to [num] carbons,\n");
  fprintf(stderr, "            generator-formula looks like 'T10,0,1-5b6b...', has to be specified,\n");
  fprintf(stderr, "    --close=[num] ........ close [num] connected boundary component(s),\n");
  fprintf(stderr, "            default is infinity,\n");
  fprintf(stderr, "    --step-copy-branch ... use ``step copy branch'' algorithm,\n");
  fprintf(stderr, "    --step-forward ....... use ``step forward'' algorithm,\n");
  fprintf(stderr, "    --step-backward ...... use ``step backward'' algorithm (default),\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message,\n");
  fprintf(stderr, "environment variables:\n");
  fprintf(stderr, "    CA_STEP_ALGORITHM .... may specify one of ``--step-*'' above.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  int symmetric = 100;
  int ordinary = -1;
  int tube = -1;
  int close = INT_MAX;
  StepAlgorithm step_algorithm = STEP_ALGORITHM_BACKWARD;
  const char* arg0 = argv[0];
  const char* generator_formula = 0;
  const char* env_value = getenv("CA_STEP_ALGORITHM");

  if (env_value)
    {
      if (strcmp(env_value, "--step-copy-branch") == 0)
        step_algorithm = STEP_ALGORITHM_COPY_BRANCH;
      else if (strcmp(env_value, "--step-forward") == 0)
        step_algorithm = STEP_ALGORITHM_FORWARD;
      else if (strcmp(env_value, "--step-backward") == 0)
        step_algorithm = STEP_ALGORITHM_BACKWARD;
    }

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
          print_version("ca-generator");
          exit(0);
        }
      else if (strncmp(argv[0], "--symmetric=", 12) == 0)
        {
          symmetric = atoi(argv[0] + 12);
          ordinary = -1;
          tube = -1;
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--ordinary=", 11) == 0)
        {
          ordinary = atoi(argv[0] + 11);
          symmetric = -1;
          tube = -1;
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--tube=", 7) == 0)
        {
          tube = atoi(argv[0] + 7);
          symmetric = -1;
          ordinary = -1;
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--close=", 8) == 0)
        {
          close = atoi(argv[0] + 8);
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--step-copy-branch") == 0)
        {
          step_algorithm = STEP_ALGORITHM_COPY_BRANCH;
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--step-forward") == 0)
        {
          step_algorithm = STEP_ALGORITHM_FORWARD;
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--step-backward") == 0)
        {
          step_algorithm = STEP_ALGORITHM_BACKWARD;
          argc--;
          argv++;
        }
      else if (!generator_formula)
        {
          generator_formula = argv[0];
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }

  setvbuf(stdout, 0, _IONBF, 0);
  Random::initialize();
  print_version("ca-generator", stdout);
  if (symmetric >= 1)
    {
      if (symmetric < 60)
        symmetric = 60;
      if (!generator_formula)
        generator_formula = "S1-";
      if ((generator_formula[0] != 'S') || (generator_formula[1] < '1') ||
          (generator_formula[1] > '9') || (generator_formula[2] != '-'))
        usage(arg0);
      Fullerenes ap = Fullerenes(generator_formula, symmetric, true,
                                 MAXIMUM_VERTICES_OF_POLYGON, close, step_algorithm);
    }
  else if (ordinary >= 1)
    {
      if (ordinary < 60)
        ordinary = 60;
      if (!generator_formula)
        generator_formula = "A1-";
      if ((generator_formula[0] != 'A') || (generator_formula[1] != '1') ||
          (generator_formula[2] != '-'))
        usage(arg0);
      Fullerenes ap = Fullerenes(generator_formula, ordinary, false,
                                 MAXIMUM_VERTICES_OF_POLYGON, close, step_algorithm);
    }
  else if (tube >= 1)
    {
      if (tube < 60)
        tube = 60;
      if (!generator_formula || (generator_formula[0] != 'T'))
        usage(arg0);
      const char *ptr = generator_formula + 1;
      while ((*ptr >= '0') && (*ptr <= '9'))
        ++ptr;
      if (*ptr++ != ',')
        usage(arg0);
      while ((*ptr >= '0') && (*ptr <= '9'))
        ++ptr;
      if (*ptr++ != ',')
        usage(arg0);
      while ((*ptr >= '0') && (*ptr <= '9'))
        ++ptr;
      if (*ptr != '-')
        usage(arg0);
      Fullerenes ap = Fullerenes(generator_formula, tube, false,
                                 MAXIMUM_VERTICES_OF_POLYGON, close, step_algorithm);
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
