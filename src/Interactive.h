/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */ 

#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Config.h"
#include "Object.h"
#include "Radius.h"
#include "Posture.h"
#include "Center.h"
#include "Normal.h"

class Interactives;

enum LocationForceType {
  LOCATION_FORCE_TYPE_ATTRACTIVE = 1,
  LOCATION_FORCE_TYPE_REPULSIVE = 2,
};

enum NormalForceType {
  NORMAL_FORCE_TYPE_PARALLELIZE = 3,
  NORMAL_FORCE_TYPE_DIRECTION_ARRANGEMENT = 4,
  NORMAL_FORCE_TYPE_OUTSIDE_ARRANGEMENT = 5,
};

enum OriginalForceType {
  ORIGINAL_FORCE_TYPE_ORIGINAL = 6,
};

class Interactive : public Object {
  // friend classes & functions

  // members
protected:
  Radius p_radius;
  Posture p_posture;
  Center p_center;
  Normal p_normal;
private:
  bool p_done;

  // private tools
private:
  static Vector3 s_attractive_force(const Vector3& you, const Vector3& me, double delta);
  static Vector3 s_repulsive_force(const Vector3& you, const Vector3& me, double delta);
  // attributes
protected:
  virtual Vector3 p_get_vertex_location(int index) const = 0;
  void p_interaction_at(const Vector3& my_point_location,
                        const Vector3& by_your_location,
                        LocationForceType force_type, double delta);

  // constructors & the destructor
public:
  Interactive(Interactives* interactives, int sequence_no, double radius_length);
  virtual ~Interactive();
  Interactive& operator = (const Interactive& you); /* dont use */

  // interactions
public:
  void fix_radius_length(double fixed_length);
  void fix_posture(const Matrix3& fixed_posture);
  void fix_center_location(const Vector3& fixed_location);
  void move_center_location(const Vector3& movement);
  virtual void reset_interaction();
  void interaction_at_center(const Vector3& by_your_location,
                             LocationForceType force_type, double delta);
  void interaction_at_vertex(int index, const Vector3& by_your_location,
                             LocationForceType force_type, double delta);
  void interaction_at_nearest_point(const Vector3& by_your_location,
                                    LocationForceType force_type, double delta);
  void interaction_to_normal_by_normal(const Vector3& your_normal,
                                       NormalForceType force_type,
                                       double delta);
  void interaction_to_normal_by_location(const Vector3& your_location,
                                         double delta);
  virtual void interaction_original(OriginalForceType force_type,
                                    Interactives* interactives, double delta);
  void operate_interactions(int &stability, const Vector3& center);
  void randomized_force(double width = 1.0);
  void move_by(const Vector3& a);

  // determining normal vector
public:
  void reset_clockwise() { p_normal.clockwise = 0; }
  int get_clockwise() const { return p_normal.clockwise; }
  void reset_done() { p_done = false; }
  bool get_done() const { return p_done; }
  void set_done() { p_done = true; }

  // I/O
public:
  virtual void draw_opaque_by_OpenGL(bool selection) const;
  virtual void draw_semitransparent_by_OpenGL(bool selection, bool frontface) const;
  void memory_shape(FILE* archive) const;
  void recall_shape(FILE* archive);

  // member accessing methods
  // attributes
public:
  virtual int number_of_vertices() const = 0;
  double get_radius_length() const { return p_radius.length; }
  virtual Vector3 get_center_location() const { return p_center.location; }
  Vector3 get_vertex_location(int index) const;
  virtual Vector3 get_nearest_point(const Vector3& by_your_location) const = 0;
  virtual Vector3 get_normal() const;
  virtual Vector3 get_normal(const Vector3& by_your_location) const;

};

#endif /* __INTERACTIVE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/

