/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#ifndef __SYMMETRYAXISNORMAL_H__
#define __SYMMETRYAXISNORMAL_H__

#include <stdio.h>
#include "InteractiveRegularPolygon.h"
#include "CarbonAllotrope.h"

class SymmetryAxisNormal : public InteractiveRegularPolygon {
  // friend classes & functions

  // members
private:
  CarbonAllotrope* p_ca;
  SymmetryAxis* p_axis;

  // private tools
private:

  // constructors & the destructor
public:
  SymmetryAxisNormal(CarbonAllotrope* ca, SymmetryAxis* axis);
  virtual ~SymmetryAxisNormal();
  Bond& operator = (const SymmetryAxisNormal& you); /* dont use */

  // interactions
public:
  virtual void reset_interaction();
  virtual void interaction_original(OriginalForceType force_type,
                                    Interactives* interactives, double delta);

  // I/O
public:
  virtual void draw_opaque_by_OpenGL(bool selection) const;

  // member accessing methods

};

#endif /* __SYMMETRYAXISNORMAL_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
