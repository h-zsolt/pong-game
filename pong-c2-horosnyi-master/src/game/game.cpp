#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "game.h"
/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */

namespace
{
enum
{
  WINDOWX = 1280,
  WINDOWY = 720,
  START_SPEED = 300,
  CLASSIC_PADDLE_COUNT = 2,
  CLASSIC_SCORE_REQUIREMENT = 11,
  CLASSIC_SLIDE = 100,
  ARCADE_SLIDE = 20
};
}

Pong::Pong()
{
  game_name = "Pong by Csongor-Zsolt Horosnyi";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
Pong::~Pong()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_move_callback_id));

  delete[] menu;
  delete[] classic_mode;
  // delete[] arcade_mode;
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool Pong::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  toggleFPS();

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Pong::keyHandler, this);

  mouse_move_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_MOVE, &Pong::mouseMoveHandler, this);

  mouse_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Pong::clickHandler, this);

  menu = new MenuScene;
  menu->initScene(renderer.get());

  arcade_config = new ConfigurationScene;
  arcade_config->initScene(renderer.get());

  classic_mode = new GameScene;
  classic_mode->initScene(renderer.get(),
                          WINDOWX,
                          WINDOWY,
                          CLASSIC_PADDLE_COUNT,
                          false,
                          true,
                          CLASSIC_SCORE_REQUIREMENT,
                          START_SPEED,
                          CLASSIC_SLIDE,
                          ASGE::COLOURS::WHITE,
                          ASGE::COLOURS::WHITE);

  arcade_mode = new GameScene;
  // TODO:
  // Arcade Mode
  // Config Menu
  // Finish Classic
  // Finish Paddle
  // Find Incorrect Destructor

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void Pong::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/
  // Scaling_and_MultiResolution_in_2D_Games.php

  // 720p is a pretty modest starting point, consider 1080p
  game_width = WINDOWX;
  game_height = WINDOWY;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void Pong::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (in_menu)
  {
    menuAction(menu->menuKeys(key));
  }

  else if (in_classic)
  {
    classic_mode->keyProcessing(key);
  }
}

//@feedback, should be in menu code, get the menu to return transitions
void Pong::menuAction(int action)
{
  switch (action)
  {
    case 0:
      in_menu = false;
      in_classic = true;
      break;

    case 1:
    {
      in_config = true;
      in_menu = false;
      break;
    }

    case 2:
      signalExit();
      break;
    default:
      break;
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void Pong::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());
  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;

  if (in_menu)
  {
    menuAction(menu->menuClick(x_pos, y_pos));
  }
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::update(const ASGE::GameTime& game_time)
{
  float dt_sec = game_time.delta.count() / 1000.0f;

  //@feedback, if you used a pointer for current scene, this could be simplified
  // make sure you use delta time in any movement calculations!
  if (in_classic)
  {
    classic_mode->update(dt_sec);
    switch (classic_mode->exitingScene())
    {
      case 0:
        resetClassic();
        in_classic = false;
        in_menu = true;
        break;
      case 1:
        resetClassic();
        in_classic = false;
        break;
      case 2:
        resetClassic();
        in_classic = false;
        break;
      default:
        break;
    }
  }
  if (in_arcade)
  {
    arcade_mode->update(dt_sec);
    switch (arcade_mode->exitingScene())
    {
      case 0:
        resetArcade();
        in_arcade = false;
        in_menu = true;
        break;
      case 1:
        resetArcade();
        in_arcade = false;
        break;
      case 2:
        resetArcade();
        in_arcade = false;
        break;
      default:
        break;
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::render(const ASGE::GameTime& game_time)
{
  renderer->setFont(0);
  if (in_menu)
  {
    menu->render(renderer.get());
  }
  if (in_classic)
  {
    classic_mode->render(renderer.get());
  }
  if (in_arcade)
  {
    arcade_mode->render(renderer.get());
  }
  if (in_config)
  {
    arcade_config->render(renderer.get());
  }
}

void Pong::mouseMoveHandler(ASGE::SharedEventData data)
{
  auto move = static_cast<const ASGE::MoveEvent*>(data.get());
  if (in_menu)
  {
    menu->update(move->xpos, move->ypos);
  }
}

void Pong::resetClassic()
{
  delete classic_mode;
  classic_mode = new GameScene;
  classic_mode->initScene(renderer.get(),
                          WINDOWX,
                          WINDOWY,
                          CLASSIC_PADDLE_COUNT,
                          false,
                          true,
                          CLASSIC_SCORE_REQUIREMENT,
                          START_SPEED,
                          CLASSIC_SLIDE,
                          ASGE::COLOURS::WHITE,
                          ASGE::COLOURS::WHITE);
}

void Pong::resetArcade()
{
  delete arcade_mode;
  arcade_mode = new GameScene;
}
