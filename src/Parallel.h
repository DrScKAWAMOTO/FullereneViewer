/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __PARALLEL_H__
#define __PARALLEL_H__

#include "Set.h"
#include "List.h"
#include "Host.h"
#include "Process.h"
#include "Collector.h"
#include "MyString.h"

class ReadSelector;
class ReadHandler;

class Parallel {
  // friend classes & functions

  // members
private:
  MyString p_client_cwd;
  MyString p_server_pipe_name;
  MyString p_server_lockfile_name;
  MyString p_server_logfile_name;
  MyString p_client_pipe_base;
  Set<Host> p_hosts;
  Set<Process> p_processes;
  Set<Process> p_unassigned_processes;
  List<Collector> p_collectors;
  ReadSelector* p_selector;

  // private tools
private:
  AssignProcessResult p_assign_process(Process* process, bool force = true);
  void p_enable_processes(Host* searched_host, int from, int to);
  void p_disable_processes(Host* searched_host, int from, int to);

  // constructors & the destructor
public:
  Parallel(const MyString& home);
  ~Parallel();

  // scheduling
public:
  bool assign_processes(bool force = true);
  void manage_unassigned_process(Process* process);

  // I/O
public:
  bool join(ReadHandler* rhandler);
  void command_loop();
  void print_hosts(FILE* output) const;
  bool print_ranges(FILE* output, int index) const;
  bool print_ranges(FILE* output, Collector* collector) const;
  bool print_processes(FILE* output, int host_index);
  void print_processes(FILE* output);
  void print_collectors(FILE* output, bool ranges_too) const;

  // member accessing methods
public:
  void set_client_cwd(const char* client_cwd);
  const char* get_client_cwd() const { return p_client_cwd; }
  bool add_host(const char* host_name, int process_num);
  bool remove_host(const char* host_name);
  Host* search_host(const char* host_name);
  const Set<Host>& get_hosts() const { return p_hosts; }
  bool enable_processes(const char* host_name, int number);
  void disable_all_processes();
  Process* search_process(Host* host, int process_id);
  const Set<Process>& get_processes() const { return p_processes; }
  bool add_collector(Collector* collector);
  int search_collector(const char* output_filename);
  bool remove_collector(int index);
  bool remove_collector(const char* output_filename);
  const List<Collector>& get_collectors() const { return p_collectors; }
  const char* get_server_pipe_name() const { return p_server_pipe_name; }
  const char* get_client_pipe_base() const { return p_client_pipe_base; }

};

#endif /* __PARALLEL_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
