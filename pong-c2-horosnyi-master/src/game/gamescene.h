//
// Created by c2-horosnyi on 29/11/2019.
//

#ifndef PONG_C2_HOROSNYI_GAMESCENE_H
#define PONG_C2_HOROSNYI_GAMESCENE_H
#include <Engine/Colours.h>
#include <Engine/DebugPrinter.h>
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include "../math/pmvector.h"
#include "ball.h"
#include "paddle.h"

enum
{
  TEMP_MAX_BALLS = 5
};

class GameScene
{
 public:
  GameScene();
  ~GameScene();
  int keyProcessing(const ASGE::KeyEvent* key);
  bool initScene(ASGE::Renderer* renderer,
                 int resolution_w,
                 int resolution_h,
                 int paddle_count,
                 bool quick_start,
                 bool classic_XY_movement,
                 int playing_to,
                 int starting_speed,
                 int slidiness,
                 ASGE::Colour Player1,
                 ASGE::Colour Player2);
  void update(float delta);
  void render(ASGE::Renderer* renderer);
  bool
  loadPicture(ASGE::Renderer* renderer, int width, int height); // if
                                                                // background
                                                                // is needed
  void inputProcessing(int input_key, float delta_time, int slide_slider);
  void setStrikes(bool allow);
  void resetBalls(bool quickstart, bool classic_XY_movement);
  void resetPaddles(int resolution_w,
                    int resolution_h,
                    int paddle_count,
                    int starting_speed);
  int exitingScene();

 private:
  struct CoordinateData
  {
    float collision_border_x = 0.0f; // how far for collision to be possible
    float collision_border_y = 0.0f;
    float paddle_thickness_x = 0.0f;
    float paddle_thickness_y = 0.0f;
    float paddle_length_x = 0.0f;
    float paddle_length_y = 0.0f;
    float paddle_gap_x = 0.0f;
    float paddle_gap_y = 0.0f;
    float ball_size = 0.0f;
    float screen_mid_x = 0.0f;
    float screen_mid_y = 0.0f;
    float screen_quad_y = 0.0f;
    float paddle_to_ball_speed = 0.7f;
    bool remember_quickstart = false;
    bool classic_movement_scheme = true;
    float initial_speed = 0.0f;
    float max_speed = 0.0f;
  };

 private:
  Paddle* players = nullptr;
  Ball* the_balls = nullptr;
  pmvector* vectors = nullptr;
  ASGE::Colour player_colour[2];
  CoordinateData helper;
  float ball_ghosts[TEMP_MAX_BALLS * 2] = { 0 };
  int current_ball_count = 1;
  int nr_of_paddles = 0;
  int exitstatus = -1;
  bool allow_stikes = false;
  int score[2] = { 0 };
  int score_limit = 0;
  bool keys_held[12] = { false };
  float current_game_speed = 0;
  int slide = 20;
  int map_size_x = 0;
  int map_size_y = 0;
  int segmentation = 6;
  float limits1[2] = { 0.0f };
  float limits2[2] = { 0.0f };
  float limits3[2] = { 0.0f };

  //@feedback, I need to review clang format for this... not your fault
  void collisionHandlerHorizontal(int i, float ball_x, float ball_y);
  void collisionHandlerVertical(int i, float ball_x, float ball_y);
  void
  collisionHandling(int ball_id, int paddle_id, float impact_x, float impact_y);

  void segmentAngleChanger(
    int ball_id, int hit, float min_angle, bool classic_mode, float sway_speed);

  void startBall(bool positive_direction);

  void checkCollision(int ball_id, float ball_x, float ball_y);

  void
  safetyCheck(int ball_id, float ball_x, float ball_y, float temp_ball_size);

  void
  smartCheck(int ball_id, float ball_x, float ball_y, float temp_ball_size);

  void increaseScore(int scoring_player);
  int checkScores();
  void increaseSpeed(float increase_by);
  void resetSpeed();
};

#endif // PONG_C2_HOROSNYI_GAMESCENE_H
