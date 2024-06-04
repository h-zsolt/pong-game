//
// Created by c2-horosnyi on 29/11/2019.
//

#ifndef PONG_C2_HOROSNYI_PADDLE_H
#define PONG_C2_HOROSNYI_PADDLE_H
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

class Paddle
{
 public:
  Paddle() = default;
  ~Paddle();
  void setPosition(float x_position, float y_position);
  void updatePosition(float delta_value, int sliding);
  void changeSpeed(float amount, float percentage);
  void incrementSpeed(float percentage, float delta);
  void changeTopSpeed(float amount, float percentage);
  void strikeHandler(float gamespeed);
  void defineAxis(bool x_axis);
  bool init(float init_size_x,
            float init_size_y,
            float init_locaton_x,
            float init_location_y,
            float pos_limits[],
            float init_top_speed);
  bool loadPicture(ASGE::Renderer* renderer, ASGE::Colour paddle_colour);
  void render(ASGE::Renderer* renderer);
  float reportX();
  float reportY();
  float reportSize(bool x_axis);
  float reportSpeed();

 private:
  ASGE::Sprite* paddle_picture = nullptr;
  float size_x = 10.0f;
  float size_y = 10.0f;
  bool doStrike();
  float position_x = 0.0f;
  float position_y = 0.0f;
  float limits[2] = { 0.0f };
  float speed = 0.0f;
  float top_speed = 1.0f;
  bool x_axis_lock = false;
  float stamina = 1000.0f;
  bool execute_strike = false;
  bool input_report = false;
  void speedLimit();
};

#endif // PONG_C2_HOROSNYI_PADDLE_H
