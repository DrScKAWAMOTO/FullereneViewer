/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

enum ErrorCode {
  ERROR_CODE_OK = 0,
  ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_CLOSED_CONCAVE_BOUNDARY_SEGMENT = -1,
  ERROR_CODE_DOES_NOT_FIX_LENGTH_OF_OPEN_CONCAVE_BOUNDARY_SEGMENT = -2,
  ERROR_CODE_TOO_LONG_OPEN_CONCAVE_BOUNDARY_SEGMENT = -3,
  ERROR_CODE_BREAKING_ISOLATED_PENTAGON_RULE = -4,
  ERROR_CODE_ONLY_ONE_CONCAVE_AND_ONLY_ONE_CONVEX_BOUNDARY_SEGMENT = -5,
  ERROR_CODE_ILLEGAL_GENERATOR_FORMULA = -6,
};

extern void error_handler(ErrorCode result);

#define EH(result)                             \
  {                                            \
    ErrorCode val = (result);                  \
    if (val != ERROR_CODE_OK)                  \
      {                                        \
        printf("%s:%d: ", __FILE__, __LINE__); \
        error_handler(val);                    \
        exit(10);                              \
      }                                        \
  }

#endif /* __ERRORCODE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
