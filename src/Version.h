/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/06/07 09:10:25 JST
 */

#ifndef __VERSION_H__
#define __VERSION_H__

#include <stdio.h>

static inline void print_version(const char* program_name_and_version)
{
  fprintf(stderr, "%s\n", program_name_and_version);
  fprintf(stderr, "Copyright (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext) All Rights Reserved.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Thanks to all contributors:\n");
  fprintf(stderr, "    KAWAMOTO,Takuto       Kunihiro AIHARA\n");
}

#endif /* __VERSION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
