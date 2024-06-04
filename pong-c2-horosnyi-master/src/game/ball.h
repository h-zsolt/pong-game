//
// Created by c2-horosnyi on 29/11/2019.
//

#ifndef PONG_C2_HOROSNYI_BALL_H
#define PONG_C2_HOROSNYI_BALL_H
#include <Engine/GameTime.h>
#include <Engine/OGLGame.h>
#include <Engine/Renderer.h>
#include <Engine/Sprite.h>

class Ball
{
 public:
  Ball() = default;
  ~Ball();

  void changeSpeed(float amount, int percentage);
  void setSpeed(float base);
  void setAngle(float angle);
  void changeAngle(float amount);
  void updatePosition(float delta_value);
  bool changeState(int value);
  void defineType(int definedvalue);
  void setSize(int value);
  bool loadPicture(ASGE::Renderer* renderer);
  bool init(float init_speed,
            float init_angle,
            int init_state,
            int init_type,
            int init_size,
            float x_position,
            float y_position,
            bool english_rule);
  void render(ASGE::Renderer* renderer) const;
  float reportX();
  float reportY();
  float reportAngle() const;
  bool reportHeading();
  float reportSize();
  float reportSpeed();
  void changeColour(ASGE::Colour set_colour);

 private:
  float speed = 1.0f;
  float position_x = 0.0f;
  float position_y = 0.0f;
  float angle = 90.0f;
  int state = 0;
  int type = 0;
  float size = 2.0f;
  ASGE::Sprite* ball_picture = nullptr;
  bool english = true;
  bool saved_data = true;
};

#endif // PONG_C2_HOROSNYI_BALL_H
