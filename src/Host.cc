/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <string.h>
#include "Host.h"
#include "DebugMemory.h"

Host::Host(const char* host_name, int number_of_processes)
{
  p_host_name = host_name;
  p_number_of_processes = number_of_processes;
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
          (p_number_of_processes > 1) ? "%s (%d processes)\n" : "%s (%d process)\n",
          (char*)p_host_name, p_number_of_processes);
}

void Host::add_process(int num)
{
  p_number_of_processes += num;
}

bool Host::remove_process(int num)
{
  if (p_number_of_processes <= num)
    return false;
  p_number_of_processes -= num;
  return true;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
