/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <sys/param.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include "ListenHandler.h"
#include "AcceptHandler.h"
#include "Parallel.h"

ListenHandler::ListenHandler(Parallel* manager)
  : ReadHandler(), p_manager(manager)
{
  p_readfd = open(p_manager->get_server_pipe_name(), O_RDONLY | O_NONBLOCK);
  assert(p_readfd > -1);
  int result = fcntl(p_readfd, F_GETFL);
  assert(result > -1);
  result &= ~O_NONBLOCK;
  result = fcntl(p_readfd, F_SETFL, result);
  assert(result > -1);
}

ListenHandler::~ListenHandler()
{
}

ReadHandlerResult ListenHandler::call()
{
  FILE* fptr = fdopen(p_readfd, "r");
  char buffer[MAXPATHLEN + 20];
  buffer[0] = '\0';
  char* fgets_result;
  fgets_result = fgets(buffer, 100, fptr);
  bool result;
  if (fgets_result)
    {
      int pid = atoi(buffer);
      char* ptr = buffer;
      while (*ptr && (*ptr != ' '))
        ++ptr;
      if (*ptr == ' ')
        ++ptr;
      int len = strlen(ptr);
      if ((len > 0) && (ptr[len - 1] == '\n'))
        ptr[len - 1] = '\0';
      p_manager->set_client_cwd(ptr);

      AcceptHandler* ahandler = new AcceptHandler(p_manager, pid);
      result = p_manager->join(ahandler);
      assert(result == true);
    }
  ListenHandler* lhandler = new ListenHandler(p_manager);
  result = p_manager->join(lhandler);
  assert(result == true);
  fclose(fptr);
  return READ_HANDLER_RESULT_TERMINATE;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
