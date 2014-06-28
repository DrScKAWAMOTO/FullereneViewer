/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2011/12/27 22:20:59 JST
 */

#include <math.h>
#include <float.h>
#include <stdio.h>
#include "Matrix3.h"
#include "Vector3.h"
#include "Random.h"
#include "Quaternion.h"
#include "Utils.h"

Matrix3::Matrix3(double xx, double xy, double xz,
                 double yx, double yy, double yz,
                 double zx, double zy, double zz)
  : p_xx(xx), p_xy(xy), p_xz(xz),
    p_yx(yx), p_yy(yy), p_yz(yz),
    p_zx(zx), p_zy(zy), p_zz(zz)
{
}

Matrix3::Matrix3(double* array)
  : p_xx(array[0]), p_xy(array[1]), p_xz(array[2]),
    p_yx(array[3]), p_yy(array[4]), p_yz(array[5]),
    p_zx(array[6]), p_zy(array[7]), p_zz(array[8])
{
}

Matrix3::Matrix3(const Matrix3& that)
  : p_xx(that.p_xx), p_xy(that.p_xy), p_xz(that.p_xz),
    p_yx(that.p_yx), p_yy(that.p_yy), p_yz(that.p_yz),
    p_zx(that.p_zx), p_zy(that.p_zy), p_zz(that.p_zz)
{
}

Matrix3::Matrix3(const Quaternion& that)
{
  p_xx = 1.0 - 2.0 * (that.p_y * that.p_y + that.p_z * that.p_z);
  p_yx = 2.0 * (that.p_y * that.p_x + that.p_r * that.p_z);
  p_zx = 2.0 * (that.p_z * that.p_x - that.p_r * that.p_y);
  p_xy = 2.0 * (that.p_x * that.p_y - that.p_r * that.p_z);
  p_yy = 1.0 - 2.0 * (that.p_z * that.p_z + that.p_x * that.p_x);
  p_zy = 2.0 * (that.p_z * that.p_y + that.p_r * that.p_x);
  p_xz = 2.0 * (that.p_x * that.p_z + that.p_r * that.p_y);
  p_yz = 2.0 * (that.p_y * that.p_z - that.p_r * that.p_x);
  p_zz = 1.0 - 2.0 * (that.p_x * that.p_x + that.p_y * that.p_y);
}

void Matrix3::operator = (const Matrix3& that)
{
  if (this != &that)
    {
      p_xx = that.p_xx;
      p_xy = that.p_xy;
      p_xz = that.p_xz;
      p_yx = that.p_yx;
      p_yy = that.p_yy;
      p_yz = that.p_yz;
      p_zx = that.p_zx;
      p_zy = that.p_zy;
      p_zz = that.p_zz;
    }
}

Matrix3::~Matrix3()
{
}

void Matrix3::randomize()
{
  double detp;
  while (1)
    {
      p_xx = Random::minus1to1();
      p_xy = Random::minus1to1();
      p_xz = Random::minus1to1();
      p_yx = Random::minus1to1();
      p_yy = Random::minus1to1();
      p_yz = Random::minus1to1();
      p_zx = Random::minus1to1();
      p_zy = Random::minus1to1();
      p_zz = Random::minus1to1();
      detp = det();
      if (fabs(detp) > FLT_EPSILON)
        break;
    }
  if (detp > 0.0)
    operator *= (exp(-log(detp) / 3.0));
  else
    operator *= (-exp(-log(-detp) / 3.0));
}

static double array44[16];

const double* Matrix3::to_array44() const
{
  array44[0] = p_xx;
  array44[1] = p_yx;
  array44[2] = p_zx;
  array44[3] = 0.0;
  array44[4] = p_xy;
  array44[5] = p_yy;
  array44[6] = p_zy;
  array44[7] = 0.0;
  array44[8] = p_xz;
  array44[9] = p_yz;
  array44[10] = p_zz;
  array44[11] = 0.0;
  array44[12] = 0.0;
  array44[13] = 0.0;
  array44[14] = 0.0;
  array44[15] = 1.0;
  return array44;
}

bool Matrix3::operator == (const Matrix3& that) const
{
  Matrix3 sub = *this - that;
  return (sub.abs() <= FLT_EPSILON);
}

Vector3 operator * (const Vector3& a, const Matrix3& b)
{
  return Vector3(a.p_x * b.p_xx + a.p_y * b.p_yx + a.p_z * b.p_zx,
                 a.p_x * b.p_xy + a.p_y * b.p_yy + a.p_z * b.p_zy,
                 a.p_x * b.p_xz + a.p_y * b.p_yz + a.p_z * b.p_zz);
}

