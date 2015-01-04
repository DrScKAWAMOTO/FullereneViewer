/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "Vector3.h"
#include "Matrix3.h"
#include "Random.h"

Vector3::Vector3(double x, double y, double z)
  : p_x(x), p_y(y), p_z(z)
{
}

Vector3::Vector3(const Vector3& you)
  : Object(you.sequence_no()), p_x(you.p_x), p_y(you.p_y), p_z(you.p_z)
{
}

void Vector3::operator = (const Vector3& you)
{
  if (this != &you)
    {
      p_x = you.p_x;
      p_y = you.p_y;
      p_z = you.p_z;
    }
}

Vector3::~Vector3()
{
}

void Vector3::randomize(double width)
{
  p_x = Random::minus_width_to_width(width);
  p_y = Random::minus_width_to_width(width);
  p_z = Random::minus_width_to_width(width);
}

void Vector3::randomized_force(double width)
{
  p_x += Random::minus_width_to_width(width);
  p_y += Random::minus_width_to_width(width);
  p_z += Random::minus_width_to_width(width);
}

Matrix3 Vector3::moment_to_rotation() const
{
  Matrix3 result = Matrix3( 1.0, p_z,-p_y,
                           -p_z, 1.0, p_x,
                            p_y,-p_x, 1.0);
  double detr = result.det();
  assert(fabs(detr) > FLT_EPSILON);
  if (detr > 0.0)
    result *= exp(-log(detr) / 3.0);
  else
    result *= -exp(-log(-detr) / 3.0);
  return result;
}

bool Vector3::operator == (const Vector3& you) const
{
  Vector3 result = *this - you;
  return !result;
}

bool Vector3::operator != (const Vector3& you) const
{
  Vector3 result = *this - you;
  return !!result;
}

bool Vector3::operator ! () const
{
  return (abs() <= FLT_EPSILON);
}

Vector3 operator + (const Vector3& a, const Vector3& b)
{
  return Vector3(a.p_x + b.p_x, a.p_y + b.p_y, a.p_z + b.p_z);
}

Vector3 operator - (const Vector3& a, const Vector3& b)
{
  return Vector3(a.p_x - b.p_x, a.p_y - b.p_y, a.p_z - b.p_z);
}

Vector3 operator * (const Vector3& a, const Vector3& b)
{
  return Vector3(a.p_x * b.p_x, a.p_y * b.p_y, a.p_z * b.p_z);
}

Vector3 operator * (const Vector3& a, double b)
{
  return Vector3(a.p_x * b, a.p_y * b, a.p_z * b);
}

Vector3 operator / (const Vector3& a, double b)
{
  return Vector3(a.p_x / b, a.p_y / b, a.p_z / b);
}

Vector3 operator - (const Vector3& a)
{
  return Vector3(-a.p_x, -a.p_y, -a.p_z);
}

Vector3 exterior_product(const Vector3& a, const Vector3& b)
{
  return Vector3(a.p_y * b.p_z - a.p_z * b.p_y,
                 a.p_z * b.p_x - a.p_x * b.p_z,
                 a.p_x * b.p_y - a.p_y * b.p_x);
}

double inner_product(const Vector3& a, const Vector3& b)
{
  return a.p_x * b.p_x + a.p_y * b.p_y + a.p_z * b.p_z;
}

void Vector3::operator += (const Vector3& a)
{
  p_x += a.p_x;
  p_y += a.p_y;
  p_z += a.p_z;
}

void Vector3::operator -= (const Vector3& a)
{
  p_x -= a.p_x;
  p_y -= a.p_y;
  p_z -= a.p_z;
}

void Vector3::operator *= (const Matrix3& a)
{
  operator = (*this * a);
}

void Vector3::operator *= (double a)
{
  p_x *= a;
  p_y *= a;
  p_z *= a;
}

void Vector3::operator /= (double a)
{
  p_x /= a;
  p_y /= a;
  p_z /= a;
}

double Vector3::abs() const
{
  return sqrt(p_x * p_x + p_y * p_y + p_z * p_z);
}

void Vector3::print() const
{
  printf("[%g,%g,%g]\n", p_x, p_y, p_z);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
