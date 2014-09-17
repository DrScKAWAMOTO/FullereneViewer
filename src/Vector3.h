/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/27 22:20:59 JST
 */

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "Object.h"

class Matrix3;

class Vector3 : public Object {
  // friend classes & functions

  // members
private:
  double p_x, p_y, p_z;

  // private tools

  // constructors & the destructor
public:
  Vector3(double x = 0.0, double y = 0.0, double z = 0.0);
  Vector3(const Vector3& you);
  void operator = (const Vector3& you);
  ~Vector3();
  void randomize(double width = 1.0);
  void randomized_force(double width = 1.0);

  // type converters
public:
  Matrix3 moment_to_rotation() const;

  // comparators
public:
  bool operator ! () const;
  bool operator == (const Vector3& you) const;
  bool operator != (const Vector3& you) const;

  // math operators
public:
  friend Vector3 operator + (const Vector3& a, const Vector3& b);
  friend Vector3 operator - (const Vector3& a, const Vector3& b);
  friend Vector3 operator * (const Vector3& a, const Vector3& b);
  friend Vector3 operator * (const Vector3& a, const Matrix3& b);
  friend Vector3 operator * (const Matrix3& a, const Vector3& b);
  friend Vector3 operator * (const Vector3& a, double b);
  friend Vector3 operator / (const Vector3& a, double b);
  friend Vector3 exterior_product(const Vector3& a, const Vector3& b);
  friend double inner_product(const Vector3& a, const Vector3& b);
  void operator += (const Vector3& a);
  void operator -= (const Vector3& a);
  void operator *= (const Matrix3& a);
  void operator *= (double a);
  void operator /= (double a);
  double abs() const;
  void affinitive_force_to_XY_plane() { p_z = 0.0; }

  // I/O
public:
  void print() const;

  // member accessing methods
public:
  double x() const { return p_x; }
  double y() const { return p_y; }
  double z() const { return p_z; }
  double& x() { return p_x; }
  double& y() { return p_y; }
  double& z() { return p_z; }

};

#endif /* __VECTOR3_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
