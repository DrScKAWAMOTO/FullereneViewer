/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

AssignProcessResult Parallel::p_assign_process(Process* process)
{
  int len = p_searches.length();
  AssignProcessResult result = ASSIGN_PROCESS_RESULT_COLLECT_DONE;
  for (int i = 0; i < len; ++i)
    {
      AssignProcessResult result2 = p_searches[i]->assign_process(process);
      if (result2 == ASSIGN_PROCESS_RESULT_SUCCESS)
        return result2;
      if (result2 == ASSIGN_PROCESS_RESULT_FAIL)
        result = result2;
    }
  return result;
}

UnassignProcessResult Parallel::p_unassign_process(Process* process)
{
  int len = p_searches.length();
  for (int i = 0; i < len; ++i)
    {
      UnassignProcessResult result = p_searches[i]->unassign_process(process);
      if (result == UNASSIGN_PROCESS_RESULT_SUCCESS)
        return result;
    }
  return UNASSIGN_PROCESS_RESULT_FAIL;
}

void Parallel::p_remove_processes(Host* searched_host, int from, int to)
{
  for (int i = from; i < to; ++i)
    {
      Process* searched_process = search_process(searched_host, i);
      assert(searched_process);
      if (searched_process->get_process_state()  == PROCESS_STATE_RANGE_ASSIGNED)
        {
          UnassignProcessResult result = p_unassign_process(searched_process);
          assert(result == UNASSIGN_PROCESS_RESULT_SUCCESS);
        }
      bool result = p_processes.remove(searched_process);
      assert(result == true);
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

bool Parallel::assign_processes()
{
  int count = 0;
  while (1)
    {
      int len = p_unassigned_processes.length();
      if (len == 0)
        return true;
      Process* process = p_unassigned_processes[len - 1];
      AssignProcessResult result = p_assign_process(process);
      if (result == ASSIGN_PROCESS_RESULT_COLLECT_DONE)
        return true;
      if (result == ASSIGN_PROCESS_RESULT_FAIL)
        ++count;
      assert(count < 10);
    }
}

void Parallel::manage_unassigned_process(Process* process)
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

void Parallel::command_loop()
{
  ListenHandler* lhandler = new ListenHandler(this);
  bool bresult = join(lhandler);
  assert(bresult);
  ScheduleHandler* shandler = new ScheduleHandler();
  bresult = join(shandler);
  assert(bresult);
  while (1)
    {
      ReadHandlerResult result = p_selector->select(INT_MAX);
      if (result == READ_HANDLER_RESULT_CONTINUED)
        /* succeeded and continue loop */;
      else if (result == READ_HANDLER_RESULT_TERMINATE)
        {
          /* this enum never returned */
          assert(0);
        }
      else if (result == READ_HANDLER_RESULT_SCHEDULE)
        assign_processes();
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
          p_selector->defect_all();
          printf("ca-server is exited\n");
          return;
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

bool Parallel::print_ranges(FILE* output, int index, int indent) const
{
  int len = p_searches.length();
  if (index < 0)
    return false;
  if (index >= len)
    return false;
  const Search* search = p_searches[index];
  len = search->number_of_ranges();
  for (int i = 0; i < len; ++i)
    {
      for (int j = 0; j < indent; ++j)
        fprintf(output, " ");
      fprintf(output, "range[%d] = ", i);
      search->get_range(i)->print(output);
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

void Parallel::print_searches(FILE* output, bool ranges_too) const
{
  int len = p_searches.length();
  for (int i = 0; i < len; ++i)
    {
      fprintf(output, "search[%1d] = ", i);
      const Search* search = p_searches[i];
      search->print(output);
      if (ranges_too)
        print_ranges(output, i, 4);
    }
}

bool Parallel::join(ReadHandler* rhandler)
{
  return p_selector->join(rhandler);
}

bool Parallel::defect(ReadHandler* rhandler)
{
  return p_selector->defect(rhandler);
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

bool Parallel::remove_host(const char* host_name)
{
  Host* searched_host = search_host(host_name);
  if (!searched_host)
    return false;
  int len = searched_host->number_of_processes();
  p_remove_processes(searched_host, 0, len);
  return p_hosts.remove(searched_host);
}

Host* Parallel::search_host(const char* host_name)
{
  Host searching_host = Host(host_name, 1);
  return p_hosts.search(&searching_host);
}

bool Parallel::add_processes(const char* host_name, int number)
{
  if (number <= 0)
    return false;
  Host* searched_host = search_host(host_name);
  if (!searched_host)
    return false;
  int last_number = searched_host->number_of_processes();
  searched_host->add_process(number);
  int curr_number = searched_host->number_of_processes();
  for (int i = last_number; i < curr_number; ++i)
    {
      Process* new_process = new Process(searched_host, i, this);
      Process* searched_process = p_processes.search_else_add(new_process);
      assert(searched_process == 0);
    }
  return true;
}

bool Parallel::remove_processes(const char* host_name, int number)
{
  if (number <= 0)
    return false;
  Host* searched_host = search_host(host_name);
  if (!searched_host)
    return false;
  int last_number = searched_host->number_of_processes();
  if (last_number == number)
    return remove_host(host_name);
  if (searched_host->remove_process(number) == false)
    return false;
  int curr_number = searched_host->number_of_processes();
  p_remove_processes(searched_host, curr_number, last_number);
  return true;
}

Process* Parallel::search_process(Host* host, int process_id)
{
  Process searching_process(host, process_id, 0);
  return p_processes.search(&searching_process);
}

ParallelErrorCode Parallel::add_search(Search* search)
{
  int len = p_searches.length();
  for (int i = 0; i < len; ++i)
    {
      if (p_searches[i]->compare(search) == 0)
        return PARALLEL_ERROR_CODE_ALREADY_ADD;
    }
  p_searches.add(search);
  if (search->setup() == false)
    return PARALLEL_ERROR_CODE_OUTPUT_FILE_OPEN_ERROR;
  return PARALLEL_ERROR_CODE_OK;
}

bool Parallel::remove_search(int index)
{
  return p_searches.remove(index);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
