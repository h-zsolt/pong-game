//
// Created by c2-horosnyi on 04/12/2019.
//

#include "pmvector.h"

float pmvector::collision(bool axis_x,
                          float start_point,
                          float end_point,
                          float slicer)
{
  if (axis_x)
  {
    if (new_y - old_y != 0)
    {
      float ratio = (new_x - old_x) / (new_y - old_y);
      float arbitrary = old_y * ratio - old_x;
      float meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_y, new_y, slicer))
      {
        return meeting;
      }
    }
  }
  else
  {
    if (new_x - old_x != 0)
    {
      float ratio = (new_y - old_y) / (new_x - old_x);
      float arbitrary = old_x * ratio - old_y;
      float meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_x, new_x, slicer))
      {
        return meeting;
      }
    }
  }
  return -1;
}

void pmvector::setvalue(float value_x,
                        float value_y,
                        float value_ghost_x,
                        float value_ghost_y)
{
  new_x = value_x;
  new_y = value_y;
  old_x = value_ghost_x;
  old_y = value_ghost_y;
}

bool pmvector::hasHappened(float old_value, float new_value, float check_this)
{
  if (old_value < new_value)
  {
    if (check_this >= old_value && check_this <= new_value)
    {
      return true;
    }
  }
  else
  {
    if (check_this <= old_value && check_this >= new_value)
    {
      return true;
    }
  }
  return false;
}

/*void pmvector::debug(float debug_x, float debug_y, float debug_ox, float
debug_oy, float debug_s, float debug_m, float debug_a)
{
    //nothing to see here
}*/
