/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <string.h>
#include "Range.h"
#include "Utils.h"
#include "Process.h"
#include "Collector.h"
#include "Host.h"

void Range::p_decide_generator_check_and_level()
{
  p_generator_formula = p_start_formula;
  char* ptr = p_generator_formula;
  int len = strlen(ptr);
  while (ptr[len - 1] == '5')
    len--;
  p_generator_formula.resize(len);
  char* srcptr = p_generator_formula;
  MyString work;
  while (*srcptr && (*srcptr != '-'))
    work.append_char(*srcptr++);
  if (*srcptr == '-')
    work.append_char(*srcptr++);
  int current_No = 0;
  int count = 0;
  len = strlen(srcptr);
  for (int i = 0; i < len; ++i)
    {
      if (current_No == srcptr[i] - '0')
        ++count;
      else
        {
          if (count)
            No_No_to_digits10x10_digits26x7(current_No, count, work);
          current_No = srcptr[i] - '0';
          count = 1;
        }
    }
  No_No_to_digits10x10_digits26x7(current_No, count, work);
  p_generator_formula = work;

  p_check_formula = p_ending_formula;
  ptr = p_check_formula;
  len = strlen(ptr);
  while (ptr[len - 1] == '5')
    len--;
  p_check_formula.resize(len);

  p_level = 0;
  const char* ptr1 = p_start_formula;
  const char* ptr2 = p_ending_formula;
  while (*ptr1 && (*ptr1 != '-'))
    ++ptr1;
  if (*ptr1 == '-')
    ++ptr1;
  while (*ptr2 && (*ptr2 != '-'))
    ++ptr2;
  if (*ptr2 == '-')
    ++ptr2;
  while (*ptr1 && *ptr2)
    {
      p_level++;
      ptr1++;
      ptr2++;
    }
  assert(*ptr1 == 0);
  assert(*ptr2 == 0);

  set_progress_formula(p_start_formula);
}

Range::Range(Collector* collector, const char* start_formula, const char* ending_formula)
  : p_collector(collector),
    p_level(0), p_range_state(RANGE_STATE_PROCESS_UNASSIGNED),
    p_assigned_process(0), p_occupation_rate(1.0), p_progress(0.0), p_elapsed_sec(0.0)
{
  assert(start_formula);
  p_start_formula = start_formula;
  assert(ending_formula);
  p_ending_formula = ending_formula;
  p_decide_generator_check_and_level();
  p_last_formula = p_generator_formula;
}

Range::~Range()
{
}

void Range::assign_process(Process* process)
{
  p_range_state = RANGE_STATE_PROCESS_ASSIGNED;
  p_assigned_process = process;
  p_progress = 0.0;
  process->assign_range(this);
}

void Range::unassign_process()
{
  assert(p_assigned_process);
  p_assigned_process->unassign_range();
  p_assigned_process = 0;
  if (p_range_state == RANGE_STATE_PROCESS_ASSIGNED)
    {
      p_range_state = RANGE_STATE_PROCESS_UNASSIGNED;
      p_decide_generator_check_and_level();
      p_progress = 0.0;
    }
}

