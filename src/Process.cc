/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <signal.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Host.h"
#include "Range.h"
#include "List.h"
#include "Collector.h"
#include "PipeHandler.h"
#include "Parallel.h"

void Process::p_kill_process()
{
  if (p_forkpid >= 0)
    {
      fprintf(stderr, "kill process pid(%d) `%s'\n",
              (int)p_forkpid, (char*)p_command_line);
      kill(p_forkpid, SIGKILL);
    }
  p_forkpid = -1;
  /* p_readfd は PipeHandler がクローズしてくれる */
  p_readfd = -1;
  if (p_writefd >= 0)
    close(p_writefd);
  p_writefd = -1;
}

Process::Process(Host* host, int process_id, Parallel* manager)
  : p_host(host), p_process_id(process_id), p_manager(manager), p_pipe_handler(0),
    p_process_state(PROCESS_STATE_RANGE_UNASSIGNED), p_disabled(false),
    p_assigned_range(0), p_forkpid(-1), p_readfd(-1), p_writefd(-1)
{
  if (p_manager)
    p_manager->manage_unassigned_process(this);
}

Process::~Process()
{
  p_kill_process();
}

#define R (0)
#define W (1)

bool Process::assign_range(Range* range)
{
  assert(p_process_state == PROCESS_STATE_RANGE_UNASSIGNED);
  int pipe_me2you[2];
  int pipe_you2me[2];
  if (pipe(pipe_me2you) < 0)
    return false;
  if (pipe(pipe_you2me) < 0)
    {
      close(pipe_me2you[R]);
      close(pipe_me2you[W]);
      return false;
    }
  assert(get_host());
  const char* hostname = get_host()->get_host_name();
  assert(hostname);
  assert(range->get_collector());
  assert(range->get_start_formula());
  p_command_line = "";
  if (strcmp(hostname, "local") != 0)
    {
      p_command_line.append_string("ssh ");
      p_command_line.append_string(hostname);
      p_command_line.append_char(' ');
    }
  p_command_line.append_string("ca-collector");
  const List<ObjectString>& options = range->get_collector()->get_options();
  int len = options.length();
  for (int i = 0; i < len; ++i)
    {
      p_command_line.append_char(' ');
      p_command_line.append_string(*(options[i]));
    }
  p_command_line.append_string(" --parallel=");
  p_command_line.append_int(range->get_level() + 4);
  p_command_line.append_char(' ');
  p_command_line.append_string(range->get_last_formula());

  p_forkpid = fork();
  if (p_forkpid < 0)
    {
      close(pipe_me2you[R]);
      close(pipe_me2you[W]);
      close(pipe_you2me[R]);
      close(pipe_you2me[W]);
      return false;
    }
  if (p_forkpid == 0)
    { /* child process */
      close(pipe_me2you[W]);
      close(pipe_you2me[R]);
      dup2(pipe_me2you[R], 0);
      dup2(pipe_you2me[W], 1);
      close(pipe_me2you[R]);
      close(pipe_you2me[W]);
      execlp("sh", "sh", "-c", (char*)p_command_line, 0);
      exit(1);
    }
  else
    { /* parent process == ca-server */
      printf("fork process pid(%d) `%s'\n", (int)p_forkpid, (char*)p_command_line);
      close(pipe_me2you[R]);
      close(pipe_you2me[W]);
      p_readfd = pipe_you2me[R];
      p_writefd = pipe_me2you[W];
      p_process_state = PROCESS_STATE_RANGE_ASSIGNED;
      p_assigned_range = range;
      p_pipe_handler = new PipeHandler(this);
      if (p_manager)
        {
          p_manager->join(p_pipe_handler);
          p_manager->manage_unassigned_process(this);
        }
      return true;
    }
}

void Process::unassign_range()
{
  assert(p_process_state == PROCESS_STATE_RANGE_ASSIGNED);
  p_kill_process();
  p_process_state = PROCESS_STATE_RANGE_UNASSIGNED;
  p_assigned_range = 0;
  p_pipe_handler = 0;
  if (p_manager)
    p_manager->manage_unassigned_process(this);
}

void Process::disable()
{
  p_disabled = true;
  send_exit_command();
}

void Process::enable()
{
  p_disabled = false;
}

int Process::compare(const Process* you) const
{
  int host_compare = get_host()->compare(you->get_host());
  if (host_compare != 0)
    return host_compare;
  return process_id() - you->process_id();
}

void Process::print(FILE* output) const
{
  if (p_process_state == PROCESS_STATE_RANGE_UNASSIGNED)
    {
      if (p_disabled)
        fprintf(output, "<DISABLED>\n");
      else
        fprintf(output, "<RESTING>\n");
    }
  else
    {
      if (p_disabled)
        fprintf(output, "pid(%d) disabling `%s'\n", (int)p_forkpid, (char*)p_command_line);
      else
        fprintf(output, "pid(%d) `%s'\n", (int)p_forkpid, (char*)p_command_line);
    }

}

void Process::send_level_command(int level)
{
  MyString command("level=");
  command.append_int(level + 4);
  command.append_char('\n');
  write(p_writefd, (char*)command, command.length());
}

void Process::send_exit_command()
{
  MyString command("exit\n");
  write(p_writefd, (char*)command, command.length());
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
