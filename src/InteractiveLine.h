/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __INTERACTIVELINE_H__
#define __INTERACTIVELINE_H__

#include "Interactive.h"

class InteractiveLine : public Interactive {
  // friend classes & functions

  // members

  // private tools
  // attributes
protected:
  virtual Vector3 p_get_vertex_location(int index) const;

  // constructors & the destructor
public:
  InteractiveLine(Interactives* interactives, int sequence_no);
  virtual ~InteractiveLine();
  InteractiveLine& operator = (const InteractiveLine& you); /* dont use */

  // interactions

  // representation

  // I/O

  // class decision

  // member accessing methods
  // attributes
public:
  virtual int number_of_vertices() const;
  virtual Vector3 get_nearest_point(const Vector3& by_your_location) const;

};

#endif /* __INTERACTIVELINE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
