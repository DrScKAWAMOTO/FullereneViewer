/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/param.h>
#include "AcceptHandler.h"
#include "Parallel.h"

static Parallel* s_manager;

void AcceptHandler::call_callback(int pid, const MyString& message)
{
  assert(s_manager);
  MyString pipe_name = s_manager->get_client_pipe_base();
  pipe_name.append_string("/ca-cal.");
  pipe_name.append_int(pid);
  int writefd = open(pipe_name, O_WRONLY | O_NONBLOCK);
  if (writefd >= 0)
    {
      MyString terminate("ca-server: callback\n");
      write(writefd, (char*)message, message.length());
      write(writefd, (char*)terminate, terminate.length());
      close(writefd);
    }
}

bool AcceptHandler::p_receive()
{
  char* buffer;
  int length;
  if ((p_accept_stage == ACCEPT_STAGE_ADD_HOST_NUMBER) ||
      (p_accept_stage == ACCEPT_STAGE_ENABLE_PROCESS_NUMBER) ||
      (p_accept_stage == ACCEPT_STAGE_WAIT_HOST_ENABLED_NUMBER))
    {
      buffer = p_buffer2;
      length = BUFFER2_SIZE;
    }
  else
    {
      buffer = p_buffer;
      length = BUFFER_SIZE;
    }
  while (1)
    {
      if (fgets(buffer, length, p_stdinput) == 0)
        return false;
      int len = strlen(buffer);
      if (len > 0)
        {
          if (buffer[len - 1] == '\n')
            buffer[--len] = '\0';
          if (strcmp(buffer, "terminate") == 0)
            return false;
          return true;
        }
    }
}

void AcceptHandler::p_get_output_filename(MyString& output_filename)
{
  if (p_buffer[6] == '/')
    output_filename = p_buffer + 6;
  else
    {
      output_filename = p_manager->get_client_cwd();
      output_filename.append_char('/');
      output_filename.append_string(p_buffer + 6);
    }
}

AcceptHandler::AcceptHandler(Parallel* manager, int pid)
  : ReadHandler(), p_manager(manager), p_pid(pid),
    p_accept_stage(ACCEPT_STAGE_RECV_COMMAND),
    p_ca_parallel_is_waiting(false), p_collector(0)
{
  s_manager = p_manager;
  MyString pipe_name = p_manager->get_client_pipe_base();
  pipe_name.append_string("/ca-cmd.");
  pipe_name.append_int(p_pid);
  p_readfd = open(pipe_name, O_RDONLY | O_NONBLOCK);
  assert(p_readfd > -1);
  int result = fcntl(p_readfd, F_GETFL);
  assert(result > -1);
  result &= ~O_NONBLOCK;
  result = fcntl(p_readfd, F_SETFL, result);
  assert(result > -1);
  p_stdinput = fdopen(p_readfd, "r");
  pipe_name = manager->get_client_pipe_base();
  pipe_name.append_string("/ca-rst.");
  pipe_name.append_int(p_pid);
  p_writefd = open(pipe_name, O_WRONLY);
  p_stdoutput = fdopen(p_writefd, "a");
  static bool started = true;
  if (started)
    printf("server is started !! (pid = %d)\n", (int)getpid());
  started = false;
}

AcceptHandler::~AcceptHandler()
{
  if (p_ca_parallel_is_waiting)
    fprintf(p_stdoutput, "ca-server: callback\n");
  else
    fprintf(p_stdoutput, "ca-server: terminate\n");
  fclose(p_stdinput);
  fclose(p_stdoutput);
}

