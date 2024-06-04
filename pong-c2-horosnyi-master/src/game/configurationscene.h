//
// Created by c2-horosnyi on 08/12/2019.
//

#ifndef PONG_C2_HOROSNYI_CONFIGURATIONSCENE_H
#define PONG_C2_HOROSNYI_CONFIGURATIONSCENE_H

#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

enum
{
  CONFIGCOUNT = 23
  // Colours, Paddle Count, QuickPlay, Playing To, Slidiness, English or Smooth,
  // Strikes, Start or Back
};

class ConfigurationScene
{
 public:
  ConfigurationScene() = default;
  ~ConfigurationScene();

  bool initScene(ASGE::Renderer* renderer);
  void update(double x_pos, double y_pos);
  bool createButton(int id,
                    float starting_pos_x,
                    float starting_pos_y,
                    float button_width,
                    float button_height,
                    std::string text_input,
                    ASGE::Renderer* renderer);
  void render(ASGE::Renderer* renderer);
  bool loadPicture(ASGE::Renderer* renderer, int width, int height, int id);

  //@feedback, never used
  int returnColor1();
  int returnColor2();
  int returnPaddles();
  bool returnQuickplay();
  int returnGoal();
  int returnSlide();
  bool returnEnglish();
  bool returnStrike();
  int menuKeys(const ASGE::KeyEvent* key);
  int menuClick(double cursor_X, double cursor_Y);

 private:
  struct buttontemplate
  {
   public:
    float position_x = 0.0f;
    float position_y = 0.0f;
    float size_x = 1.0f;
    float size_y = 1.0f;
    std::string text = "";
    float text_scale = 1.00f;
  };

 private:
  bool onButton(double cursor_X, double cursor_Y, int button_identifier);
  int button_hover = -1;
  ASGE::Sprite* button_picture[CONFIGCOUNT] = { nullptr };
  buttontemplate buttons[CONFIGCOUNT];

  int color_choice_1 = 1;
  int color_choice_2 = 2;
  int paddlecount = 2;
  bool quickplay = false;
  int playing_to = 11;
  int slidiness = 100;
  bool English = true;
  bool Strikes = false;
};

#endif // PONG_C2_HOROSNYI_CONFIGURATIONSCENE_H
