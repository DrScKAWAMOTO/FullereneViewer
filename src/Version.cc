/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
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
  fprintf(output, "%s%s Ver " FULLERENE_VIEWER_VERSION "\n", comment, program_name);
  fprintf(output, "%s\n", comment);
  if (output == stderr)
    {
      fprintf(output, "%sThanks to all contributors:\n", comment);
      fprintf(output, "%s    KAWAMOTO,Takuto       Kunihiro AIHARA       Yuji Nomura\n", comment);
    }
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
