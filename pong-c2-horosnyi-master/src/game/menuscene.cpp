//
// Created by c2-horosnyi on 29/11/2019.
//

#include "menuscene.h"

enum
{

};

void MenuScene::update(double x_pos, double y_pos)
{
  button_hover = -1;
  for (int i = 0; i < BUTTONCOUNT; i++)
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

int MenuScene::menuKeys(const ASGE::KeyEvent* key)
{
  //@feedback, simplify your checks
  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    // if key
  }
  else if (key->action == ASGE::KEYS::KEY_RELEASED)
  {
    // if key
  }

  if (key->key == ASGE::KEYS::KEY_UP && key->action == ASGE::KEYS::KEY_RELEASED)
  {
    kb_selection_value--;
  }
  else if (key->key == ASGE::KEYS::KEY_DOWN &&
           key->action == ASGE::KEYS::KEY_RELEASED)
  {
    kb_selection_value++;
  }
  else if (key->key == ASGE::KEYS::KEY_ENTER &&
           key->action == ASGE::KEYS::KEY_RELEASED)
  {
  }

  if (kb_selection_value >= BUTTONCOUNT)
  {
    kb_selection_value = BUTTONCOUNT - 1;
  }
  if (kb_selection_value < 0)
  {
    kb_selection_value = 0;
  }
  if (key->key == ASGE::KEYS::KEY_SPACE &&
      key->action == ASGE::KEYS::KEY_RELEASED)
  {
    return kb_selection_value;
  }
  if (key->key == ASGE::KEYS::KEY_ENTER &&
      key->action == ASGE::KEYS::KEY_RELEASED)
  {
    return kb_selection_value;
  }
  return -1;
}

bool MenuScene::initScene(ASGE::Renderer* renderer)
{
  char display_text1[8] = { 'C', 'l', 'a', 's', 's', 'i', 'c' };
  createButton(0, 200, 100, 600, 150, 7, display_text1, renderer);
  char display_text2[7] = { 'A', 'r', 'c', 'a', 'd', 'e' };
  createButton(1, 200, 300, 600, 150, 6, display_text2, renderer);
  char display_text3[5] = { 'E', 'x', 'i', 't' };
  createButton(2, 200, 500, 600, 150, 4, display_text3, renderer);
  int test = 0;
}

bool MenuScene::createButton(int id,
                             float starting_pos_x,
                             float starting_pos_y,
                             float button_width,
                             float button_height,
                             int character_amount,
                             char shown_text[],
                             ASGE::Renderer* renderer)
{
  buttons[id].position_x = starting_pos_x;
  buttons[id].position_y = starting_pos_y;
  buttons[id].size_x = button_width;
  buttons[id].size_y = button_height;
  buttons[id].textlength = character_amount;

  //@feedback, just use std::strings
  delete buttons[id].buttontext;

  buttons[id].buttontext = new char[buttons[id].textlength + 1];
  for (int i = 0; i < buttons[id].textlength; i++)
  {
    buttons[id].buttontext[i] = shown_text[i];
  }
  buttons[id].buttontext[buttons[id].textlength] = '\0';
  if (!loadPicture(renderer, button_width, button_height, id))
  {
    return false;
  }
  button_picture[id]->yPos(starting_pos_y);
  button_picture[id]->xPos(starting_pos_x);
  return true;
}

bool MenuScene::onButton(double cursor_X,
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

MenuScene::~MenuScene()
{
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    delete button_picture[i];
  }
  delete buttons->buttontext;
}

bool MenuScene::loadPicture(ASGE::Renderer* renderer,
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

void MenuScene::render(ASGE::Renderer* renderer)
{
  int text_maker_length = buttons[kb_selection_value].textlength + 2;
  char* selection_text = new char[text_maker_length];
  selection_text[0] = '>';
  for (int i = 1; i < text_maker_length; i++)
  {
    selection_text[i] = buttons[kb_selection_value].buttontext[i - 1];
  }
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    renderer->renderSprite(*button_picture[i]);
    renderer->renderText(
      i == kb_selection_value ? selection_text : buttons[i].buttontext,
      i == kb_selection_value
        ? buttons[i].position_x + (buttons[i].size_x / 2.0) -
            (buttons[i].textlength * 10) - 19
        : buttons[i].position_x + (buttons[i].size_x / 2.0) -
            (buttons[i].textlength * 10),
      buttons[i].position_y + (buttons[i].size_y / 2.0) + 15,
      1.75f,
      i != button_hover ? ASGE::COLOURS::BLACK : ASGE::COLOURS::WHITE);
  }
  delete[] selection_text;
}

int MenuScene::menuClick(double cursor_X, double cursor_Y)
{
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (onButton(cursor_X, cursor_Y, i))
    {
      return i;
    }
  }
  return -1;
}