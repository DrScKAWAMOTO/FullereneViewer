/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 1996/02/24 11:59:35 JST
 */

#ifndef __DEBUGMEMORY_H__
#define __DEBUGMEMORY_H__

// #define DEBUG_MEMORY

#ifdef DEBUG_MEMORY
#include <sys/types.h>
#include <new>

void* operator new(std::size_t, const char*, int) throw(std::bad_alloc);
void* operator new[](std::size_t, const char *source_file_name, int source_line_number) throw(std::bad_alloc);

#define new new(__FILE__,__LINE__)

#endif

#endif /* __DEBUGMEMORY_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
