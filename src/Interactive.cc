/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <float.h>
#include <assert.h>
#include "Interactive.h"
#include "Interactives.h"
#include "Random.h"
#include "ShutUp.h"

Vector3 Interactive::s_attractive_force(const Vector3& you, const Vector3& me,
                                        double delta)
{
  Vector3 diff = you - me;
  diff *= delta;
  return diff;
}

Vector3 Interactive::s_repulsive_force(const Vector3& you, const Vector3& me,
                                       double delta)
{
  Vector3 diff = me - you;
  double abs = diff.abs();
  while (abs <= FLT_EPSILON)
    {
      diff.randomize();
      abs = diff.abs();
    }
  double abs3 = abs;
  abs3 *= abs;
  abs3 *= abs;
  diff *= (delta / abs3);
  return diff;
}

void Interactive::p_interaction_at(const Vector3& my_point_location,
                                   const Vector3& by_your_location,
                                   LocationForceType force_type, double delta)
{
  const Vector3 my_center_location = get_center_location();
  const Vector3 my_point_center = my_point_location - my_center_location;
  const Vector3 my_point_direction = my_point_center / my_point_center.abs();
  Vector3 next_force;
  switch (force_type)
    {
    case LOCATION_FORCE_TYPE_ATTRACTIVE:
      next_force = s_attractive_force(by_your_location, my_point_location, delta);
      break;
    case LOCATION_FORCE_TYPE_REPULSIVE:
      next_force = s_repulsive_force(by_your_location, my_point_location, delta);
      break;
    }
  p_center.next_force += next_force;
  p_posture.next_moment += exterior_product(my_point_center, next_force);
  p_radius.next_force += inner_product(my_point_direction, next_force);
}

Interactive::Interactive(Interactives* interactives,
                         int sequence_no, double radius_length)
  : Object(sequence_no), p_radius(radius_length), p_posture(), p_center(), p_normal()
{
  p_posture.posture.randomize();
  p_center.location.randomize();
  interactives->register_interactive(this);
}

Interactive::~Interactive()
{
}

void Interactive::fix_radius_length(double fixed_length)
{
  p_radius.fixed = true;
  p_radius.fixed_length = fixed_length;
}

void Interactive::fix_posture(const Matrix3& fixed_posture)
{
  p_posture.fixed = true;
  p_posture.fixed_posture = fixed_posture;
}

void Interactive::fix_center_location(const Vector3& fixed_location)
{
  p_center.fixed = true;
  p_center.fixed_location = fixed_location;
}

void Interactive::move_center_location(const Vector3& movement)
{
  p_center.location += movement;
}

void Interactive::reset_interaction()
{
  p_radius.next_force = 0.0;
  p_posture.next_moment = 0.0;
  p_center.next_force = 0.0;
}

void Interactive::interaction_at_center(const Vector3& by_your_location,
                                        LocationForceType force_type, double delta)
{
  const Vector3 my_center_location = get_center_location();
  switch (force_type)
    {
    case LOCATION_FORCE_TYPE_ATTRACTIVE:
      p_center.next_force +=
        s_attractive_force(by_your_location, my_center_location, delta);
      break;
    case LOCATION_FORCE_TYPE_REPULSIVE:
      p_center.next_force +=
        s_repulsive_force(by_your_location, my_center_location, delta);
      break;
    }
}

void Interactive::interaction_at_vertex(int index, const Vector3& by_your_location,
                                        LocationForceType force_type, double delta)
{
  p_interaction_at(get_vertex_location(index), by_your_location, force_type, delta);
}

void Interactive::interaction_at_nearest_point(const Vector3& by_your_location,
                                               LocationForceType force_type, double delta)
{
  p_interaction_at(get_nearest_point(by_your_location), by_your_location,
                   force_type, delta);
}

void Interactive::interaction_to_normal_by_normal(const Vector3& your_normal,
                                                  NormalForceType force_type,
                                                  double delta)
{
  const Vector3 my_normal = get_normal();
  double cos = inner_product(your_normal, my_normal);
  cos /= your_normal.abs();
  cos /= my_normal.abs();
  Vector3 next_force;
  switch (force_type)
    {
    case NORMAL_FORCE_TYPE_PARALLELIZE:
      if (cos < -FLT_EPSILON)
        next_force = s_attractive_force(Vector3() - your_normal, my_normal, delta);
      else
        next_force = s_attractive_force(your_normal, my_normal, delta);
      break;
    case NORMAL_FORCE_TYPE_DIRECTION_ARRANGEMENT:
      if (cos < -0.5)
        {
          Vector3 ex = exterior_product(your_normal, my_normal);
          if (ex.abs() <= FLT_EPSILON)
            {
              ex = exterior_product(Vector3(1.0, 0.0, 0.0), my_normal);
              if (ex.abs() <= FLT_EPSILON)
                {
                  ex = exterior_product(Vector3(0.0, 1.0, 0.0), my_normal);
                }
            }
          assert(ex.abs() > FLT_EPSILON);
          ex /= ex.abs();
          next_force = ex * delta;
        }
      else
        next_force = s_attractive_force(your_normal, my_normal, delta);
      break;
    case NORMAL_FORCE_TYPE_OUTSIDE_ARRANGEMENT:
      assert(0);
      break;
    }
  p_posture.next_moment += exterior_product(my_normal, next_force);
}

