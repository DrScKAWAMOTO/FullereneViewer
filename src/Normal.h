/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __NORMAL_H__
#define __NORMAL_H__

class Normal {
  // members
public:
  int clockwise;
  
  // constructors & the destructor
public:
  Normal() : clockwise(0) { }
  ~Normal() { }
  Normal& operator = (const Normal& you); /* dont use */

};

#endif /* __NORMAL_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
