/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 * Create: 2012/01/21 17:43:41 JST
 */

#include <assert.h>
#include "Config.h"
#include "Interactives.h"
#include "OpenGLUtil.h"
#include "DebugMemory.h"

void Interactives::p_calculate_interaction(LocationForceType force_type, double delta,
                                           Interactive* one, int one_index,
                                           Interactive* the_other, int the_other_index)
{
  Vector3 by_your_location;
  if (the_other_index == ACTION_LOCATION_CENTER)
    by_your_location = the_other->get_center_location();
  else if (the_other_index == ACTION_LOCATION_NEAREST)
    by_your_location = the_other->get_nearest_point(one->get_center_location());
  else
    by_your_location = the_other->get_vertex_location(the_other_index);
  if (one_index == ACTION_LOCATION_CENTER)
    one->interaction_at_center(by_your_location, force_type, delta);
  else if (one_index == ACTION_LOCATION_NEAREST)
    one->interaction_at_nearest_point(by_your_location, force_type, delta);
  else
    one->interaction_at_vertex(one_index, by_your_location, force_type, delta);
}

void Interactives::p_calculate_interaction(NormalForceType force_type, double delta,
                                           Interactive* one, Interactive* the_other)
{
  if (force_type == NORMAL_FORCE_TYPE_OUTSIDE_ARRANGEMENT)
    one->interaction_to_normal_by_location(the_other->get_center_location(), delta);
  else
    one->interaction_to_normal_by_normal(the_other->
                                         get_normal(one->get_center_location()),
                                         force_type, delta);
}

Interactives::Interactives()
{
}

Interactives::~Interactives()
{
}

/* one_index, the_other_index は以下のどれか
   ACTION_LOCATION_CENTER
   ACTION_LOCATION_NEAREST
   ACTION_LOCATION_VERTEX + vertex_index
*/
void Interactives::p_register_interaction(LocationForceType force_type,
                                          Interactive* one, int one_index,
                                          Interactive* the_other, int the_other_index)
{
  InteractiveOperation* op = new InteractiveOperation();
  op->force_type = force_type;
  op->one = one;
  op->one_index = one_index;
  op->the_other = the_other;
  op->the_other_index = the_other_index;
  p_operations.add(op);
}

void Interactives::p_register_interaction(NormalForceType force_type,
                                          Interactive* one, Interactive* the_other)
{
  InteractiveOperation* op = new InteractiveOperation();
  op->force_type = force_type;
  op->one = one;
  op->one_index = 0;
  op->the_other = the_other;
  op->the_other_index = 0;
  p_operations.add(op);
}

void Interactives::p_register_interaction(OriginalForceType force_type, Interactive* one)
{
  InteractiveOperation* op = new InteractiveOperation();
  op->force_type = force_type;
  op->one = one;
  op->one_index = 0;
  op->the_other = 0;
  op->the_other_index = 0;
  p_operations.add(op);
}

void Interactives::register_interactive(Interactive* interactive)
{
  p_interactives.add(interactive);
}

void Interactives::operate_interactions(double delta)
{
  const char unknown_force_type = 0;
  int ope_len = p_operations.length();
  int int_len = p_interactives.length();
  for (int i = 0; i < int_len; ++i)
    {
      Interactive* interactive = p_interactives[i];
      interactive->reset_interaction();
    }
  for (int i = 0; i < ope_len; ++i)
    {
      InteractiveOperation* op = p_operations[i];
      switch (op->force_type)
        {
        case LOCATION_FORCE_TYPE_ATTRACTIVE:
        case LOCATION_FORCE_TYPE_REPULSIVE:
          p_calculate_interaction((LocationForceType)op->force_type, delta,
                                  op->one, op->one_index,
                                  op->the_other, op->the_other_index);
          p_calculate_interaction((LocationForceType)op->force_type, delta,
                                  op->the_other, op->the_other_index,
                                  op->one, op->one_index);
          break;
        case NORMAL_FORCE_TYPE_PARALLELIZE:
        case NORMAL_FORCE_TYPE_DIRECTION_ARRANGEMENT:
          p_calculate_interaction((NormalForceType)op->force_type, delta,
                                  op->one, op->the_other);
          p_calculate_interaction((NormalForceType)op->force_type, delta,
                                  op->the_other, op->one);
          break;
        case NORMAL_FORCE_TYPE_OUTSIDE_ARRANGEMENT:
          p_calculate_interaction((NormalForceType)op->force_type, delta,
                                  op->one, op->the_other);
          break;
        case ORIGINAL_FORCE_TYPE_ORIGINAL:
          op->one->interaction_original((OriginalForceType)op->force_type, this, delta);
          break;
        default:
          assert(unknown_force_type);
          break;
        }
    }
  for (int i = 0; i < int_len; ++i)
    {
      Interactive* interactive = p_interactives[i];
      interactive->operate_interactions();
    }
}

void Interactives::randomized_force(double width)
{
  int len = p_interactives.length();
  for (int i = 0; i < len; ++i)
    p_interactives[i]->randomized_force(width);
}

void Interactives::draw_by_OpenGL(bool selection) const
{
  int len = p_interactives.length();
  for (int i = 0; i < len; ++i)
    {
      Interactive* interactive = p_interactives[i];
      interactive->draw_opaque_by_OpenGL(selection);
    }
#if defined(CONFIG_PUT_COLORED_CELLOPHANE_IN_5_MEMBER_RING)
  OpenGLUtil::semitransparent_mode();
  OpenGLUtil::backface_mode();
  for (int i = 0; i < len; ++i)
    {
      Interactive* interactive = p_interactives[i];
      interactive->draw_semitransparent_by_OpenGL(selection, false);
    }
  OpenGLUtil::frontface_mode();
  for (int i = 0; i < len; ++i)
    {
      Interactive* interactive = p_interactives[i];
      interactive->draw_semitransparent_by_OpenGL(selection, true);
    }
  OpenGLUtil::opaque_mode();
#endif
}

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
