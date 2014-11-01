/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Parallel.h"
#include "ReadSelector.h"
#include "ListenHandler.h"
#include "ScheduleHandler.h"
#include "DebugMemory.h"
#include "Utils.h"

AssignProcessResult Parallel::p_assign_process(Process* process, bool force)
{
  int len = p_collectors.length();
  AssignProcessResult result = ASSIGN_PROCESS_RESULT_COLLECT_DONE;
  for (int i = 0; i < len; ++i)
    {
      AssignProcessResult result2 = p_collectors[i]->assign_process(process, force);
      if (result2 == ASSIGN_PROCESS_RESULT_SUCCESS)
        return result2;
      if (result2 == ASSIGN_PROCESS_RESULT_FAIL)
        result = result2;
    }
  return result;
}

void Parallel::p_enable_processes(Host* searched_host, int from, int to)
{
  for (int i = from; i < to; ++i)
    {
      Process* searched_process = search_process(searched_host, i);
      assert(searched_process);
      searched_process->enable();
      manage_unassigned_process(searched_process);
    }
}

void Parallel::p_disable_processes(Host* searched_host, int from, int to)
{
  for (int i = from; i < to; ++i)
    {
      Process* searched_process = search_process(searched_host, i);
      assert(searched_process);
      searched_process->disable();
      manage_unassigned_process(searched_process);
    }
}

Parallel::Parallel(const MyString& home)
  : p_server_pipe_name(home), p_server_lockfile_name(home),
    p_server_logfile_name(home), p_client_pipe_base(home), p_selector(0)
{
  int len = home.length();
  if (home[len - 1] != '/')
    {
      p_server_pipe_name.append_char('/');
      p_server_lockfile_name.append_char('/');
      p_server_logfile_name.append_char('/');
      p_client_pipe_base.append_char('/');
    }
  p_server_pipe_name.append_string("tmp");
  p_server_lockfile_name.append_string("tmp");
  p_server_logfile_name.append_string("log");
  p_client_pipe_base.append_string("tmp");
  mkdir((char*)p_server_pipe_name, 0755);
  mkdir((char*)p_server_logfile_name, 0755);
  p_server_pipe_name.append_string("/ca-server");
  p_server_lockfile_name.append_string("/ca-lockfile");
  p_server_logfile_name.append_string("/ca-server.log");

  signal(SIGPIPE, SIG_IGN);
  bool lockfile_locked = true;
  for (int count = 0; count < 5; ++count)
    {
      int fd = open((char*)p_server_lockfile_name,
                    O_EXLOCK | O_RDONLY | O_CREAT | O_NONBLOCK, 0660);
      if (fd >= 0)
        {
          lockfile_locked = false;
          break;
        }
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = 1;
      nanosleep(&ts, 0);
    }
  if (lockfile_locked)
    exit(0);
  struct stat sb;
  bool do_mkfifo = false;
  if (stat((char*)p_server_pipe_name, &sb) == 0)
    {
      if (!S_ISFIFO(sb.st_mode))
        {
          unlink((char*)p_server_pipe_name);
          do_mkfifo = true;
        }
    }
  else
    do_mkfifo = true;
  if (do_mkfifo)
    if (mkfifo((char*)p_server_pipe_name, 0770) != 0)
      {
        perror("mkfifo");
        exit(1);
      }
  p_selector = new ReadSelector();
  daemon(0, 0);
  int out_fd = open((char*)p_server_logfile_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
  if (out_fd == -1)
    {
      perror("open");
      exit(1);
    }
  dup2(out_fd, 1);
  dup2(out_fd, 2);
  close(out_fd);
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);
}

Parallel::~Parallel()
{
  unlink(p_server_pipe_name);
}

bool Parallel::assign_processes(bool force)
{
  int count = 0;
  while (1)
    {
      int len = p_unassigned_processes.length();
      if (len == 0)
        return true;
      Process* process = p_unassigned_processes[len - 1];
      AssignProcessResult result = p_assign_process(process, force);
      if (result == ASSIGN_PROCESS_RESULT_COLLECT_DONE)
        return true;
      if (result == ASSIGN_PROCESS_RESULT_SUCCESS)
        continue;
      if (force == false)
        return false;
      ++count;
      assert(count < 10);
    }
}

void Parallel::manage_unassigned_process(Process* process)
{
  if (process->is_disabled())
    {
      p_unassigned_processes.remove(process);
    }
  else
    {
      if (process->get_process_state() == PROCESS_STATE_RANGE_ASSIGNED)
        {
          bool result = p_unassigned_processes.remove(process);
          assert(result == true);
        }
      else
        {
          Process* searched_process = p_unassigned_processes.search_else_add(process);
          assert(searched_process == 0);
        }
    }
}

