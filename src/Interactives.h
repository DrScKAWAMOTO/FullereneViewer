/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 17:43:41 JST
 */ 

#ifndef __INTERACTIVES_H__
#define __INTERACTIVES_H__

#include "Object.h"
#include "List.h"
#include "InteractiveOperation.h"
#include "Interactive.h"

enum ActionLocation {
  ACTION_LOCATION_CENTER = -1,
  ACTION_LOCATION_NEAREST = -2,
  ACTION_LOCATION_VERTEX = 0,
};

#define STABILITY_THRESHOLD 100

class Interactives : public Object {
  // friend classes & functions

  // members
private:
  List<InteractiveOperation> p_operations;
  List<Interactive> p_interactives;
  int p_simulation_active;

  // private tools
private:
  void p_calculate_interaction(LocationForceType force_type, double delta,
                               Interactive* one, int one_index,
                               Interactive* the_other, int the_other_index);
  void p_calculate_interaction(NormalForceType force_type, double delta,
                               Interactive* one, Interactive* the_other);

  // constructors & the destructor
public:
  Interactives();
  virtual ~Interactives();
  Interactives& operator = (const Interactives& that); /* dont use */

  // interactions
protected:
  void p_register_interaction(LocationForceType force_type,
                              Interactive* one, int one_index,
                              Interactive* the_other, int the_other_index);
  void p_register_interaction(NormalForceType force_type,
                              Interactive* one, Interactive* the_other);
  void p_register_interaction(OriginalForceType force_type, Interactive* one);
public:
  void register_interactive(Interactive* interactive);
  void operate_interactions(double delta);
  void randomized_force(double width = 1.0);

  // I/O
public:
  void draw_by_OpenGL(bool selection) const;

  // stability
  void resume_simulation() { p_simulation_active = STABILITY_THRESHOLD; }

  // member accessing methods

};

#endif /* __INTERACTIVES_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

