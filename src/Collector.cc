/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Collector.h"
#include "Utils.h"
#include "Generator.h"
#include "Parallel.h"
#include "AcceptHandler.h"

Collector::Collector()
  : p_number(0), p_outfd(-1)
{
}

Collector::~Collector()
{
  if (p_outfd >= 0)
    close(p_outfd);
}

AssignProcessResult Collector::assign_process(Process* process, bool force)
{
  /* 未割り当てレンジのうち、レンジの大きさが一番大きいものを探す */
  double max_occupation = 0.0;
  int search_index = -1;
  int len = p_ranges.length();
  for (int i = 0; i < len; ++i)
    {
      Range* range = p_ranges[i];
      if (range->get_range_state() != RANGE_STATE_PROCESS_UNASSIGNED)
        continue;
      if (range->get_occupation_rate() > max_occupation)
        {
          max_occupation = range->get_occupation_rate();
          search_index = i;
        }
    }
  if (search_index >= 0)
    {
      p_ranges[search_index]->assign_process(process);
      /* プロセス割り当て成功 */
      return ASSIGN_PROCESS_RESULT_SUCCESS;
    }
  if (force == false)
    return ASSIGN_PROCESS_RESULT_FAIL;
  /* 割り当てレンジのうち、進捗残りが一番多いものを探す */
  double max_rest = 0.0;
  search_index = -1;
  for (int i = 0; i < len; ++i)
    {
      Range* range = p_ranges[i];
      if (range->get_range_state() != RANGE_STATE_PROCESS_ASSIGNED)
        continue;
      double my_occupation = range->get_occupation_rate();
      double my_progress = range->get_progress();
      double my_rest = (1.0 - my_progress) * my_occupation;
      if (my_rest > max_rest)
        {
          max_rest = my_rest;
          search_index = i;
        }
    }
  if (search_index >= 0)
    {
      /* 見つかったレンジを、分割する */
      Range* range = p_ranges[search_index];
      Range* new_range = range->divide();
      Range* searched_range = p_ranges.search_else_add(new_range);
      assert(searched_range == 0);
      if (new_range->get_range_state() == RANGE_STATE_PROCESS_UNASSIGNED)
        { /* 分割した相手のレンジが未割り当てだった場合 */
          new_range->assign_process(process);
          /* プロセス割り当て成功 */
          return ASSIGN_PROCESS_RESULT_SUCCESS;
        }
      else
        { /* 分割した相手のレンジが収集完了だった場合 */
          /* 収集中だがプロセス割り当て失敗 */
          return ASSIGN_PROCESS_RESULT_FAIL;
        }
    }
  /* 割り当てられるレンジが無かった場合 */
  /* 収集完了で、プロセスを割り当てなかった */
  return ASSIGN_PROCESS_RESULT_COLLECT_DONE;
}

int Collector::compare(const Collector* you) const
{
  return strcmp(get_output_filename(), you->get_output_filename());
}

void Collector::print(FILE* output) const
{
  bool done;
  double progress;
  progress = get_progress(done) * 100.0;
  if (done)
    fprintf(output, "%d <DONE>", p_number);
  else
    fprintf(output, "%d %6.2f%%", p_number, progress);
  print_elapsed_sec(output, get_elapsed_sec());
  const List<ObjectString>& options = get_options();
  int len = options.length();
  for (int i = 0; i < len; ++i)
    fprintf(output, " %s", (char*)(*options[i]));
  fprintf(output, " --out=%s", get_output_filename());
  const List<ObjectString>& formula_segments = get_formula_segments();
  len = formula_segments.length();
  for (int i = 0; i < len; ++i)
    fprintf(output, " %s", (char*)(*formula_segments[i]));
  fprintf(output, "\n");
}

bool Collector::collect(const char* line, int length)
{
  bool commented = (*line == '#');
  int write_len = length;
  while (write_len > 0)
    {
      int wrote_len = write(p_outfd, line, write_len);
      if (wrote_len == -1)
        return false;
      write_len -= wrote_len;
      line += wrote_len;
    }
  if (!commented)
    p_number++;
  return true;
}

