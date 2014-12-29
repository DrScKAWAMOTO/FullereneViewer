/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <string.h>
#include "Host.h"
#include "DebugMemory.h"
#include "AcceptHandler.h"

void Host::p_search_and_call_enable_waiters(int num)
{
  /* enabled の数が num かどうかチェックする */
  if (num != p_number_of_enabled_processes)
    return;
  /* メッセージ作成 */
  MyString message("enable ");
  message.append_int(num);
  message.append_string(" done\n");
  /* num 分の enable_waiters を呼び出し */
  int len = p_enable_waiters.length();
  for (int i = 0; i < len; ++i)
    {
      ObjectInt2* waiter = p_enable_waiters[i];
      if (waiter->get_key() == num)
        AcceptHandler::call_callback(waiter->get_value(), message);
    }
}

Host::Host(const char* host_name, int number_of_processes)
{
  p_host_name = host_name;
  p_number_of_processes = number_of_processes;
  p_number_of_enabled_processes = number_of_processes;
}

Host::~Host()
{
}

int Host::compare(const Host* you) const
{
  return strcmp((char*)p_host_name, (char*)you->p_host_name);
}

void Host::print(FILE* output) const
{
  fprintf(output,
          ((p_number_of_processes > 1) ? "%s (%d processes %d enabled)\n" :
           "%s (%d process %d enabled)\n"),
           (char*)p_host_name, p_number_of_processes, p_number_of_enabled_processes);
}

bool Host::enable_processes(int num)
{
  if (num < 0)
    return false;
  if (p_number_of_processes < num)
    return false;
  p_number_of_enabled_processes = num;
  p_search_and_call_enable_waiters(num);
  return true;
}

void Host::enter_enabled_waiter(int num, int pid)
{
  ObjectInt2* entry = new ObjectInt2(num, pid);
  p_enable_waiters.add(entry);
  p_search_and_call_enable_waiters(p_number_of_enabled_processes);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
