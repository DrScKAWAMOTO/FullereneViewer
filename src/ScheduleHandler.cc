/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include "ScheduleHandler.h"

bool ScheduleHandler::p_receive()
{
  char work[1];
  int red_len;
  red_len = read(p_readfd, work, 1);
  if (red_len <= 0)
    return false;
  return true;
}

static ScheduleHandler* singleton = 0;

static void signal_handler(int signame)
{
  assert(singleton);
  char work[1];
  work[0] = '\n';
  write(singleton->get_writefd(), work, 1);
}

#define R (0)
#define W (1)

ScheduleHandler::ScheduleHandler()
  : ReadHandler(), p_writefd(-1)
{
  assert(singleton == 0);
  singleton = this;
  int fds[2];
  int result;
  result = pipe(fds);
  assert(result == 0);
  p_readfd = fds[R];
  p_writefd = fds[W];
  sig_t result2 = signal(SIGALRM, signal_handler);
  assert(result2 != SIG_ERR);
  struct itimerval itv;
  struct itimerval oitv;
  itv.it_interval.tv_sec = SCHEDULE_INTERVAL;
  itv.it_interval.tv_usec = 0;
  itv.it_value.tv_sec = SCHEDULE_INTERVAL;
  itv.it_value.tv_usec = 0;
  result = setitimer(ITIMER_REAL, &itv, &oitv);
  assert(result == 0);
}

ScheduleHandler::~ScheduleHandler()
{
}

ReadHandlerResult ScheduleHandler::call()
{
  p_receive();
  return READ_HANDLER_RESULT_SCHEDULE_FORCE;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
