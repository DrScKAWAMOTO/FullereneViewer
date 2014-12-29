/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include "DebugMemory.h"

#ifdef DEBUG_MEMORY

#include <stdio.h>
#include <stdlib.h>

class DebugMemory {
public:
  static unsigned int sequence_no;
  static unsigned int check_point_no;
  static bool now_memory_tracing;
  DebugMemory();
  ~DebugMemory();
  static void* new_alloc(size_t size, const char* source_file_name,
                         int source_line_number, bool is_array) throw(std::bad_alloc);
  static void* new_alloc(size_t size, bool is_array) throw(std::bad_alloc);
  static void delete_free(void* ptr, bool is_array) throw();
};

#undef new

void*
operator new(std::size_t size, const char* source_file_name, int source_line_number)
  throw(std::bad_alloc)
{
  return DebugMemory::new_alloc(size, source_file_name, source_line_number, false);
}

void*
operator new[](std::size_t size, const char* source_file_name, int source_line_number)
  throw(std::bad_alloc)
{
  return DebugMemory::new_alloc(size, source_file_name, source_line_number, true);
}

void* operator new(std::size_t size) throw(std::bad_alloc)
{
  return DebugMemory::new_alloc(size, false);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
  return DebugMemory::new_alloc(size, true);
}

void operator delete(void* ptr) throw()
{
  DebugMemory::delete_free(ptr, false);
}

void operator delete[](void* ptr) throw()
{
  DebugMemory::delete_free(ptr, true);
}

typedef union _alignment Alignment;

union _alignment {
  double x[2];
  char pad[sizeof(double) * 2];
};

class MemoryCell {
public:
  MemoryCell* next;
  MemoryCell* prev;
  unsigned int sequence_no;
  unsigned int check_point_no;
  const char* source_file_name;
  int source_line_number;
  bool is_array;
  size_t size;
  Alignment head_magic;
  Alignment body;
  static MemoryCell allocated;
  static MemoryCell* search(void* ptr);
  static void append(MemoryCell* ptr);
  static void remove(MemoryCell* ptr);
  void print_memory_alloc() const;
  void print_memory_free() const;
  void print_memory_leak() const;
  void print_is_array_mismatch() const;
  MemoryCell();
};

MemoryCell MemoryCell::allocated;
unsigned int DebugMemory::sequence_no = 0;
unsigned int DebugMemory::check_point_no = 0;
DebugMemory __attribute__((init_priority(101))) memory_debug;
bool DebugMemory::now_memory_tracing = false;

MemoryCell::MemoryCell()
: next(this), prev(this)
{
}

MemoryCell* MemoryCell::search(void* ptr)
{
  for (MemoryCell* work = allocated.next; work != &allocated;
       work = work->next)
    if (&work->body == ptr)
      return work;
  return 0;
}

void MemoryCell::append(MemoryCell* ptr)
{
  ptr->next = &allocated;
  ptr->prev = allocated.prev;
  if (allocated.prev)
    {
      allocated.prev->next = ptr;
      allocated.prev = ptr;
    }
}

void MemoryCell::remove(MemoryCell* ptr)
{
  ptr->prev->next = ptr->next;
  ptr->next->prev = ptr->prev;
}

void MemoryCell::print_memory_alloc() const
{
  printf("???:???: unmanaged memory alloc allocate_no=%d,address=%08X,size=%d\n",
         sequence_no, (int)&body, size);
}

void MemoryCell::print_memory_free() const
{
  printf("???:???: unmanaged memory free allocate_no=%d,address=%08X,size=%d\n",
         sequence_no, (int)&body, size);
}

void MemoryCell::print_memory_leak() const
{
  if (source_file_name)
    printf("%s:%d: memory leak allocate_no=%d,address=%08X,size=%d\n",
           source_file_name, source_line_number,
           sequence_no, (int)&body, size);
  else
    printf("???:???: unmanaged memory leak allocate_no=%d,address=%08X,size=%d\n",
           sequence_no, (int)&body, size);
}

void MemoryCell::print_is_array_mismatch() const
{
  if (source_file_name)
    printf("%s:%d: array mismatch allocate_no=%d,address=%08X,size=%d\n",
           source_file_name, source_line_number,
           sequence_no, (int)&body, size);
  else
    printf("???:???: unmanaged array mismatch allocate_no=%d,address=%08X,size=%d\n",
           sequence_no, (int)&body, size);
}

void* DebugMemory::new_alloc(size_t size, const char* source_file_name,
                             int source_line_number, bool is_array) throw(std::bad_alloc)
{
  MemoryCell* ptr = (MemoryCell*)malloc(sizeof(MemoryCell) + size);
  if (!ptr)
    throw std::bad_alloc();
  MemoryCell::append(ptr);
  ptr->sequence_no = sequence_no++;
  ptr->check_point_no = check_point_no;
  ptr->source_file_name = source_file_name;
  ptr->source_line_number = source_line_number;
  ptr->is_array = is_array;
  ptr->size = size;
  for (unsigned int i = 0; i < 2 * sizeof(Alignment) + size; ++i)
    ptr->head_magic.pad[i] = 'E';
  void* result = &ptr->body;
  return result;
}

void* DebugMemory::new_alloc(size_t size, bool is_array) throw(std::bad_alloc)
{
  MemoryCell* ptr = (MemoryCell*)malloc(sizeof(MemoryCell) + size);
  if (!ptr)
    throw std::bad_alloc();
  MemoryCell::append(ptr);
  ptr->sequence_no = sequence_no++;
  ptr->check_point_no = check_point_no;
  ptr->source_file_name = 0;
  ptr->source_line_number = -1;
  ptr->is_array = is_array;
  ptr->size = size;
  for (unsigned int i = 0; i < 2 * sizeof(Alignment) + size; ++i)
    ptr->head_magic.pad[i] = 'E';
  void* result = &ptr->body;
  ptr->print_memory_alloc();
  return result;
}

void DebugMemory::delete_free(void* ptr, bool is_array) throw()
{
  if (!ptr)
    return;
  MemoryCell* work = MemoryCell::search(ptr);
  if (work)
    {
      if (!work->source_file_name)
        work->print_memory_free();
      if (work->is_array != is_array)
        work->print_is_array_mismatch();
      MemoryCell::remove(work);
      free(work);
    }
  else if (now_memory_tracing)
    printf("???:???: delete (%08X) illegal memory area\n", (int)ptr);
}

DebugMemory::DebugMemory()
{
  if (now_memory_tracing)
    {
      printf("double definition of Memory object\n");
      exit(-1);
    }
  now_memory_tracing = true;
  sequence_no = 0;
  check_point_no = 0;
  MemoryCell::allocated.next = &MemoryCell::allocated;
  MemoryCell::allocated.prev = &MemoryCell::allocated;
}

DebugMemory::~DebugMemory()
{
  MemoryCell* ptr = MemoryCell::allocated.next;
  if (now_memory_tracing)
    {
      printf("Memory Leaks ...\n");
      while (ptr != &MemoryCell::allocated)
        {
          MemoryCell* work = ptr->next;
          ptr->print_memory_leak();
          free(ptr);
          ptr = work;
        }
    }
  now_memory_tracing = false;
}

#endif

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
