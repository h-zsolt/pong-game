//
// Created by c2-horosnyi on 29/11/2019.
//

#include "ball.h"
#include <Engine/DebugPrinter.h>

Ball::~Ball()
{
  delete ball_picture;
}

bool Ball::init(float init_speed,
                float init_angle,
                int init_state,
                int init_type,
                int init_size,
                float x_init,
                float y_init,
                bool english_rule)
{
  setSize(init_size);
  setSpeed(init_speed);
  setAngle(init_angle);
  changeState(init_state);
  defineType(init_type);
  position_x = x_init;
  position_y = y_init;
  english = english_rule;
}

bool Ball::loadPicture(ASGE::Renderer* renderer)
{
  ball_picture = renderer->createRawSprite();

  if (!ball_picture->loadTexture("/data/images/rectanglemaker.png"))
  {
    return false;
  }
  ball_picture->width(size);
  ball_picture->height(size);
  return true;
}

void Ball::setSize(int value)
{
  size = value;
  if (ball_picture != nullptr)
  {
    ball_picture->width(value);
    ball_picture->height(value);
  }
}

void Ball::setSpeed(float value)
{
  speed = value;
}

void Ball::setAngle(float value)
{
  angle = value;
}

void Ball::defineType(int definedvalue)
{
  type = definedvalue;
}

bool Ball::changeState(int value)
{
  state += value;
  if (state <= 0)
    return true;
  return false;
}

void Ball::render(ASGE::Renderer* renderer) const
{
  renderer->renderSprite(*ball_picture);
}

void Ball::updatePosition(float delta_value)
{
  if (english)
  {
    position_x += delta_value * speed;
    position_y += ((angle - 90.0f) / 90.0f) * delta_value * abs(speed);
  }
  else
  {
    position_x += (1 - abs((angle - 90.0f) / 90.0f)) * delta_value * speed;
    position_y += ((angle - 90.0f) / 90.0f) * delta_value * abs(speed);
  }
  ball_picture->xPos(position_x);
  ball_picture->yPos(position_y);
}

float Ball::reportX()
{
  return position_x;
}

float Ball::reportY()
{
  return position_y;
}

void Ball::changeColour(ASGE::Colour set_colour)
{
  ball_picture->colour(set_colour);
}

void Ball::changeSpeed(float amount, int percentage)
{
  speed += amount;
  speed += speed * percentage / 100.0f;
  ASGE::DebugPrinter{} << "Speed: " << speed << std::endl;
}

void Ball::changeAngle(float amount)
{
  angle += amount;
}

float Ball::reportAngle() const
{
  return angle;
}

bool Ball::reportHeading()
{
  if (speed > 0)
  {
    return true;
  }
  return false;
}

float Ball::reportSize()
{
  return size;
}

float Ball::reportSpeed()
{
  return speed;
}