Vector3 operator * (const Matrix3& a, const Vector3& b)
{
  return Vector3(b.p_x * a.p_xx + b.p_y * a.p_xy + b.p_z * a.p_xz,
                 b.p_x * a.p_yx + b.p_y * a.p_yy + b.p_z * a.p_yz,
                 b.p_x * a.p_zx + b.p_y * a.p_zy + b.p_z * a.p_zz);
}

Matrix3 operator * (const Matrix3& a, const Matrix3& b)
{
  return Matrix3(a.p_xx * b.p_xx + a.p_xy * b.p_yx + a.p_xz * b.p_zx,
                 a.p_xx * b.p_xy + a.p_xy * b.p_yy + a.p_xz * b.p_zy,
                 a.p_xx * b.p_xz + a.p_xy * b.p_yz + a.p_xz * b.p_zz,
                 a.p_yx * b.p_xx + a.p_yy * b.p_yx + a.p_yz * b.p_zx,
                 a.p_yx * b.p_xy + a.p_yy * b.p_yy + a.p_yz * b.p_zy,
                 a.p_yx * b.p_xz + a.p_yy * b.p_yz + a.p_yz * b.p_zz,
                 a.p_zx * b.p_xx + a.p_zy * b.p_yx + a.p_zz * b.p_zx,
                 a.p_zx * b.p_xy + a.p_zy * b.p_yy + a.p_zz * b.p_zy,
                 a.p_zx * b.p_xz + a.p_zy * b.p_yz + a.p_zz * b.p_zz);
}

Matrix3 operator - (const Matrix3& a, const Matrix3& b)
{
  return Matrix3(a.p_xx - b.p_xx, a.p_xy - b.p_xy, a.p_xz - b.p_xz,
                 a.p_yx - b.p_yx, a.p_yy - b.p_yy, a.p_yz - b.p_yz,
                 a.p_zx - b.p_zx, a.p_zy - b.p_zy, a.p_zz - b.p_zz);
}

void Matrix3::operator *= (const Matrix3& b)
{
  operator = (*this * b);
}

void Matrix3::operator *= (double b)
{
  p_xx *= b;
  p_xy *= b;
  p_xz *= b;
  p_yx *= b;
  p_yy *= b;
  p_yz *= b;
  p_zx *= b;
  p_zy *= b;
  p_zz *= b;
}

double Matrix3::det() const
{
  double d123 = p_xx * p_yy * p_zz;
  double d132 = p_xx * p_yz * p_zy;
  double d213 = p_xy * p_yx * p_zz;
  double d231 = p_xy * p_yz * p_zx;
  double d312 = p_xz * p_yx * p_zy;
  double d321 = p_xz * p_yy * p_zx;
  return d123 - d132 - d213 + d231 + d312 - d321;
}

double Matrix3::abs() const
{
  double beki = (p_xx * p_xx + p_xy * p_xy + p_xz * p_xz +
                 p_yx * p_yx + p_yy * p_yy + p_yz * p_yz +
                 p_zx * p_zx + p_zy * p_zy + p_zz * p_zz);
  return sqrt(beki);
}

Matrix3 Matrix3::inv() const
{
  Matrix3 result = Matrix3(p_yy * p_zz - p_yz * p_zy,
                           p_xz * p_zy - p_xy * p_zz,
                           p_xy * p_yz - p_xz * p_yy,
                           p_yz * p_zx - p_yx * p_zz,
                           p_xx * p_zz - p_xz * p_zx,
                           p_xz * p_yx - p_xx * p_yz,
                           p_yx * p_zy - p_yy * p_zx,
                           p_xy * p_zx - p_xx * p_zy,
                           p_xx * p_yy - p_xy * p_yx);
  result *= (1.0 / det());
  return result;
}

Matrix3 Matrix3::transpose() const
{
  return Matrix3(p_xx, p_yx, p_zx, p_xy, p_yy, p_zy, p_xz, p_yz, p_zz);
}

