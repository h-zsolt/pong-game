//
// Created by c2-horosnyi on 04/12/2019.
//

#ifndef PONG_C2_HOROSNYI_PMVECTOR_H
#define PONG_C2_HOROSNYI_PMVECTOR_H

class pmvector
{
 public:
  void setvalue(float value_x,
                float value_y,
                float value_ghost_x,
                float value_ghost_y);
  float
  collision(bool axis_x, float start_point, float end_point, float slicer);
  bool hasHappened(float old_value, float new_value, float check_this);
  // void debug(float debug_x, float debug_y, float debug_ox, float debug_oy,
  // float debug_s, float debug_m, float debug_a);
 private:
  float old_x;
  float new_x;
  float old_y;
  float new_y;
};

#endif // PONG_C2_HOROSNYI_PMVECTOR_H
