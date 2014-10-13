/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Search.h"
#include "Utils.h"

Search::Search()
  : p_number(0), p_outfd(-1)
{
}

Search::~Search()
{
  if (p_outfd >= 0)
    close(p_outfd);
}

AssignProcessResult Search::assign_process(Process* process)
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
  /* 割り当てレンジのうち、進捗が一番遅れているものを探す */
  double min_progress = 1.0;
  search_index = -1;
  for (int i = 0; i < len; ++i)
    {
      Range* range = p_ranges[i];
      if (range->get_range_state() != RANGE_STATE_PROCESS_ASSIGNED)
        continue;
      if (range->get_progress() < min_progress)
        {
          min_progress = range->get_progress();
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

UnassignProcessResult Search::unassign_process(Process* process)
{
  /* 指定プロセスに割り当っているレンジを探す */
  int found_index = -1;
  int len = p_ranges.length();
  for (int i = 0; i < len; ++i)
    {
      Range* range = p_ranges[i];
      if (range->get_range_state() == RANGE_STATE_PROCESS_ASSIGNED)
        if (range->get_assigned_process() == process)
          {
            found_index = i;
            break;
          }
    }
  if (found_index == -1)
    {
      /* そのプロセスはどのレンジにも割り当たっていない */
      return UNASSIGN_PROCESS_RESULT_FAIL;
    }
  /* 見つかったレンジが、進捗率 0.0 になるまで分割する */
  Range* range = p_ranges[found_index];
  int debug_count = 0;
  while (range->get_progress() > 0.0)
    {
      assert(debug_count < 10);
      Range* new_range = range->divide();
      Range* searched_range = p_ranges.search_else_add(new_range);
      assert(searched_range == 0);
      ++debug_count;
    }
  range->delete_PipeHandler_then_unassign_process();
  /* プロセス解除成功 */
  return UNASSIGN_PROCESS_RESULT_SUCCESS;
}

int Search::compare(const Search* you) const
{
  return strcmp(get_output_filename(), you->get_output_filename());
}

void Search::print(FILE* output) const
{
  fprintf(output, "%d %6.2f%%", p_number, get_progress() * 100.0);
  print_elapsed_sec(output, get_elapsed_sec());
  const List<ObjectString>& options = get_options();
  int len = options.length();
  for (int i = 0; i < len; ++i)
    fprintf(output, " %s", (char*)(*options[i]));
  fprintf(output, " --out=%s %s\n", get_output_filename(), get_formula_segment());
}

bool Search::collect(const char* line, int length)
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

void Search::add_option(const char* option)
{
  p_options.add(new ObjectString(option));
}

bool Search::add_formula_segment(const char* formula_segment)
{
  int len = strlen(formula_segment);
  if ((len > 0) && (formula_segment[len - 1] == '-'))
    {
      p_formula_segment = formula_segment;
      return true;
    }
  else
    return false;
}

void Search::add_output_filename(const char* output_filename)
{
  p_output_filename = output_filename;
}

bool Search::setup()
{
  MyString formula5 = p_formula_segment;
  MyString formula6 = p_formula_segment;
  MyString formula7 = p_formula_segment;
  formula5.append_char('5');
  formula6.append_char('6');
  formula7.append_char('7');
  Range* range = new Range(this, formula5, formula6);
  range->set_occupation_rate(0.5);
  Range* searched_range = p_ranges.search_else_add(range);
  assert(searched_range == 0);
  range = new Range(this, formula6, formula7);
  range->set_occupation_rate(0.5);
  searched_range = p_ranges.search_else_add(range);
  assert(searched_range == 0);
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

double Search::get_progress() const
{
  double result = 0.0;
  int len = p_ranges.length();
  for (int i = 0; i < len; ++i)
    {
      const Range* range = get_range(i);
      double occ = range->get_occupation_rate();
      double pro = range->get_progress();
      result += occ * pro;
    }
  return result;
}

double Search::get_elapsed_sec() const
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

const Range* Search::get_range(int index) const
{
  if (index < 0)
    return 0;
  if (index >= p_ranges.length())
    return 0;
  return p_ranges[index];
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
