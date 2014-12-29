/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Version.h"
#include "Object.h"
#include "Queue.h"

class Test : public Object {
public:
  int p_value;
  Test(int value) : p_value(value) { }
  ~Test() { }
  void print() const { printf("%d\n", p_value); }
};

void print(Queue<Test>& queue)
{
  while (1)
    {
      Test* ptr = queue.dequeue();
      if (!ptr)
        return;
      ptr->print();
    }
}

static void fill_test(Queue<Test>& queue, int& given_value, int n)
{
  for (int i = 0; i < n; ++i)
    queue.enqueue(new Test(given_value++));
}

static void check_test(Queue<Test>& queue, int& expect_value, int n)
{
  for (int i = 0; i < n; ++i)
    {
      Test* work = queue.dequeue();
      assert(work->p_value == expect_value++);
    }
  Test* result = queue.dequeue();
  assert(result == 0);
}

static void test_1_sub(int pre, int n, int count)
{
  Queue<Test> foo;
  int given_value = 0;
  int expect_value = 0;
  fill_test(foo, given_value, pre);
  check_test(foo, expect_value, pre);
  for (int i = 0; i < count; ++i)
    {
      fill_test(foo, given_value, n);
      check_test(foo, expect_value, n);
    }
}

static void test_1(int pre_max, int n_max, int count)
{
  for (int pre = 0; pre <= pre_max; ++pre)
    for (int n = 0; n <= n_max; ++n)
      test_1_sub(pre, n, count);
}

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
          print_version("ca-test");
          exit(0);
        }
      else
        usage(arg0);
    }

  test_1(9, 11, 100);
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
