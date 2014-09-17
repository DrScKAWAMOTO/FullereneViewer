/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/25 11:35:13 JST
 */

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

class Vector3;

class Quaternion {
  // friend classes & functions
  friend class Matrix3;

  // members
private:
  double p_r, p_x, p_y, p_z;

  // private tools
private:
  void p_initialize_from_axis_and_degree(const Vector3& axis, double degree);

  // constructors & the destructor
public:
  Quaternion(double r, double x, double y, double z);
  Quaternion(Vector3 from, Vector3 to);
  Quaternion(const Vector3& you);
  Quaternion(const Vector3& axis, double degree);
  void operator = (const Quaternion& you);
  ~Quaternion();

  // type converters

  // comparators

  // math operators
public:
  friend Quaternion operator * (const Quaternion& a, const Quaternion& b);
  friend Quaternion operator + (const Quaternion& a, const Quaternion& b);
  friend Quaternion operator - (const Quaternion& a, const Quaternion& b);
  Quaternion conjugation() const;
  void normalize();
  Vector3 operator () (const Vector3& a) const;
  // V * Matrix3(Q) = Q(V)

  // I/O
public:
  void print_out() const;

  // class decision

  // member accessing methods

};

#endif /* __QUATERNION_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