void Matrix3::orthonormalize()
{
  double norm = sqrt(p_xx * p_xx + p_xy * p_xy + p_xz * p_xz);
  p_xx /= norm;
  p_xy /= norm;
  p_xz /= norm;
  norm = p_yx * p_xx + p_yy * p_xy + p_yz * p_xz;
  p_yx -= norm * p_xx;
  p_yy -= norm * p_xy;
  p_yz -= norm * p_xz;
  norm = sqrt(p_yx * p_yx + p_yy * p_yy + p_yz * p_yz);
  p_yx /= norm;
  p_yy /= norm;
  p_yz /= norm;
  norm = p_zx * p_xx + p_zy * p_xy + p_zz * p_xz;
  p_zx -= norm * p_xx;
  p_zy -= norm * p_xy;
  p_zz -= norm * p_xz;
  norm = p_zx * p_yx + p_zy * p_yy + p_zz * p_yz;
  p_zx -= norm * p_yx;
  p_zy -= norm * p_yy;
  p_zz -= norm * p_yz;
  norm = sqrt(p_zx * p_zx + p_zy * p_zy + p_zz * p_zz);
  p_zx /= norm;
  p_zy /= norm;
  p_zz /= norm;
}

void Matrix3::
Eigenvalues_and_Eigenvectors(const Matrix3& target,
                             double& value1, Vector3& vector1,
                             double& value2, Vector3& vector2,
                             double& value3, Vector3& vector3)
{
  Matrix3 Vg = target;
  Matrix3 Pg = Matrix3();
  Matrix3 Pg_inv = Matrix3();
  while (1)
    {
      Vector3 axis;
      double tan2theta;
      double max = 0.0;
      double V23 = fabs(Vg.yz());
      if (V23 > max)
        {
          max = V23;
          axis = Vector3(1, 0, 0);
          tan2theta = -2.0 * Vg.yz() / (Vg.yy() - Vg.zz());
        }
      double V31 = fabs(Vg.xz());
      if (V31 > max)
        {
          max = V31;
          axis = Vector3(0, 1, 0);
          tan2theta = -2.0 * Vg.zx() / (Vg.zz() - Vg.xx());
        }
      double V12 = fabs(Vg.xy());
      if (V12 > max)
        {
          max = V12;
          axis = Vector3(0, 0, 1);
          tan2theta = -2.0 * Vg.xy() / (Vg.xx() - Vg.yy());
        }
      if (max <= FLT_EPSILON)
        break;
      double theta = -atan(tan2theta) * 90.0 / PAI;
      Matrix3 P = Matrix3(Quaternion(axis, theta));
      Matrix3 P_inv = Matrix3(Quaternion(axis, -theta));
      Vg = P_inv * Vg;
      Vg = Vg * P;
      Pg = Pg * P;
      Pg_inv = P_inv * Pg_inv;
    }
  if (Vg.xx() > Vg.yy())
    {
      if (Vg.xx() > Vg.zz())
        {
          if (Vg.yy() > Vg.zz())
            {
              // xx > yy > zz
              value1 = Vg.xx();
              value2 = Vg.yy();
              value3 = Vg.zz();
              vector1 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
              vector2 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
              vector3 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
            }
          else
            {
              // xx > zz >= yy
              value1 = Vg.xx();
              value2 = Vg.zz();
              value3 = Vg.yy();
              vector1 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
              vector2 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
              vector3 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
            }
        }
      else
        {
          // zz >= xx > yy
          value1 = Vg.zz();
          value2 = Vg.xx();
          value3 = Vg.yy();
          vector1 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
          vector2 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
          vector3 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
        }
    }
  else
    {
      if (Vg.yy() > Vg.zz())
        {
          if (Vg.xx() > Vg.zz())
            {
              // yy > xx > zz
              value1 = Vg.yy();
              value2 = Vg.xx();
              value3 = Vg.zz();
              vector1 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
              vector2 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
              vector3 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
            }
          else
            {
              // yy > zz >= xx
              value1 = Vg.yy();
              value2 = Vg.zz();
              value3 = Vg.xx();
              vector1 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
              vector2 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
              vector3 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
            }
        }
      else
        {
          // zz >= yy >= xx
          value1 = Vg.zz();
          value2 = Vg.yy();
          value3 = Vg.xx();
          vector1 = Vector3(Pg.xz(), Pg.yz(), Pg.zz());
          vector2 = Vector3(Pg.xy(), Pg.yy(), Pg.zy());
          vector3 = Vector3(Pg.xx(), Pg.yx(), Pg.zx());
        }
    }
}

void Matrix3::print(int width) const
{
  printf("/ %*.*f %*.*f %*.*f \\\n", width, width - 3, p_xx,
         width, width - 3, p_xy, width, width - 3, p_xz);
  printf("| %*.*f %*.*f %*.*f |\n", width, width - 3, p_yx,
         width, width - 3, p_yy, width, width - 3, p_yz);
  printf("\\ %*.*f %*.*f %*.*f /\n", width, width - 3, p_zx,
         width, width - 3, p_zy, width, width - 3, p_zz);
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