ReadHandlerResult AcceptHandler::call()
{
  bool result = p_receive();
  switch (p_accept_stage)
    {
    case ACCEPT_STAGE_RECV_COMMAND:
      {
        if (result == false)
          goto usage_recv_command;
        if (strcmp(p_buffer, "exit") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_EXIT_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "list") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_LIST_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "add") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_ADD_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "remove") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "enable") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_ENABLE_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "wait") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_WAIT_COMMAND;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_recv_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " exit\n");
      fprintf(p_stdoutput, " list ...\n");
      fprintf(p_stdoutput, " add ...\n");
      fprintf(p_stdoutput, " remove ...\n");
      fprintf(p_stdoutput, " enable ...\n");
      fprintf(p_stdoutput, " wait ...\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_EXIT_COMMAND:
      {
        if (result == true)
          goto usage_list_command;
        return READ_HANDLER_RESULT_EXIT;
      }
    case ACCEPT_STAGE_LIST_COMMAND:
      {
        if (result == false)
          goto usage_list_command;
        if ((strcmp(p_buffer, "host") == 0) || (strcmp(p_buffer, "hosts") == 0))
          {
            p_accept_stage = ACCEPT_STAGE_LIST_HOSTS;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if ((strcmp(p_buffer, "process") == 0) || (strcmp(p_buffer, "processes") == 0))
          {
            p_accept_stage = ACCEPT_STAGE_LIST_PROCESSES;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if ((strcmp(p_buffer, "collector") == 0) || (strcmp(p_buffer, "collectors") == 0))
          {
            p_accept_stage = ACCEPT_STAGE_LIST_COLLECTORS;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if ((strcmp(p_buffer, "range") == 0) || (strcmp(p_buffer, "ranges") == 0))
          {
            p_accept_stage = ACCEPT_STAGE_LIST_RANGES;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_list_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " list hosts\n");
      fprintf(p_stdoutput, " list processes [index]\n");
      fprintf(p_stdoutput, " list collectors\n");
      fprintf(p_stdoutput, " list ranges\n");
      fprintf(p_stdoutput, " list ranges --out=<file>\n");
      fprintf(p_stdoutput, " list ranges [index]\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_ADD_COMMAND:
      {
        if (result == false)
          goto usage_add_command;
        if (strcmp(p_buffer, "host") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_ADD_HOST;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "collector") == 0)
          {
            p_collector = new Collector();
            p_accept_stage = ACCEPT_STAGE_ADD_COLLECTOR;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_add_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " add host <hostname> [<number_of_processes>=1]\n");
      fprintf(p_stdoutput, " add collector <options> <formula_segments>\n");
      fprintf(p_stdoutput, "       <formula_segment> is `Sn-', `A1-', or `Tn,m,h-'\n");
      fprintf(p_stdoutput, "       <options>:\n");
      fprintf(p_stdoutput, "           --symmetric=<num>\n");
      fprintf(p_stdoutput, "           --ordinary=<num>\n");
      fprintf(p_stdoutput, "           --tube=<num>\n");
      fprintf(p_stdoutput, "           --close=<num>\n");
      fprintf(p_stdoutput, "           --step-copy-branch\n");
      fprintf(p_stdoutput, "           --step-forward\n");
      fprintf(p_stdoutput, "           --step-backward\n");
      fprintf(p_stdoutput, "           --out=<file>\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_ENABLE_COMMAND:
      {
        if (result == false)
          goto usage_enable_command;
        if (strcmp(p_buffer, "process") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_ENABLE_PROCESS;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_enable_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " enable process <hostname> <number_of_processes>\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_REMOVE_COMMAND:
      {
        if (result == false)
          goto usage_remove_command;
        if (strcmp(p_buffer, "collector") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_COLLECTOR;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_remove_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " remove collector --out=<file>\n");
      fprintf(p_stdoutput, " remove collector [index]\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_WAIT_COMMAND:
      {
        if (result == false)
          goto usage_wait_command;
        if (strcmp(p_buffer, "host") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_WAIT_HOST;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "collector") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_WAIT_COLLECTOR_DONE;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_wait_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " wait host enabled <hostname> <num>\n");
      fprintf(p_stdoutput, " wait collector done --out=<file>\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_LIST_HOSTS:
      {
        if (result == true)
          goto usage_list_command;
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_LIST_PROCESSES:
      {
        if (result == true)
          {
            int num = atoi(p_buffer);
            if (p_manager->print_processes(p_stdoutput, num) == false)
              fprintf(p_stdoutput, "Illegal number.\n");
          }
        else
          p_manager->print_processes(p_stdoutput);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_LIST_COLLECTORS:
      {
        if (result == true)
          goto usage_list_command;
        p_manager->print_collectors(p_stdoutput, false);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_LIST_RANGES:
      {
        if (result == true)
          {
            Collector* searched_collector = 0;
            if (strncmp(p_buffer, "--out=", 6) == 0)
              {
                MyString output_filename;
                p_get_output_filename(output_filename);
                int index = p_manager->search_collector(output_filename);
                if (index >= 0)
                  {
                    searched_collector = p_manager->get_collectors()[index];
                    fprintf(p_stdoutput, "collector[%d] = ", index);
                  }
                else
                  fprintf(p_stdoutput, "Unknown file `%s'.\n", (char*)output_filename);
              }
            else
              {
                int index = atoi(p_buffer);
                searched_collector = p_manager->get_collectors()[index];
                if (searched_collector)
                  fprintf(p_stdoutput, "collector[%d] = ", index);
                else
                  fprintf(p_stdoutput, "Illegal number %d.\n", index);
              }
            if (searched_collector)
              {
                searched_collector->print(p_stdoutput);
                p_manager->print_ranges(p_stdoutput, searched_collector);
              }
          }
        else
          p_manager->print_collectors(p_stdoutput, true);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_ADD_HOST:
      {
        if (result == false)
          goto usage_add_command;
        p_accept_stage = ACCEPT_STAGE_ADD_HOST_NUMBER;
        return READ_HANDLER_RESULT_CONTINUED;
      }
    case ACCEPT_STAGE_ADD_COLLECTOR:
      {
        if (result == false)
          {
            if (p_collector->get_formula_segments().length() == 0)
              {
                delete p_collector;
                fprintf(p_stdoutput, "Formula segments are not specified.\n");
                p_collector = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            if (p_collector->setup_formula_segments() == false)
              {
                delete p_collector;
                fprintf(p_stdoutput, "Illegal <formula segment> is specified.\n");
                p_collector = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            const char* output_filename = p_collector->get_output_filename();
            if (output_filename[0] == '\0')
              {
                delete p_collector;
                fprintf(p_stdoutput, "Output filename is not specified.\n");
                p_collector = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            if (p_collector->setup_output_file() == false)
              {
                delete p_collector;
                fprintf(p_stdoutput, "Output file can not open.\n");
                p_collector = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            if (p_manager->add_collector(p_collector) == false)
              {
                delete p_collector;
                fprintf(p_stdoutput, "Already added.\n");
                p_collector = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            p_manager->print_collectors(p_stdoutput, false);
            p_collector = 0;
            return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
          }
        if (strncmp(p_buffer, "--out=", 6) == 0)
          {
            MyString output_filename;
            p_get_output_filename(output_filename);
            p_collector->add_output_filename((char*)output_filename);
          }
        else if (p_buffer[0] == '-')
          {
            p_collector->add_option(p_buffer);
          }
        else
          {
            if (p_collector->add_formula_segment(p_buffer) == false)
              {
                fprintf(p_stdoutput, "Illegal <formula_segment>.\n");
                delete p_collector;
                p_collector = 0;
                goto usage_add_command;
              }
          }
        return READ_HANDLER_RESULT_CONTINUED;
      }
    case ACCEPT_STAGE_ADD_HOST_NUMBER:
      {
        int num;
        if (result)
          num = atoi(p_buffer2);
        else
          num = 1;
        if (num > 0)
          {
            if (p_manager->add_host(p_buffer, num) == false)
              fprintf(p_stdoutput, "host `%s' is already added.\n", p_buffer);
          }
        else
          fprintf(p_stdoutput, "Positive number is required.\n");
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
      }
    case ACCEPT_STAGE_REMOVE_COLLECTOR:
      {
        if (result == false)
          goto usage_remove_command;
        if (strncmp(p_buffer, "--out=", 6) == 0)
          {
            MyString output_filename;
            p_get_output_filename(output_filename);
            p_manager->remove_collector((char*)output_filename);
          }
        else
          {
            int num = atoi(p_buffer);
            p_manager->remove_collector(num);
          }
        p_manager->print_collectors(p_stdoutput, false);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_ENABLE_PROCESS:
      {
        if (result == false)
          goto usage_add_command;
        if (p_manager->search_host(p_buffer))
          {
            p_accept_stage = ACCEPT_STAGE_ENABLE_PROCESS_NUMBER;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        else
          {
            fprintf(p_stdoutput, "There is not such host `%s'.\n", p_buffer);
            p_manager->print_hosts(p_stdoutput);
            return READ_HANDLER_RESULT_TERMINATE;
          }
      }
    case ACCEPT_STAGE_ENABLE_PROCESS_NUMBER:
      {
        if (result == false)
          goto usage_enable_command;
        int num = atoi(p_buffer2);
        if (num >= 0)
          {
            if (p_manager->enable_processes(p_buffer, num) == false)
              fprintf(p_stdoutput, "Illegal number is specified.\n");
          }
        else
          fprintf(p_stdoutput, "Non negative number is required.\n");
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
      }
    case ACCEPT_STAGE_WAIT_HOST:
      {
        if (result == false)
          goto usage_wait_command;
        if (strcmp(p_buffer, "enabled") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_WAIT_HOST_ENABLED;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        goto usage_wait_command;
      }
    case ACCEPT_STAGE_WAIT_HOST_ENABLED:
      {
        if (result == false)
          goto usage_wait_command;
        p_accept_stage = ACCEPT_STAGE_WAIT_HOST_ENABLED_NUMBER;
        return READ_HANDLER_RESULT_CONTINUED;
      }
    case ACCEPT_STAGE_WAIT_HOST_ENABLED_NUMBER:
      {
        if (result == false)
          goto usage_wait_command;
        Host* searched_host = p_manager->search_host(p_buffer);
        if (searched_host)
          {
            int num;
            num = atoi(p_buffer2);
            searched_host->enter_enabled_waiter(num, p_pid);
            p_ca_parallel_is_waiting = true;
            return READ_HANDLER_RESULT_TERMINATE;
          }
        else
          {
            fprintf(p_stdoutput, "There is not such host `%s'.\n", p_buffer);
            p_manager->print_hosts(p_stdoutput);
            return READ_HANDLER_RESULT_TERMINATE;
          }
        goto usage_wait_command;
      }
    case ACCEPT_STAGE_WAIT_COLLECTOR_DONE:
      {
        if (result == false)
          goto usage_wait_command;
        if (strcmp(p_buffer, "done") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_WAIT_COLLECTOR_DONE_OUT;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        goto usage_wait_command;
      }
    case ACCEPT_STAGE_WAIT_COLLECTOR_DONE_OUT:
      {
        if (result == false)
          goto usage_wait_command;
        if (strncmp(p_buffer, "--out=", 6) == 0)
          {
            MyString output_filename;
            p_get_output_filename(output_filename);
            int index = p_manager->search_collector((char*)output_filename);
            if (index == -1)
              {
                fprintf(p_stdoutput, "There is not such collector `%s'.\n",
                        (char*)output_filename);
                p_manager->print_collectors(p_stdoutput, false);
                return READ_HANDLER_RESULT_TERMINATE;
              }
            Collector* searched_collector = p_manager->get_collectors()[index];
            searched_collector->enter_done_waiter(p_pid);
            p_ca_parallel_is_waiting = true;
            return READ_HANDLER_RESULT_TERMINATE;
          }
        goto usage_wait_command;
      }
    default:
      goto usage_recv_command;
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
