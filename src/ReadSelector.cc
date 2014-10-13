/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "ReadSelector.h"

ReadSelector::ReadSelector()
  : p_last_num(0), p_last_readfd(-1), p_last_nfds(0)
{
}

ReadSelector::~ReadSelector()
{
}

bool ReadSelector::join(ReadHandler* handler)
{
  if (p_handlers.search_else_add(handler))
    return false;
  p_last_num = 0;
  return true;
}

bool ReadSelector::defect(ReadHandler* handler)
{
  bool result = p_handlers.remove(handler);
  if (result)
    p_last_num = 0;
  return result;
}

void ReadSelector::defect_all()
{
  int len = p_handlers.length();
  for (int i = len - 1; i >= 0; --i)
    {
      ReadHandler* rhandler = p_handlers[i];
      p_handlers.remove(rhandler);
    }
}

ReadHandlerResult ReadSelector::select(int timeout)
{
  if (p_last_num == 0)
    {
      int nfds = 0;
      int len = p_handlers.length();
      for (int i = 0; i < len; ++i)
        {
          int readfd = p_handlers[i]->p_readfd;
          if (readfd >= nfds)
            nfds = readfd + 1;
        }
      p_last_nfds = nfds;
      FD_ZERO(p_last_readfds);
      for (int i = 0; i < len; ++i)
        {
          int readfd = p_handlers[i]->p_readfd;
          FD_SET(readfd, p_last_readfds);
        }
      struct timeval timeval;
      timeval.tv_sec = timeout;
      timeval.tv_usec = 0;
      while (1)
        {
          p_last_num = ::select(p_last_nfds, p_last_readfds, 0, 0,
                                (timeout == INT_MAX) ? 0 : &timeval);
          if (p_last_num == 0)
            return READ_HANDLER_RESULT_TIMEOUT;
          if (p_last_num > 0)
            break;
        }
    }
  for (int i = 0; i < p_last_nfds; ++i)
    {
      if (++p_last_readfd >= p_last_nfds)
        p_last_readfd = 0;
      if (FD_ISSET(p_last_readfd, p_last_readfds))
        {
          p_last_num--;
          ReadHandler searching_handler = ReadHandler();
          searching_handler.p_readfd = p_last_readfd;
          ReadHandler* searched_handler = p_handlers.search(&searching_handler);
          assert(searched_handler);
          ReadHandlerResult result = searched_handler->call();
          if (result == READ_HANDLER_RESULT_TERMINATE)
            {
              defect(searched_handler);
              result = READ_HANDLER_RESULT_CONTINUED;
            }
          else if (result == READ_HANDLER_RESULT_SCHEDULE_AND_TERMINATE)
            {
              defect(searched_handler);
              result = READ_HANDLER_RESULT_SCHEDULE;
            }
          return result;
        }
    }
  assert(0);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
