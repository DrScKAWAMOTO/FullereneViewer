/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "PipeHandler.h"
#include "Process.h"
#include "Range.h"
#include "Collector.h"

bool PipeHandler::p_receive()
{
  int red_len;
  red_len = read(p_readfd, p_buffer + p_data_len, BUFFER_SIZE - p_data_len);
  if (red_len <= 0)
    return false;
  p_data_len += red_len;
  p_line_top = 0;
  p_line_len = 0;
  return true;
}

bool PipeHandler::p_read_line()
{
  p_line_top += p_line_len;
  int index = p_line_top;
  while (index < p_data_len)
    if (p_buffer[index] == '\n')
      break;
    else
      ++index;
  if (index < p_data_len)
    {
      p_line_len = index - p_line_top + 1;
      return true;
    }
  memmove(p_buffer, p_buffer + p_line_top, p_data_len - p_line_top);
  p_data_len -= p_line_top;
  p_line_top = 0;
  p_line_len = 0;
  return false;
}

PipeHandler::PipeHandler(Process* process)
  : ReadHandler(), p_data_len(0), p_line_top(0), p_line_len(0), p_process(process)
{
  p_readfd = p_process->get_readfd();
  p_assigned_range = p_process->get_assigned_range();
  assert(p_assigned_range);
  p_assigned_collector = p_assigned_range->get_collector();
  assert(p_assigned_collector);
}

PipeHandler::~PipeHandler()
{
  p_assigned_range->unassign_process();
}

ReadHandlerResult PipeHandler::call()
{
  if (p_receive() == false)
    {
      printf("pipe disconnected\n");
      return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
    }
  while (p_read_line())
    {
      if ((p_assigned_collector == 0) || (p_assigned_range == 0) || (p_process == 0))
        {
          printf("ignored over(or under)-running !!\n");
          continue;
        }
      if (strncmp(p_buffer + p_line_top, "pg$ ", 4) == 0)
        {
          p_buffer[p_line_top + p_line_len - 1] = '\0';
          char progress[1000] = "";
          double elapsed = 0.0;
          sscanf(p_buffer + p_line_top + 4, "%s %lf", progress, &elapsed);
          printf("  pid(%d) %s\n", p_process->get_forkpid(), p_buffer + p_line_top + 4);
          p_assigned_range->set_elapsed_sec(elapsed);
          if (p_assigned_range->set_progress_formula(progress) == false)
            return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
        }
      else
        {
          if (p_assigned_range->set_last_formula(p_buffer + p_line_top))
            p_assigned_collector->collect(p_buffer + p_line_top, p_line_len);
        }
    }
  return READ_HANDLER_RESULT_CONTINUED;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
