//
// Created by c2-horosnyi on 29/11/2019.
//

#ifndef PONG_C2_HOROSNYI_MENUSCENE_H
#define PONG_C2_HOROSNYI_MENUSCENE_H

#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

enum
{
  BUTTONCOUNT = 3
};

class MenuScene
{
 public:
  MenuScene() = default;
  ~MenuScene();
  int menuKeys(const ASGE::KeyEvent* key);
  int menuClick(double cursor_X, double cursor_Y);
  bool initScene(ASGE::Renderer* renderer);
  void update(double x_pos, double y_pos);
  bool createButton(int id,
                    float starting_pos_x,
                    float starting_pos_y,
                    float button_width,
                    float button_height,
                    int character_amount,
                    char shown_text[],
                    ASGE::Renderer* renderer);
  void render(ASGE::Renderer* renderer);
  bool loadPicture(ASGE::Renderer* renderer, int width, int height, int id);

 private:
  bool onButton(double cursor_X, double cursor_Y, int button_identifier);
  int kb_selection_value = 0;
  int button_hover = -1;
  ASGE::Sprite* button_picture[BUTTONCOUNT] = { nullptr };

  //@feedback, could you not use a single template definition for both states
  // rather than defining it twice
  struct ButtonTemplate
  {
   public:
    float position_x = 0.0f;
    float position_y = 0.0f;
    float size_x = 1.0f;
    float size_y = 1.0f;
    int textlength = 0;
    char* buttontext = new char[textlength + 1];
  };
  ButtonTemplate buttons[BUTTONCOUNT];
};

#endif // PONG_C2_HOROSNYI_MENUSCENE_H
