/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <math.h>
#include <float.h>
#include <stdio.h>
#include "Quaternion.h"
#include "Vector3.h"
#include "Utils.h"

void Quaternion::p_initialize_from_axis_and_degree(const Vector3& axis, double degree)
{
  if (fabs(degree) <= DBL_EPSILON)
    {
      p_r = 1.0;
      p_x = 0.0;
      p_y = 0.0;
      p_z = 0.0;
    }
  else
    {
      const double length = axis.abs();
      const double c = cos(degree / 360.0 * PAI);
      const double s = sin(degree / 360.0 * PAI) / length;
      p_r = c;
      p_x = axis.x() * s;
      p_y = axis.y() * s;
      p_z = axis.z() * s;
    }
}

Quaternion::Quaternion(double r, double x, double y, double z)
  : p_r(r), p_x(x), p_y(y), p_z(z)
{
}

Quaternion::Quaternion(Vector3 from, Vector3 to)
{
  double a = from.abs();
  if ((a != 1.0) && (a > DBL_EPSILON))
    from /= a;
  a = to.abs();
  if ((a != 1.0) && (a > DBL_EPSILON))
    to /= a;
  Vector3 axis = exterior_product(to, from);
  double degree = acos(inner_product(to, from)) / PAI * 180.0;
  p_initialize_from_axis_and_degree(axis, degree);
}

Quaternion::Quaternion(const Vector3& you)
  : p_r(0.0), p_x(you.x()), p_y(you.y()), p_z(you.z())
{
}

Quaternion::Quaternion(const Vector3& axis, double degree)
{
  p_initialize_from_axis_and_degree(axis, degree);
}

void Quaternion::operator = (const Quaternion& you)
{
  if (this != &you)
    {
      p_r = you.p_r;
      p_x = you.p_x;
      p_y = you.p_y;
      p_z = you.p_z;
    }
}

Quaternion::~Quaternion()
{
}

Quaternion operator * (const Quaternion& a, const Quaternion& b)
{
  return Quaternion(a.p_r * b.p_r - a.p_x * b.p_x - a.p_y * b.p_y - a.p_z * b.p_z,
                    a.p_r * b.p_x + a.p_x * b.p_r + a.p_y * b.p_z - a.p_z * b.p_y,
                    a.p_r * b.p_y + a.p_y * b.p_r + a.p_z * b.p_x - a.p_x * b.p_z,
                    a.p_r * b.p_z + a.p_z * b.p_r + a.p_x * b.p_y - a.p_y * b.p_x);
}

Quaternion operator + (const Quaternion& a, const Quaternion& b)
{
  return Quaternion(a.p_r + b.p_r, a.p_x + b.p_x, a.p_y + b.p_y, a.p_z + b.p_z);
}

Quaternion operator - (const Quaternion& a, const Quaternion& b)
{
  return Quaternion(a.p_r - b.p_r, a.p_x - b.p_x, a.p_y - b.p_y, a.p_z - b.p_z);
}

Quaternion Quaternion::conjugation() const
{
  return Quaternion(p_r, -p_x, -p_y, -p_z);
}

void Quaternion::normalize()
{
  double abs = sqrt(p_r * p_r + p_x * p_x + p_y * p_y + p_z * p_z);
  if (abs > DBL_EPSILON)
    {
      p_r /= abs;
      p_x /= abs;
      p_y /= abs;
      p_z /= abs;
    }
}

Vector3 Quaternion::operator () (const Vector3& a) const
{
  Quaternion b = Quaternion(- a.x() * p_x - a.y() * p_y - a.z() * p_z,
                            + a.x() * p_r + a.y() * p_z - a.z() * p_y,
                            + a.y() * p_r + a.z() * p_x - a.x() * p_z,
                            + a.z() * p_r + a.x() * p_y - a.y() * p_x);
  return Vector3(p_r * b.p_x - p_x * b.p_r - p_y * b.p_z + p_z * b.p_y,
                 p_r * b.p_y - p_y * b.p_r - p_z * b.p_x + p_x * b.p_z,
                 p_r * b.p_z - p_z * b.p_r - p_x * b.p_y + p_y * b.p_x);
}

void Quaternion::print_out() const
{
  printf("[%g; (%g,%g,%g)]\n", p_r, p_x, p_y, p_z);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
