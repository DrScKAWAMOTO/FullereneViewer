/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/27 22:20:59 JST
 */

#ifndef __MATRIX3_H__
#define __MATRIX3_H__

class Vector3;
class Quaternion;

class Matrix3 {
  // friend classes & functions

  // members
private:
  double p_xx, p_xy, p_xz;
  double p_yx, p_yy, p_yz;
  double p_zx, p_zy, p_zz;

  // private tools

  // constructors & the destructor
public:
  Matrix3(double xx = 1.0, double xy = 0.0, double xz = 0.0,
          double yx = 0.0, double yy = 1.0, double yz = 0.0,
          double zx = 0.0, double zy = 0.0, double zz = 1.0);
  Matrix3(double* array);
  Matrix3(const Matrix3& that);
  Matrix3(const Quaternion& that);
  // Matrix3(Q1 * Q2) = Matrix3(Q1) * Matrix3(Q2)
  // V * Matrix3(Q) = Vector3(Q.conjugation() * Quaternion(V) * Q)
  void operator = (const Matrix3& that);
  ~Matrix3();
  void randomize();

  // type converters
  const double* to_array44() const;

  // comparators
  bool operator == (const Matrix3& that);

  // math operators
  friend Vector3 operator * (const Vector3& a, const Matrix3& b);
  friend Vector3 operator * (const Matrix3& a, const Vector3& b);
  friend Matrix3 operator * (const Matrix3& a, const Matrix3& b);
  friend Matrix3 operator - (const Matrix3& a, const Matrix3& b);
  void operator *= (const Matrix3& b);
  void operator *= (double b);
  double det() const;
  double abs() const;
  Matrix3 inv() const;
  Matrix3 transpose() const;
  void orthonormalize();
  static void Eigenvalues_and_Eigenvectors(const Matrix3& target,
                                           double& value1, Vector3& vector1,
                                           double& value2, Vector3& vector2,
                                           double& value3, Vector3& vector3);

  // I/O
public:
  void print(int width) const;

  // class decision

  // member accessing methods
public:
  double xx() const { return p_xx; }
  double xy() const { return p_xy; }
  double xz() const { return p_xz; }
  double yx() const { return p_yx; }
  double yy() const { return p_yy; }
  double yz() const { return p_yz; }
  double zx() const { return p_zx; }
  double zy() const { return p_zy; }
  double zz() const { return p_zz; }
  double& xx() { return p_xx; }
  double& xy() { return p_xy; }
  double& xz() { return p_xz; }
  double& yx() { return p_yx; }
  double& yy() { return p_yy; }
  double& yz() { return p_yz; }
  double& zx() { return p_zx; }
  double& zy() { return p_zy; }
  double& zz() { return p_zz; }

};

#endif /* __MATRIX3_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