void Interactive::interaction_to_normal_by_location(const Vector3& your_location,
                                                    double delta)
{
  const Vector3 my_normal = get_normal();
  const Vector3 my_location = get_center_location();
  const Vector3 you_minus_me = your_location - my_location;
  double cos = inner_product(you_minus_me, my_normal);
  cos /= you_minus_me.abs();
  cos /= my_normal.abs();
  if (cos > FLT_EPSILON)
    p_center.next_force += my_normal * cos * delta * 5.0;
}

void Interactive::interaction_original(OriginalForceType UNUSED(force_type),
                                       Interactives* UNUSED(interactives), double UNUSED(delta))
{
  assert(0);
}

void Interactive::operate_interactions(int &stability, const Vector3& center)
{
  // for simulation cut off
  p_radius.last_length = p_radius.length;
  p_posture.last_posture = p_posture.posture;
  p_center.last_location = p_center.location;

  if (p_radius.fixed)
    p_radius.length = p_radius.fixed_length;
  else
    p_radius.length += p_radius.next_force;
  if (p_posture.fixed)
    p_posture.posture = p_posture.fixed_posture;
  else
    {
      p_posture.posture *= p_posture.next_moment.moment_to_rotation();
      p_posture.posture.orthonormalize();
    }
  if (p_center.fixed)
    p_center.location = p_center.fixed_location;
  else
    {
      p_center.location += p_center.next_force;
      p_center.location -= center;
    }

  // for simulation cut off
  if (!p_radius.stabled())
    stability++;
  if (!p_posture.stabled())
    stability++;
  if (!p_center.stabled())
    stability++;
}

void Interactive::randomized_force(double width)
{
  p_radius.length += Random::minus_width_to_width(width);
  p_posture.posture.randomize();
  p_center.location.randomized_force(width);
}

void Interactive::move_by(const Vector3& a)
{
  p_center.location += a;
}

void Interactive::draw_opaque_by_OpenGL(bool UNUSED(selection)) const
{
}

void Interactive::draw_semitransparent_by_OpenGL(bool UNUSED(selection), bool UNUSED(frontface)) const
{
}

void Interactive::memory_shape(FILE* archive) const
{
  // radius
  fprintf(archive, "%g %d %g\n", p_radius.length, p_radius.fixed, p_radius.fixed_length);
  // posture
  const double* array33 = p_posture.posture.to_array33();
  fprintf(archive, "%g %g %g %g %g %g %g %g %g ", array33[0], array33[1], array33[2],
         array33[3], array33[4], array33[5], array33[6], array33[7], array33[8]);
  fprintf(archive, "%d ", p_posture.fixed);
  array33 = p_posture.fixed_posture.to_array33();
  fprintf(archive, "%g %g %g %g %g %g %g %g %g\n", array33[0], array33[1], array33[2],
         array33[3], array33[4], array33[5], array33[6], array33[7], array33[8]);
  // center
  fprintf(archive, "%g %g %g %d %g %g %g\n", p_center.location.x(), p_center.location.y(),
         p_center.location.z(), p_center.fixed, p_center.fixed_location.x(),
         p_center.fixed_location.y(), p_center.fixed_location.z());
  // normal
  fprintf(archive, "%d\n", p_normal.clockwise);
}

void Interactive::recall_shape(FILE* archive)
{
  double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  int a;
  // radius
  fscanf(archive, "%lg %d %lg", &xx, &a, &xy);
  p_radius.length = xx;
  p_radius.fixed = !!a;
  p_radius.fixed_length = xy;
  // posture
  fscanf(archive, "%lg %lg %lg %lg %lg %lg %lg %lg %lg",
         &xx, &xy, &xz, &yx, &yy, &yz, &zx, &zy, &zz);
  p_posture.posture = Matrix3(xx, xy, xz, yx, yy, yz, zx, zy, zz);
  fscanf(archive, "%d", &a);
  p_posture.fixed = !!a;
  fscanf(archive, "%lg %lg %lg %lg %lg %lg %lg %lg %lg",
         &xx, &xy, &xz, &yx, &yy, &yz, &zx, &zy, &zz);
  p_posture.fixed_posture = Matrix3(xx, xy, xz, yx, yy, yz, zx, zy, zz);
  // center
  fscanf(archive, "%lg %lg %lg %d %lg %lg %lg\n", &xx, &xy, &xz, &a, &yx, &yy, &yz);
  p_center.location = Vector3(xx, xy, xz);
  p_center.fixed = !!a;
  p_center.fixed_location = Vector3(yx, yy, yz);
  // normal
  fscanf(archive, "%d", &a);
  p_normal.clockwise = a;
}

Vector3 Interactive::get_vertex_location(int index) const
{
  return (p_get_vertex_location(index) * p_posture.posture * p_radius.length +
          p_center.location);
}

Vector3 Interactive::get_normal() const
{
  return Vector3(0.0, 0.0, (double)p_normal.clockwise) * p_posture.posture;
}

Vector3 Interactive::get_normal(const Vector3& UNUSED(by_your_location)) const
{
  return get_normal();
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
