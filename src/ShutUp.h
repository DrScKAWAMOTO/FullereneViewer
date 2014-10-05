/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2014/06/07 09:10:25 JST
 */

#ifndef __SHUTUP_H__
#define __SHUTUP_H__

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#endif /* __SHUTUP_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
