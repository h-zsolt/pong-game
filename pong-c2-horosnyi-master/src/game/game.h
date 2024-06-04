#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "configurationscene.h"
#include "gamescene.h"
#include "menuscene.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class Pong : public ASGE::OGLGame
{
 public:
  Pong();
  ~Pong() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void mouseMoveHandler(ASGE::SharedEventData data);
  void setupResolution();
  void menuAction(int action);

  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;
  void resetClassic();
  void resetArcade();

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  int mouse_move_callback_id = -1;
  bool in_menu = true;
  bool in_classic = false;
  bool in_arcade = false;
  bool in_config = false;
  MenuScene* menu = nullptr;
  GameScene* classic_mode = nullptr;
  GameScene* arcade_mode = nullptr;
  ConfigurationScene* arcade_config = nullptr;
};