//
// Created by c2-horosnyi on 08/12/2019.
//

#include "configurationscene.h"

void ConfigurationScene::update(double x_pos, double y_pos)
{
  button_hover = -1;
  for (int i = 0; i < CONFIGCOUNT; i++)
  {
    if (onButton(x_pos, y_pos, i))
    {
      button_hover = i;
    }
    button_picture[i]->colour(ASGE::COLOURS::LIGHTGRAY);
  }
  if (button_hover >= 0)
    button_picture[button_hover]->colour(ASGE::COLOURS::DARKBLUE);
}

bool ConfigurationScene::initScene(ASGE::Renderer* renderer)
{
  createButton(0, 100, 100, 80, 80, "", renderer);
  createButton(1, 200, 100, 80, 80, "", renderer);
  createButton(2, 300, 100, 80, 80, "", renderer);
  createButton(3, 400, 100, 80, 80, "", renderer);
  createButton(4, 100, 200, 80, 80, "", renderer);
  createButton(5, 200, 200, 80, 80, "", renderer);
  createButton(6, 300, 200, 80, 80, "", renderer);
  createButton(7, 400, 200, 80, 80, "", renderer);
  createButton(8, 100, 400, 200, 80, "", renderer);
  createButton(9, 300, 400, 200, 80, "", renderer);
  createButton(10, 500, 400, 200, 80, "", renderer);
  createButton(11, 100, 500, 100, 100, "", renderer);
  createButton(12, 400, 500, 100, 100, "", renderer);
  createButton(13, 550, 500, 100, 100, "", renderer);
  createButton(14, 100, 500, 200, 100, "", renderer);
  createButton(15, 300, 500, 200, 100, "", renderer);
  createButton(16, 500, 500, 200, 100, "", renderer);
  createButton(17, 250, 500, 100, 100, "", renderer);
  createButton(18, 400, 500, 100, 100, "", renderer);
  createButton(19, 550, 500, 100, 100, "", renderer);
  createButton(20, 100, 500, 200, 100, "", renderer);
  createButton(21, 300, 500, 200, 100, "", renderer);
  createButton(22, 500, 500, 200, 100, "", renderer);
}

bool ConfigurationScene::createButton(int id,
                                      float starting_pos_x,
                                      float starting_pos_y,
                                      float button_width,
                                      float button_height,
                                      std::string input_text,
                                      ASGE::Renderer* renderer)
{
  buttons[id].position_x = starting_pos_x;
  buttons[id].position_y = starting_pos_y;
  buttons[id].size_x = button_width;
  buttons[id].size_y = button_height;
  buttons[id].text = input_text;
  if (!loadPicture(renderer, button_width, button_height, id))
  {
    return false;
  }
  button_picture[id]->yPos(starting_pos_y);
  button_picture[id]->xPos(starting_pos_x);
  return true;
}

bool ConfigurationScene::onButton(double cursor_X,
                                  double cursor_Y,
                                  int button_identifier)
{
  if (buttons[button_identifier].position_x <= cursor_X &&
      buttons[button_identifier].position_y <= cursor_Y)
  {
    if (buttons[button_identifier].position_x +
            buttons[button_identifier].size_x >=
          cursor_X &&
        buttons[button_identifier].position_y +
            buttons[button_identifier].size_y >=
          cursor_Y)
    {
      return true;
    }
  }
  return false;
}

ConfigurationScene::~ConfigurationScene()
{
  for (int i = 0; i < CONFIGCOUNT; i++)
  {
    delete button_picture[i];
  }
}

bool ConfigurationScene::loadPicture(ASGE::Renderer* renderer,
                                     int width,
                                     int height,
                                     int id)
{
  button_picture[id] = renderer->createRawSprite();

  if (!button_picture[id]->loadTexture("/data/images/rectanglemaker.png"))
  {
    return false;
  }
  button_picture[id]->width(width);
  button_picture[id]->height(height);
  button_picture[id]->colour(ASGE::COLOURS::LIGHTGRAY);
  return true;
}

void ConfigurationScene::render(ASGE::Renderer* renderer)
{
  for (int i = 0; i < CONFIGCOUNT; i++)
  {
    renderer->renderSprite(*button_picture[i]);
    renderer->renderText(buttons[i].text,
                         buttons[i].position_x + (buttons[i].size_x / 2.0) -
                           (buttons[i].text.length() * 10),
                         buttons[i].position_y + (buttons[i].size_y / 2.0) + 15,
                         buttons[i].text_scale,
                         i != button_hover ? ASGE::COLOURS::BLACK
                                           : ASGE::COLOURS::WHITE);
  }
}

int ConfigurationScene::menuClick(double cursor_X, double cursor_Y)
{
  for (int i = 0; i < CONFIGCOUNT; i++)
  {
    if (onButton(cursor_X, cursor_Y, i))
    {
      return i;
    }
  }
  return -1;
}

int ConfigurationScene::returnColor1()
{
  return color_choice_1;
}

int ConfigurationScene::returnColor2()
{
  return color_choice_2;
}

int ConfigurationScene::returnPaddles()
{
  return paddlecount;
}

bool ConfigurationScene::returnQuickplay()
{
  return quickplay;
}

int ConfigurationScene::returnGoal()
{
  return playing_to;
}

int ConfigurationScene::returnSlide()
{
  return slidiness;
}

bool ConfigurationScene::returnEnglish()
{
  return English;
}

bool ConfigurationScene::returnStrike()
{
  return false;
}