Range* Range::divide()
{
  if (p_range_state == RANGE_STATE_COLLECT_DONE)
    return 0;
  else if (p_range_state == RANGE_STATE_PROCESS_ASSIGNED)
    {
      MyString middle_formula = p_start_formula;
      middle_formula.append_char('6');
      int result = middle_formula.compare(&p_progress_formula);
      if (result > 0)
        { /* progress is under the middle_formula */
          p_occupation_rate /= 2.0;
          double progress_save = 2.0 * p_progress;
          Range* new_range = new Range(p_collector, p_start_formula, p_ending_formula);
          p_ending_formula = p_start_formula;
          p_start_formula.append_char('5');
          p_ending_formula.append_char('6');
          p_decide_generator_check_and_level();
          p_last_formula = p_generator_formula;
          p_assigned_process->send_level_command(p_level);
          p_progress = progress_save;
          new_range->p_start_formula.append_char('6');
          new_range->p_ending_formula.append_char('5');
          new_range->p_range_state = RANGE_STATE_PROCESS_UNASSIGNED;
          new_range->p_occupation_rate = p_occupation_rate;
          new_range->p_decide_generator_check_and_level();
          new_range->p_progress = 0.0;
          new_range->p_last_formula = new_range->p_generator_formula;
          return new_range;
        }
      else
        { /* progress is over the middle_formula */
          p_occupation_rate /= 2.0;
          double progress_save = 2.0 * p_progress - 1.0;
          Range* new_range = new Range(p_collector, p_start_formula, p_start_formula);
          new_range->p_start_formula.append_char('5');
          new_range->p_ending_formula.append_char('6');
          new_range->p_range_state = RANGE_STATE_COLLECT_DONE;
          new_range->p_occupation_rate = p_occupation_rate;
          new_range->p_decide_generator_check_and_level();
          new_range->p_last_formula = new_range->p_generator_formula;
          new_range->p_progress = 1.0;
          p_start_formula.append_char('6');
          p_ending_formula.append_char('5');
          p_decide_generator_check_and_level();
          p_last_formula = p_generator_formula;
          p_assigned_process->send_level_command(p_level);
          p_progress = progress_save;
          return new_range;
        }
    }
  else
    {
      p_occupation_rate /= 2.0;
      Range* new_range = new Range(p_collector, p_start_formula, p_start_formula);
      new_range->p_start_formula.append_char('5');
      new_range->p_ending_formula.append_char('6');
      new_range->p_range_state = RANGE_STATE_PROCESS_UNASSIGNED;
      new_range->p_occupation_rate = p_occupation_rate;
      new_range->p_decide_generator_check_and_level();
      new_range->p_last_formula = new_range->p_generator_formula;
      new_range->p_progress = 0.0;
      p_start_formula.append_char('6');
      p_ending_formula.append_char('5');
      p_decide_generator_check_and_level();
      p_last_formula = p_generator_formula;
      p_progress = 0.0;
      return new_range;
    }
}

int Range::compare(const Range* you) const
{
  if (p_collector != you->p_collector)
    return p_collector->compare(you->p_collector);
  return p_start_formula.compare(&you->p_start_formula);
}

void Range::print(FILE* output, bool verbose) const
{
  if (p_range_state == RANGE_STATE_COLLECT_DONE)
    {
      assert(p_progress == 1.0);
      fprintf(output, " <DONE>");
    }
  else if (p_range_state == RANGE_STATE_PROCESS_ASSIGNED)
    fprintf(output, "%6.2f%%", p_progress * 100.0);
  else
    {
      assert(p_progress == 0.0);
      fprintf(output, " ------");
    }
  fprintf(output, " in %5.2f%%", p_occupation_rate * 100.0);
  print_elapsed_sec(output, p_elapsed_sec);
  fprintf(output, " level=%d %s", p_level, (char*)p_start_formula);
  if (verbose)
    {
      fprintf(output, "(%s/%s) %s %s(%s)", (char*)p_generator_formula,
              (char*)p_last_formula, (char*)p_progress_formula,
              (char*)p_ending_formula, (char*)p_check_formula);
    }
  if (p_assigned_process)
    fprintf(output, " %s", p_assigned_process->get_host()->get_host_name());
  fprintf(output, "\n");
}

bool Range::set_last_formula(const char* buffer)
{
  if (*buffer != 'C')
    return false;
  while (*buffer && (*buffer != ' '))
    ++buffer;
  if (*buffer++ != ' ')
    return false;
  if (*buffer != '(')
    return false;
  while (*buffer && (*buffer != ')'))
    ++buffer;
  if (*buffer++ != ')')
    return false;
  if (*buffer++ != ' ')
    return false;
  p_last_formula = "";
  while (*buffer != '\n')
    p_last_formula.append_char(*buffer++);
  return true;
}

bool Range::set_progress_formula(const char* progress_formula)
{
  p_progress_formula = progress_formula;
  const char* ptr = p_progress_formula;
  while (*ptr && (*ptr != '-'))
    ++ptr;
  if (*ptr == '-')
    ++ptr;
  int result = ptr - p_progress_formula;
  int progress_level = p_progress_formula.length() - result;
  p_progress = 0.0;
  if (progress_level > p_level)
    {
      ptr += p_level;
      double geta = 0.5;
      while (*ptr)
        {
          if (*ptr == '6')
            p_progress += geta;
          geta /= 2.0;
          ++ptr;
        }
    }
  if (p_progress_formula.compare(&p_check_formula) < 0)
    return true;
  p_range_state = RANGE_STATE_COLLECT_DONE;
  p_progress = 1.0;
  if (p_collector)
    p_collector->check_done_and_call_done_waiters();
  return false;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
