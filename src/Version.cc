/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/09/12 16:05:38 JST
 */

#include <stdio.h>
#include "Version.h"
#include "Config.h"

void print_version(const char* program_name, FILE* output)
{
  const char* comment = "";
  if (output)
    comment = "### ";
  else
    output = stderr;

  fprintf(output, "%sCopyright (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)"
          " All Rights Reserved.\n", comment);
  fprintf(output, "%s%s Ver 1.3.2", comment, program_name);
#if defined(CONFIG_REFLECTED_IMAGE_IS_REGARDED_AS_ISOMORPHIC)
  fprintf(output, " (refleiso version)\n");
  fprintf(output, "%s   configured that mirror image is regarded as isomorphic.\n",
          comment);
#else
  fprintf(output, " (mirror version)\n");
  fprintf(output, "%s   configured that mirror image is also list up.\n", comment);
#endif
  fprintf(output, "%s\n", comment);
  if (output == stderr)
    {
      fprintf(output, "%sThanks to all contributors:\n", comment);
      fprintf(output, "%s    KAWAMOTO,Takuto       Kunihiro AIHARA\n", comment);
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
