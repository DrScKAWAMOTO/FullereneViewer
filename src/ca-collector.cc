/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include "Version.h"
#include "Fullerenes.h"
#include "Step.h"
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
  fprintf(stderr, "    --parallel=[level] ... special use of invoked from ca-parallel,\n");
  fprintf(stderr, "    --step-copy-branch ... use ``step copy branch'' algorithm,\n");
  fprintf(stderr, "    --step-forward ....... use ``step forward'' algorithm,\n");
  fprintf(stderr, "    --step-backward ...... use ``step backward'' algorithm (default),\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message,\n");
  fprintf(stderr, "environment variables:\n");
  fprintf(stderr, "    CA_STEP_ALGORITHM .... may specify one of ``--step-*'' above.\n");
  exit(0);
}

static void* command_routine(void *arg)
{
  while (1)
    {
      char command_line[1024];
      if (fgets(command_line, 1024, stdin) == NULL)
        exit(2);
      int len = strlen(command_line);
      if ((len > 0) && (command_line[len - 1] == '\n'))
        command_line[len - 1] = '\0';
      if (strncmp(command_line, "level=", 6) == 0)
        {
          int progress_level = atoi(command_line + 6);
          Step::set_progress_level(progress_level);
        }
      else if (strcmp(command_line, "exit") == 0)
        exit(0);
    }
}

int main(int argc, char *argv[])
{
  int symmetric = 100;
  int ordinary = -1;
  int tube = -1;
  int close = INT_MAX;
  int parallel_level = 0;
  StepAlgorithm step_algorithm = STEP_ALGORITHM_BACKWARD;
  const char* arg0 = argv[0];
  MyString generator_formula;
  MyString env_value;
  char* foo = getenv("CA_STEP_ALGORITHM");
  if (foo)
    env_value = foo;
  if (env_value.length() > 0)
    {
      if (env_value.compare("--step-copy-branch") == 0)
        step_algorithm = STEP_ALGORITHM_COPY_BRANCH;
      else if (env_value.compare("--step-forward") == 0)
        step_algorithm = STEP_ALGORITHM_FORWARD;
      else if (env_value.compare("--step-backward") == 0)
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
          print_version("ca-collector");
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
      else if (strncmp(argv[0], "--parallel=", 11) == 0)
        {
          parallel_level = atoi(argv[0] + 11);
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
      else if (generator_formula.length() == 0)
        {
          generator_formula = argv[0];
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }

  if (parallel_level > 0)
    {
      pthread_t thread;
      Step::initiate_progress_level(parallel_level);
      int result;
      result = pthread_create(&thread, 0, command_routine, 0);
      if (result != 0)
        {
          fprintf(stderr, "%s: pthread_create() error\n", arg0);
          exit(1);
        }
    }
  setvbuf(stdout, 0, _IONBF, 0);
  Random::initialize();
  print_version("ca-collector", stdout);
  if (symmetric >= 1)
    {
      if (symmetric < 60)
        symmetric = 60;
      if (generator_formula.length() == 0)
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
      if (generator_formula.length() == 0)
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
      if ((generator_formula.length() == 0) || (generator_formula[0] != 'T'))
        usage(arg0);
      const char *ptr = (char*)generator_formula + 1;
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
