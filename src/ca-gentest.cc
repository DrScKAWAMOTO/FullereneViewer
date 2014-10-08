/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Version.h"
#include "Utils.h"

enum Type {
  TYPE_TERMINATE = 1,
  TYPE_PENTAGON = 2,
  TYPE_HEXAGON = 3,
};

class Test {
private:
  int p_length;
  char p_array[8];

public:
  Test();
  ~Test();
  bool next();
  void print() const;
  void compressed(char* buffer) const;
  void compressed_print() const;
};

Test::Test()
  : p_length(2)
{
  p_array[0] = TYPE_PENTAGON;
  p_array[1] = TYPE_TERMINATE;
}

Test::~Test()
{
}

bool Test::next()
{
  if (p_length == 8)
    p_array[p_length - 1] = TYPE_HEXAGON;

  while (p_array[p_length - 1] == TYPE_HEXAGON)
    {
      if (p_length == 1)
        return false;
      --p_length;
    }
  if (p_array[p_length - 1] == TYPE_PENTAGON)
    {
      p_array[p_length - 1] = TYPE_HEXAGON;
      ++p_length;
      p_array[p_length - 1] = TYPE_TERMINATE;
      return true;
    }
  if (p_array[p_length - 1] == TYPE_TERMINATE)
    {
      p_array[p_length - 1] = TYPE_PENTAGON;
      ++p_length;
      p_array[p_length - 1] = TYPE_TERMINATE;
      return true;
    }
  return false;
}

void Test::print() const
{
  printf("C100 (NoA=10) T10,0,3-");
  for (int i = 0; i < p_length; ++i)
    {
      if (p_array[i] == TYPE_PENTAGON)
        printf("5");
      else if (p_array[i] == TYPE_HEXAGON)
        printf("6");
      else if (p_array[i] == TYPE_TERMINATE)
        printf("\n");
    }
}

void Test::compressed(char* buffer) const
{
  sprintf(buffer, "C100 (NoA=10) T10,0,3-");
  char *ptr = buffer + strlen(buffer);
  for (int i = 0; i < p_length; ++i)
    {
      int No;
      if (p_array[i] == TYPE_PENTAGON)
        No = 5;
      else if (p_array[i] == TYPE_HEXAGON)
        No = 6;
      else
        break;
      int length = 0;
      int j;
      for (j = i; j < p_length; ++j)
        {
          if (p_array[i] == p_array[j])
            ++length;
          else
            break;
        }
      No_to_digits10x10(No, ptr);
      No_to_digits26x7(length, ptr);
      i = j - 1;
    }
  *ptr++ = '\0';
}

void Test::compressed_print() const
{
  char buffer[100];
  compressed(buffer);
  fprintf(stdout, "%s\n", buffer);
}

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options] <generator-formula>\n", arg0);
  fprintf(stderr, "    <generator-formula> .. list up from this formula,\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    --symmetric=[num] .... ignored,\n");
  fprintf(stderr, "    --ordinary=[num] ..... ignored,\n");
  fprintf(stderr, "    --tube=[num] ......... ignored,\n");
  fprintf(stderr, "    --close=[num] ........ ignored,\n");
  fprintf(stderr, "    --step-and-copy ...... ignored,\n");
  fprintf(stderr, "    --step-to-collect .... ignored,\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  const char* arg0 = argv[0];
  const char* generator_formula = 0;
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
          print_version("ca-gentest");
          exit(0);
        }
      else if (strncmp(argv[0], "-", 1) == 0)
        {
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
  if (!generator_formula)
    usage(arg0);

  Test test;
  bool start = false;
  if (strcmp(generator_formula, "T10,0,3-") == 0)
    start = true;
  print_version("ca-gentest", stdout);
  while (1)
    {
      char buffer[100];
      test.compressed(buffer);
      if (start == false)
        {
          if (strcmp(buffer + 14, generator_formula) == 0)
            start = true;
        }
      if (start)
        fprintf(stdout, "%s\n", buffer);
      if (test.next() == false)
        return 0;
    }
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
