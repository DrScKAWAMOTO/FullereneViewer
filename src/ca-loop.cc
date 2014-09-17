/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/08/24 20:00:00 JST
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "Fullerenes.h"
#include "Version.h"
#include "Utils.h"

#define BUFFER_SIZE 1000

static void usage(const char* arg0)
{
  fprintf(stderr,
          "usage: %s [options] [<start generator-formula> [<ending generator-formula>]]\n",
          arg0);
  fprintf(stderr, "    <start generator-formula> ..... list up from this formula,\n");
  fprintf(stderr, "    <ending generator-formula> .... list up to this formula,\n");
  fprintf(stderr, "            <ending generator-formulas> permit with ending ``*'',\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --symmetric=[num] .... list up all symmetric fullerenes up to [num] carbons,\n");
  fprintf(stderr, "            <generator-formula>s look like 'S1-5b6b...',\n");
  fprintf(stderr, "            default <start generator-formulas> is 'S1-',\n");
  fprintf(stderr, "            default <ending generator-formulas> is 'S4-*',\n");
  fprintf(stderr, "    --ordinary=[num] ..... list up all ordinary fullerenes up to [num] carbons,\n");
  fprintf(stderr, "            <generator-formula>s look like 'A1-5b6b...',\n");
  fprintf(stderr, "            default <start generator-formulas> is 'A1-',\n");
  fprintf(stderr, "            default <ending generator-formulas> is 'A1-*',\n");
  fprintf(stderr, "    --tube=[num] ......... list up all carbon nano tubes up to [num] carbons,\n");
  fprintf(stderr, "            <generator-formula>s look like 'T7,6,2-5b6b...',\n");
  fprintf(stderr, "            default <start generator-formulas> is 'T10,0,3-',\n");
  fprintf(stderr, "            default <ending generator-formulas> is 'T10,0,3-*',\n");
  fprintf(stderr, "    --test ............... list up test patterns,\n");
  fprintf(stderr, "            <generator-formula>s look like 'T10,0,3-5b6b...', has to be specified,\n");
  fprintf(stderr, "            default <start generator-formulas> is 'T10,0,3-',\n");
  fprintf(stderr, "            default <ending generator-formulas> is 'T10,0,3-*',\n");
  fprintf(stderr, "    --close=[num] ........ close [num] connected boundary component(s),\n");
  fprintf(stderr, "            default is infinity,\n");
  fprintf(stderr, "    --step-copy-branch ... use ``step copy branch'' algorithm,\n");
  fprintf(stderr, "    --step-forward ....... use ``step forward'' algorithm,\n");
  fprintf(stderr, "    --step-backward ...... use ``step backward'' algorithm (default),\n");
  fprintf(stderr, "    --until-ending ....... until ending formula, closed condition (default),\n");
  fprintf(stderr, "    --except-ending ...... except ending formula, open condition,\n");
  fprintf(stderr, "    --out=<gf file> ...... output generator formula file name,\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message,\n");
  fprintf(stderr, "environment variables:\n");
  fprintf(stderr, "    CA_STEP_ALGORITHM .... may specify one of ``--step-*'' above.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  int symmetric = -1;
  int ordinary = -1;
  int tube = -1;
  int test = -1;
  int close = INT_MAX;
  StepAlgorithm step_algorithm = STEP_ALGORITHM_BACKWARD;
  bool except_ending = false;
  const char* arg0 = argv[0];
  char start_generator_formula[BUFFER_SIZE] = "";
  char ending_generator_formula[BUFFER_SIZE] = "";
  const char* gf_file_name = 0;
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

  if (argc == 1)
    usage(arg0);
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
          print_version("ca-loop");
          exit(0);
        }
      else if (strncmp(argv[0], "--symmetric=", 12) == 0)
        {
          symmetric = atoi(argv[0] + 12);
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--ordinary=", 11) == 0)
        {
          ordinary = atoi(argv[0] + 11);
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--tube=", 7) == 0)
        {
          tube = atoi(argv[0] + 7);
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--test") == 0)
        {
          test = 1;
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
      else if (strcmp(argv[0], "--until-ending") == 0)
        {
          except_ending = false;
          argc--;
          argv++;
        }
      else if (strcmp(argv[0], "--except-ending") == 0)
        {
          except_ending = true;
          argc--;
          argv++;
        }
      else if (strncmp(argv[0], "--out=", 6) == 0)
        {
          gf_file_name = argv[0] + 6;
          argc--;
          argv++;
        }
      else if (start_generator_formula[0] == '\0')
        {
          assert(strlen(argv[0]) < BUFFER_SIZE);
          strcpy(start_generator_formula, argv[0]);
          argc--;
          argv++;
        }
      else if (ending_generator_formula[0] == '\0')
        {
          assert(strlen(argv[0]) < BUFFER_SIZE);
          strcpy(ending_generator_formula, argv[0]);
          argc--;
          argv++;
        }
      else
        usage(arg0);
    }

  FILE* fptr = stdout;
  if (gf_file_name)
    {
      fptr = fopen(gf_file_name, "w");
      if (fptr == 0)
        {
          fprintf(stderr, "%s: can not open generator formula file ``%s''\n",
                  arg0, gf_file_name);
          exit(1);
        }
    }
  setvbuf(fptr, 0, _IONBF, 0);
  if (symmetric >= 1)
    {
      if (symmetric < 60)
        symmetric = 60;
      if (start_generator_formula[0] == '\0')
        strcpy(start_generator_formula, "S1-");
      if (ending_generator_formula[0] == '\0')
        strcpy(ending_generator_formula, "S4-*");
      if (start_generator_formula[0] != 'S')
        usage(arg0);
      if (ending_generator_formula[0] != 'S')
        usage(arg0);
      ordinary = -1;
      tube = -1;
      test = -1;
    }
  else if (ordinary >= 1)
    {
      if (ordinary < 60)
        ordinary = 60;
      if (start_generator_formula[0] == '\0')
        strcpy(start_generator_formula, "A1-");
      if (ending_generator_formula[0] == '\0')
        strcpy(ending_generator_formula, "A1-*");
      if (start_generator_formula[0] != 'A')
        usage(arg0);
      if (ending_generator_formula[0] != 'A')
        usage(arg0);
      symmetric = -1;
      tube = -1;
      test = -1;
    }
  else if (tube >= 1)
    {
      if (tube < 60)
        tube = 60;
      if (start_generator_formula[0] == '\0')
        strcpy(start_generator_formula, "T10,0,3-");
      if (ending_generator_formula[0] == '\0')
        strcpy(ending_generator_formula, "T10,0,3-*");
      if (start_generator_formula[0] != 'T')
        usage(arg0);
      if (ending_generator_formula[0] != 'T')
        usage(arg0);
      symmetric = -1;
      ordinary = -1;
      test = -1;
    }
  else if (test >= 1)
    {
      if (start_generator_formula[0] == '\0')
        strcpy(start_generator_formula, "T10,0,3-");
      if (ending_generator_formula[0] == '\0')
        strcpy(ending_generator_formula, "T10,0,3-*");
      if (start_generator_formula[0] != 'T')
        usage(arg0);
      if (ending_generator_formula[0] != 'T')
        usage(arg0);
      symmetric = -1;
      ordinary = -1;
      tube = -1;
    }
  else
    {
      fprintf(stderr, "%s: --symmetric=[num] --ordeinary=[num] --tube=[num] or --test"
              " must be specified.\n", arg0);
      exit(1);
    }

  print_version("ca-loop", fptr);
  while (1)
    {
      char command[BUFFER_SIZE];
      char *ptr = command;
      if (test > 0)
        sprintf(ptr, "ca-gentest");
      else
        sprintf(ptr, "ca-generator");
      ptr += strlen(ptr);
      if (symmetric > 0)
        sprintf(ptr, " --symmetric=%d", symmetric);
      else if (ordinary > 0)
        sprintf(ptr, " --ordinary=%d", ordinary);
      else if (tube > 0)
        sprintf(ptr, " --tube=%d", tube);
      else if (test > 0)
        sprintf(ptr, " --test");
      ptr += strlen(ptr);
      if (close != INT_MAX)
        {
          sprintf(ptr, " --close=%d", close);
          ptr += strlen(ptr);
        }
      switch (step_algorithm)
        {
        case STEP_ALGORITHM_COPY_BRANCH:
          sprintf(ptr, " --step-copy-branch");
          break;
        case STEP_ALGORITHM_FORWARD:
          sprintf(ptr, " --step-forward");
          break;
        case STEP_ALGORITHM_BACKWARD:
          sprintf(ptr, " --step-backward");
          break;
        }
      ptr += strlen(ptr);
      sprintf(ptr, " %s", start_generator_formula);
      fprintf(stderr, "%s\n", command);
      FILE* generator = popen(command, "r");
      if (generator == 0)
        {
          fprintf(stderr, "%s: fork/pipe/memory error\n", arg0);
          exit(1);
        }
      setvbuf(generator, 0, _IONBF, 0);
      int lines = 0;
      while (1)
        {
          char buffer[BUFFER_SIZE];
          if (fgets(buffer, BUFFER_SIZE, generator) == NULL)
            {
              if (lines == 0)
                exit(1);
              break;
            }
          int len = strlen(buffer);
          if ((len > 0) && (buffer[len - 1] == '\n'))
            buffer[len - 1] = '\0';
          lines++;
          if (buffer[0] == '#')
            continue;
          if (strncmp(buffer, "Warning:", 8) == 0)
            {
              fprintf(stderr, "skip ``%s''\n", buffer);
              continue;
            }
          if (buffer[0] != 'C')
            {
              fprintf(stderr, "unknown ``%s''\n", buffer);
              continue;
            }
          assert(buffer[0] == 'C');
          char* sptr = buffer;
          while (*sptr != ' ')
            ++sptr;
          ++sptr;
          while (*sptr != ' ')
            ++sptr;
          ++sptr;
          int result = compare_generator_formula(ending_generator_formula, sptr);
          if ((result < 0) || (except_ending && (result == 0)))
            {
              pclose(generator);
              fclose(fptr);
              exit(0);
            }
          strcpy(start_generator_formula, sptr);
          fputs(buffer, fptr);
          fputc('\n', fptr);
        }
      int result = pclose(generator);
      if (result == 0)
        {
          fclose(fptr);
          exit(0);
        }
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
