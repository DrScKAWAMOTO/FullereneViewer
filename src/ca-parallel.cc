/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyString.h"
#include "Version.h"

static void usage(const char* arg0)
{
  fprintf(stderr, "usage: %s [options]\n", arg0);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "    -v (--version) ....... show version,\n");
  fprintf(stderr, "    -h (--help) .......... show this message.\n");
  exit(0);
}

static void fork_server(const MyString& home)
{
  MyString lockfile_name = home;
  int len = home.length();
  if (home[len - 1] != '/')
    lockfile_name.append_char('/');
  lockfile_name.append_string("tmp/ca-lockfile");
  bool server_is_alive = true;
  for (int count = 0; count < 5; ++count)
    {
      int fd = open((char*)lockfile_name,
                    O_EXLOCK | O_RDONLY | O_CREAT | O_NONBLOCK, 0660);
      if (fd >= 0)
        {
          close(fd);
          server_is_alive = false;
          break;
        }
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = 1;
      nanosleep(&ts, 0);
    }
  if (server_is_alive)
    return;
  for (int count = 0; count < 3; ++count)
    {
      pid_t pid;
      pid = fork();
      if (pid  == 0)
        { /* child process == ca-server */
          execlp("ca-server", "ca-server", (char *)0);
          perror("execlp");
          exit(1);
        }
      else if (pid != -1)
        return;
    }
  fprintf(stderr, "fork failed three times.\n");
  exit(1);
}

static void generate_pipe(const char* pipe_name)
{
  struct stat sb;
  if (stat(pipe_name, &sb) == 0)
    {
      if (S_ISFIFO(sb.st_mode))
        return;
      unlink(pipe_name);
    }
  if (mkfifo(pipe_name, 0770) != 0)
    {
      perror("mkfifo");
      exit(1);
    }
}

static MyString cmd_pipe_name;
static MyString rst_pipe_name;

static int start_communication(const MyString& home)
{
  int len = home.length();
  pid_t pid = getpid();
  cmd_pipe_name = home;
  if (home[len - 1] != '/')
    cmd_pipe_name.append_char('/');
  cmd_pipe_name.append_string("tmp/ca-cmd.");
  cmd_pipe_name.append_int((int)pid);
  generate_pipe(cmd_pipe_name);
  rst_pipe_name = home;
  if (home[len - 1] != '/')
    rst_pipe_name.append_char('/');
  rst_pipe_name.append_string("tmp/ca-rst.");
  rst_pipe_name.append_int((int)pid);
  generate_pipe(rst_pipe_name);
  char pwd[MAXPATHLEN + 1];
  getwd(pwd);
  MyString buffer;
  buffer.append_int((int)pid);
  buffer.append_char(' ');
  buffer.append_string(pwd);
  buffer.append_char('\n');
  MyString server_pipe = home;
  if (home[len - 1] != '/')
    server_pipe.append_char('/');
  server_pipe.append_string("tmp/ca-server");
  len = buffer.length();
  for (int count = 0; count < 5; ++count)
    {
      int fd = open((char*)server_pipe, O_WRONLY);
      if (fd < 0)
        {
          struct timespec ts;
          ts.tv_sec = 0;
          ts.tv_nsec = 10000000;
          nanosleep(&ts, 0);
          continue;
        }
      int rstfd = open((char*)rst_pipe_name, O_RDONLY | O_NONBLOCK);
      assert(rstfd > -1);
      int result = fcntl(rstfd, F_GETFL);
      assert(result > -1);
      result &= ~O_NONBLOCK;
      result = fcntl(rstfd, F_SETFL, result);
      assert(result > -1);
      int wrote_len = write(fd, (char*)buffer, len);
      assert(wrote_len == len);
      close(fd);
      return rstfd;
    }
  fprintf(stderr, "open `%s' failed five times.\n", (char*)server_pipe);
  exit(1);
}

static void send_command(int argc, char *argv[])
{
  int fd = open(cmd_pipe_name, O_WRONLY);
  FILE* fptr = fdopen(fd, "a");
  while (argc > 0)
    {
      fprintf(fptr, "%s\n", argv[0]);
      argc--;
      argv++;
    }
  fprintf(fptr, "terminate\n");
  fclose(fptr);
}

static void recv_result(int fd)
{
  FILE* fptr = fdopen(fd, "r");
  while (1)
    {
      char buffer[1024];
      if (fgets(buffer, 1024, fptr) == 0)
        break;
      if (strcmp(buffer, "ca-server: terminate\n") == 0)
        break;
      printf("%s", buffer);
    }
  fclose(fptr);
}

static void term_communication()
{
  unlink(cmd_pipe_name);
  unlink(rst_pipe_name);
}

int main(int argc, char *argv[])
{
  const char* arg0 = argv[0];
  argc--;
  argv++;
  while (argc > 0)
    {
      if ((strcmp(argv[0], "-h") == 0) || (strcmp(argv[0], "--help") == 0))
        {
          usage(arg0);
        }
      else if ((strcmp(argv[0], "-v") == 0) || (strcmp(argv[0], "--version") == 0))
        {
          print_version("ca-parallel");
          exit(0);
        }
      else
        break;
    }
  const char* home_env = getenv("HOME");
  if (home_env == 0)
    {
      fprintf(stderr, "set environment HOME\n");
      exit(1);
    }
  MyString home = home_env;
  fork_server(home);
  int rstfd = start_communication(home);
  send_command(argc, argv);
  recv_result(rstfd);
  term_communication();
  return 0;
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
