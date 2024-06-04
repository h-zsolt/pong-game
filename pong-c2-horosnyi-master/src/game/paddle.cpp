//
// Created by c2-horosnyi on 29/11/2019.
//

#include "paddle.h"

void Paddle::setPosition(float x_position, float y_position)
{
  position_x = x_position;
  position_y = y_position;
}

void Paddle::updatePosition(float delta_value, int sliding)
{
  if (x_axis_lock)
  {
    position_y -= delta_value * speed;
    if (position_y < limits[0])
    {
      position_y = limits[0];
      speed = 0.0f;
    }
    if (position_y > limits[1])
    {
      position_y = limits[1];
      speed = 0.0f;
    }
  }
  else
  {
    position_x -= delta_value * speed;
    if (position_x < limits[0])
    {
      position_x = limits[0];
      speed = 0.0f;
    }
    if (position_x > limits[1])
    {
      position_x = limits[1];
      speed = 0.0f;
    }
  }
  if (!input_report)
  {
    changeSpeed(0, -(delta_value * 12.5 * sliding));
  }
  paddle_picture->xPos(position_x);
  paddle_picture->yPos(position_y);
  input_report = false;
}

void Paddle::strikeHandler(float gamespeed) {}

void Paddle::render(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*paddle_picture);
}

void Paddle::defineAxis(bool x_axis)
{
  x_axis_lock = !x_axis;
}

bool Paddle::loadPicture(ASGE::Renderer* renderer, ASGE::Colour paddle_colour)
{
  paddle_picture = renderer->createRawSprite();

  if (!paddle_picture->loadTexture("/data/images/rectanglemaker.png"))
  {
    return false;
  }
  paddle_picture->width(size_x);
  paddle_picture->height(size_y);
  paddle_picture->colour(paddle_colour);
  return true;
}

bool Paddle::init(float init_size_x,
                  float init_size_y,
                  float init_locaton_x,
                  float init_location_y,
                  float pos_limits[],
                  float init_top_speed)
{
  size_x = init_size_x;
  size_y = init_size_y;
  position_x = init_locaton_x;
  position_y = init_location_y;
  top_speed = init_top_speed;
  limits[0] = pos_limits[0];
  limits[1] = pos_limits[1];
  return false;
}

Paddle::~Paddle()
{
  delete paddle_picture;
}

void Paddle::changeSpeed(float amount, float percentage)
{
  speed += amount;
  speed += speed * percentage / 100.0f;
  speedLimit();
}

void Paddle::changeTopSpeed(float amount, float percentage)
{
  top_speed += amount;
  top_speed += top_speed * percentage / 100.0f;
}

void Paddle::incrementSpeed(float percentage, float delta)
{
  speed += top_speed * percentage / 30.0f * delta;
  speedLimit();
  input_report = true;
}

void Paddle::speedLimit()
{
  if (speed > top_speed)
  {
    speed = top_speed;
  }
  if (speed < -top_speed)
  {
    speed = -top_speed;
  }
}

float Paddle::reportX()
{
  return position_x;
}

float Paddle::reportY()
{
  return position_y;
}

float Paddle::reportSize(bool x_axis)
{
  if (x_axis)
  {
    return size_x;
  }
  else
  {
    return size_y;
  }
}

float Paddle::reportSpeed()
{
  return speed;
}