void Collector::add_option(const char* option)
{
  p_options.add(new ObjectString(option));
}

bool Collector::add_formula_segment(const char* formula_segment)
{
  Generator gen(formula_segment);
  if (gen.type() == GENERATOR_TYPE_ILLEGAL)
    return false;
  MyString genstring;
  gen.get_generator_formula(genstring, false);
  p_formula_segments.add(new ObjectString((char*)genstring));
  return true;
}

bool Collector::setup_formula_segments()
{
  int len = p_formula_segments.length();
  assert(len > 0);
  double occupation_rate = 1.0 / (double)len;
  for (int i = 0; i < len; ++i)
    {
      ObjectString* os = p_formula_segments[i];
      const char* top = (char*)*os;
      const char* ptr = top;
      int toplen = strlen(top);
      assert(toplen > 0);
      while (*ptr && (*ptr != '-'))
        ++ptr;
      if (*ptr == '-')
        ++ptr;
      if (*ptr == '\0')
        {
          MyString formula5 = (char*)*os;
          MyString formula6 = (char*)*os;
          MyString formula7 = (char*)*os;
          formula5.append_char('5');
          formula6.append_char('6');
          formula7.append_char('7');
          Range* range = new Range(this, formula5, formula6);
          range->set_occupation_rate(0.5 * occupation_rate);
          Range* searched_range = p_ranges.search_else_add(range);
          assert(searched_range == 0);
          range = new Range(this, formula6, formula7);
          range->set_occupation_rate(0.5 * occupation_rate);
          searched_range = p_ranges.search_else_add(range);
          assert(searched_range == 0);
        }
      else
        return false;
    }
  return true;
}

void Collector::add_output_filename(const char* output_filename)
{
  p_output_filename = output_filename;
}

bool Collector::setup_output_file()
{
  if (p_outfd >= 0)
    close(p_outfd);
  p_outfd = open(p_output_filename, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
  if (p_outfd < 0)
    return false;
  char* rpath = realpath(p_output_filename, 0);
  p_output_filename = rpath;
  free(rpath);
  return true;
}

double Collector::get_progress(bool& done) const
{
  double result = 0.0;
  int len = p_ranges.length();
  done = true;
  for (int i = 0; i < len; ++i)
    {
      const Range* range = get_range(i);
      if (range->get_range_state() != RANGE_STATE_COLLECT_DONE)
        done = false;
      double occ = range->get_occupation_rate();
      double pro = range->get_progress();
      result += occ * pro;
    }
  return result;
}

double Collector::get_elapsed_sec() const
{
  double result = 0.0;
  int len = p_ranges.length();
  for (int i = 0; i < len; ++i)
    {
      const Range* range = get_range(i);
      double sec = range->get_elapsed_sec();
      result += sec;
    }
  return result;
}

void Collector::add_range(Range* range)
{
  Range* searched_range = p_ranges.search_else_add(range);
  assert(searched_range == 0);
}

const Range* Collector::get_range(int index) const
{
  if (index < 0)
    return 0;
  if (index >= p_ranges.length())
    return 0;
  return p_ranges[index];
}

void Collector::enter_done_waiter(int pid)
{
  printf("enter_done_waiter(%d)\n", pid);
  ObjectInt* entry = new ObjectInt(pid);
  p_done_waiters.add(entry);
}

void Collector::check_done_and_call_done_waiters()
{
  /* 全部 DONE かどうかチェックする */
  int len = p_ranges.length();
  for (int i = 0; i < len; ++i)
    {
      Range* range = p_ranges[i];
      if (range->get_range_state() != RANGE_STATE_COLLECT_DONE)
        return;
    }
  /* メッセージ作成 */
  MyString message("collecting done\n");
  /* 全 done_waiters を呼び出し */
  len = p_done_waiters.length();
  for (int i = 0; i < len; ++i)
    {
      ObjectInt* waiter = p_done_waiters[i];
      printf("AcceptHandler::call_callback(...)\n");
      AcceptHandler::call_callback(waiter->get_value(), message);
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
