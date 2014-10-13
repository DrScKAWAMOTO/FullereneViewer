/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/param.h>
#include "AcceptHandler.h"
#include "Parallel.h"

bool AcceptHandler::p_receive()
{
  char* buffer;
  int length;
  if ((p_accept_stage == ACCEPT_STAGE_ADD_HOST_NUMBER) ||
      (p_accept_stage == ACCEPT_STAGE_ADD_PROCESS_NUMBER) ||
      (p_accept_stage == ACCEPT_STAGE_REMOVE_PROCESS_NUMBER))
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

AcceptHandler::AcceptHandler(Parallel* manager, int pid)
  : ReadHandler(), p_manager(manager), p_accept_stage(ACCEPT_STAGE_RECV_COMMAND),
    p_search(0), p_formula_set(false)
{
  MyString pipe_name = manager->get_client_pipe_base();
  pipe_name.append_string("/ca-cmd.");
  pipe_name.append_int(pid);
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
  pipe_name.append_int(pid);
  p_writefd = open(pipe_name, O_WRONLY);
  p_stdoutput = fdopen(p_writefd, "a");
  static bool started = true;
  if (started)
    printf("server is started !! (pid = %d)\n", (int)getpid());
  started = false;
}

AcceptHandler::~AcceptHandler()
{
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
          return READ_HANDLER_RESULT_EXIT;
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
      }
    usage_recv_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " exit\n");
      fprintf(p_stdoutput, " list ...\n");
      fprintf(p_stdoutput, " add ...\n");
      fprintf(p_stdoutput, " remove ...\n");
      return READ_HANDLER_RESULT_TERMINATE;
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
        if ((strcmp(p_buffer, "search") == 0) || (strcmp(p_buffer, "searches") == 0))
          {
            p_accept_stage = ACCEPT_STAGE_LIST_SEARCHES;
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
      fprintf(p_stdoutput, " list searches\n");
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
        if (strcmp(p_buffer, "process") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_ADD_PROCESS;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "search") == 0)
          {
            p_search = new Search();
            p_formula_set = false;
            p_accept_stage = ACCEPT_STAGE_ADD_SEARCH;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_add_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " add host <hostname> [<number_of_processes>=1]\n");
      fprintf(p_stdoutput, " add process <hostname> <number_of_processes>\n");
      fprintf(p_stdoutput, " add search <options> <formula_segment>\n");
      return READ_HANDLER_RESULT_TERMINATE;
    case ACCEPT_STAGE_REMOVE_COMMAND:
      {
        if (result == false)
          goto usage_remove_command;
        if (strcmp(p_buffer, "host") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_HOST;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "process") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_PROCESS;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        if (strcmp(p_buffer, "search") == 0)
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_SEARCH;
            return READ_HANDLER_RESULT_CONTINUED;
          }
      }
    usage_remove_command:
      fprintf(p_stdoutput, "usage:\n");
      fprintf(p_stdoutput, " remove host <hostname>\n");
      fprintf(p_stdoutput, " remove process <hostname> <number_of_processes>\n");
      fprintf(p_stdoutput, " remove search <number>\n");
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
    case ACCEPT_STAGE_LIST_SEARCHES:
      {
        if (result == true)
          goto usage_list_command;
        p_manager->print_searches(p_stdoutput, false);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_LIST_RANGES:
      {
        if (result == true)
          {
            int num = atoi(p_buffer);
            if (p_manager->print_ranges(p_stdoutput, num, 0) == false)
              fprintf(p_stdoutput, "Illegal number.\n");
          }
        else
          p_manager->print_searches(p_stdoutput, true);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_ADD_HOST:
      {
        if (result == false)
          goto usage_add_command;
        p_accept_stage = ACCEPT_STAGE_ADD_HOST_NUMBER;
        return READ_HANDLER_RESULT_CONTINUED;
      }
    case ACCEPT_STAGE_ADD_PROCESS:
      {
        if (result == false)
          goto usage_add_command;
        if (p_manager->search_host(p_buffer))
          {
            p_accept_stage = ACCEPT_STAGE_ADD_PROCESS_NUMBER;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        else
          {
            fprintf(p_stdoutput, "There is not such host `%s'.\n", p_buffer);
            p_manager->print_hosts(p_stdoutput);
            return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
          }
      }
    case ACCEPT_STAGE_ADD_SEARCH:
      {
        if (result == false)
          {
            ParallelErrorCode error_code;
            error_code = p_manager->add_search(p_search);
            if (error_code == PARALLEL_ERROR_CODE_ALREADY_ADD)
              {
                delete p_search;
                fprintf(p_stdoutput, "Already added.\n");
                p_search = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            else if (error_code == PARALLEL_ERROR_CODE_OUTPUT_FILE_OPEN_ERROR)
              {
                delete p_search;
                fprintf(p_stdoutput, "output file can not open.\n");
                p_search = 0;
                return READ_HANDLER_RESULT_TERMINATE;
              }
            else
              {
                p_manager->print_searches(p_stdoutput, false);
                p_search = 0;
                return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
              }
          }
        if (strncmp(p_buffer, "--out=", 6) == 0)
          {
            if (p_buffer[6] == '/')
              p_search->add_output_filename(p_buffer + 6);
            else
              {
                MyString fullpath(p_manager->get_client_cwd());
                fullpath.append_char('/');
                fullpath.append_string(p_buffer + 6);
                p_search->add_output_filename(fullpath);
              }
          }
        else if (p_buffer[0] == '-')
          {
            p_search->add_option(p_buffer);
          }
        else if (p_formula_set == false)
          {
            p_formula_set = true;
            if (p_search->add_formula_segment(p_buffer) == false)
              {
                fprintf(p_stdoutput, "<formula_segment> must end with the letter `-'.\n");
                delete p_search;
                p_search = 0;
                goto usage_add_command;
              }
          }
        else
          {
            delete p_search;
            p_search = 0;
            goto usage_add_command;
          }
        return READ_HANDLER_RESULT_CONTINUED;
      }
    case ACCEPT_STAGE_REMOVE_HOST:
      {
        if (result == false)
          goto usage_remove_command;
        if (p_manager->remove_host(p_buffer) == false)
          fprintf(p_stdoutput, "There is no such host `%s'.\n", p_buffer);
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    case ACCEPT_STAGE_REMOVE_PROCESS:
      {
        if (result == false)
          goto usage_remove_command;
        if (p_manager->search_host(p_buffer))
          {
            p_accept_stage = ACCEPT_STAGE_REMOVE_PROCESS_NUMBER;
            return READ_HANDLER_RESULT_CONTINUED;
          }
        else
          {
            fprintf(p_stdoutput, "There is not such host `%s'.\n", p_buffer);
            p_manager->print_hosts(p_stdoutput);
            return READ_HANDLER_RESULT_TERMINATE;
          }
      }
    case ACCEPT_STAGE_REMOVE_SEARCH:
      {
        if (result == false)
          goto usage_remove_command;
        int num = atoi(p_buffer);
        p_manager->remove_search(num);
        p_manager->print_searches(p_stdoutput, false);
        return READ_HANDLER_RESULT_TERMINATE;
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
    case ACCEPT_STAGE_ADD_PROCESS_NUMBER:
      {
        if (result == false)
          goto usage_add_command;
        int num = atoi(p_buffer2);
        if (num > 0)
          p_manager->add_processes(p_buffer, num);
        else
          fprintf(p_stdoutput, "Positive number is required.\n");
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE;
      }
    case ACCEPT_STAGE_REMOVE_PROCESS_NUMBER:
      {
        if (result == false)
          goto usage_remove_command;
        int num = atoi(p_buffer2);
        if (num > 0)
          {
            if (p_manager->remove_processes(p_buffer, num) == false)
              fprintf(p_stdoutput, "There is no process of a sufficient number.\n");
          }
        else
          fprintf(p_stdoutput, "Positive number is required.\n");
        p_manager->print_hosts(p_stdoutput);
        return READ_HANDLER_RESULT_TERMINATE;
      }
    default:
      goto usage_recv_command;
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