void Parallel::command_loop()
{
  ListenHandler* lhandler = new ListenHandler(this);
  bool bresult = join(lhandler);
  assert(bresult);
  ScheduleHandler* shandler = new ScheduleHandler();
  bresult = join(shandler);
  assert(bresult);
  bool exit_required = false;
  while (1)
    {
      if (exit_required)
        {
          int number = p_selector->get_number_of_handlers();
          if (number == 0)
            {
              printf("ca-server is exited\n");
              return;
            }
        }
      ReadHandlerResult result = p_selector->select(INT_MAX);
      if (result == READ_HANDLER_RESULT_CONTINUED)
        /* succeeded and continue loop */;
      else if (result == READ_HANDLER_RESULT_TERMINATE)
        {
          /* this enum never returned */
          assert(0);
        }
      else if (result == READ_HANDLER_RESULT_SCHEDULE)
        assign_processes(false);
      else if (result == READ_HANDLER_RESULT_SCHEDULE_FORCE)
        assign_processes(true);
      else if (result == READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE)
        {
          /* this enum never returned */
          assert(0);
        }
      else if (result == READ_HANDLER_RESULT_TIMEOUT)
        {
          /* timeout */
          continue;
        }
      else if (result == READ_HANDLER_RESULT_EXIT)
        {
          p_selector->defect(shandler);
          p_selector->defect_listen_handler();
          disable_all_processes();
          exit_required = true;
          continue;
        }
      else
        {
          /* others never returned */
          assert(0);
        }
    }
}

void Parallel::print_hosts(FILE* output) const
{
  int len = p_hosts.length();
  for (int i = 0; i < len; ++i)
    {
      fprintf(output, "host[%d] = ", i);
      p_hosts[i]->print(output);
    }
}

bool Parallel::print_ranges(FILE* output, int index) const
{
  int len = p_collectors.length();
  if (index < 0)
    return false;
  if (index >= len)
    return false;
  Collector* collector = p_collectors[index];
  return print_ranges(output, collector);
}

bool Parallel::print_ranges(FILE* output, Collector* collector) const
{
  int len = collector->number_of_ranges();
  for (int i = 0; i < len; ++i)
    {
      fprintf(output, "   range[%3d] =", i);
      collector->get_range(i)->print(output);
    }
  return true;
}

bool Parallel::print_processes(FILE* output, int host_index)
{
  int len = p_hosts.length();
  if (host_index < 0)
    return false;
  if (host_index >= len)
    return false;
  Host* host = p_hosts[host_index];
  fprintf(output, "host[%d] = ", host_index);
  host->print(output);
  len = host->number_of_processes();
  for (int i = 0; i < len; ++i)
    {
      fprintf(output, "    process[%d] = ", i);
      const Process* searched_process = search_process(host, i);
      assert(searched_process);
      searched_process->print(output);
    }
  return true;
}

void Parallel::print_processes(FILE* output)
{
  int len = p_hosts.length();
  for (int i = 0; i < len; ++i)
    {
      bool result = print_processes(output, i);
      assert(result == true);
    }
}

void Parallel::print_collectors(FILE* output, bool ranges_too) const
{
  int len = p_collectors.length();
  for (int i = 0; i < len; ++i)
    {
      fprintf(output, "collector[%1d] = ", i);
      const Collector* collector = p_collectors[i];
      collector->print(output);
      if (ranges_too)
        print_ranges(output, i);
    }
}

bool Parallel::join(ReadHandler* rhandler)
{
  return p_selector->join(rhandler);
}

void Parallel::set_client_cwd(const char* client_cwd)
{
  p_client_cwd = client_cwd;
}

bool Parallel::add_host(const char* host_name, int process_num)
{
  Host* new_host = new Host(host_name, process_num);
  if (p_hosts.search_else_add(new_host))
    {
      delete new_host;
      return false;
    }
  for (int i = 0; i < process_num; ++i)
    {
      Process* new_process = new Process(new_host, i, this);
      Process* searched_process = p_processes.search_else_add(new_process);
      assert(searched_process == 0);
    }
  return true;
}

Host* Parallel::search_host(const char* host_name)
{
  Host searching_host = Host(host_name, 1);
  return p_hosts.search(&searching_host);
}

bool Parallel::enable_processes(const char* host_name, int number)
{
  if (number < 0)
    return false;
  Host* searched_host = search_host(host_name);
  if (!searched_host)
    return false;
  int last_number = searched_host->number_of_enabled_processes();
  if (searched_host->enable_processes(number) == false)
    return false;
  int next_number = searched_host->number_of_enabled_processes();
  if (last_number == next_number)
    return true;
  if (last_number < next_number)
    p_enable_processes(searched_host, last_number, next_number);
  else
    p_disable_processes(searched_host, next_number, last_number);
  return true;
}

void Parallel::disable_all_processes()
{
  int len = p_processes.length();
  for (int i = 0; i < len; ++i)
    {
      Process* process = p_processes[i];
      assert(process);
      process->disable();
    }
}

Process* Parallel::search_process(Host* host, int process_id)
{
  Process searching_process(host, process_id, 0);
  return p_processes.search(&searching_process);
}

bool Parallel::add_collector(Collector* collector)
{
  int len = p_collectors.length();
  for (int i = 0; i < len; ++i)
    {
      if (p_collectors[i]->compare(collector) == 0)
        return false;
    }
  p_collectors.add(collector);
  return true;
}

int Parallel::search_collector(const char* output_filename)
{
  int len = p_collectors.length();
  for (int i = 0; i < len; ++i)
    {
      Collector* searched_collector = p_collectors[i];
      if (strcmp(searched_collector->get_output_filename(), output_filename) == 0)
        return i;
    }
  return -1;
}

bool Parallel::remove_collector(int index)
{
  return p_collectors.remove(index);
}

bool Parallel::remove_collector(const char* output_filename)
{
  int len = p_collectors.length();
  for (int i = 0; i < len; ++i)
    {
      Collector* searched_collector = p_collectors[i];
      if (strcmp(searched_collector->get_output_filename(), output_filename) == 0)
        return p_collectors.remove(i);
    }
  return false;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
