//
// Created by c2-horosnyi on 29/11/2019.
//

#include "gamescene.h"

namespace
{
enum
{
  NORMALBALL = 0,
  W_KEY = 0,
  S_KEY = 1,
  A_KEY = 2,
  D_KEY = 3,
  UP_KEY = 4,
  DOWN_KEY = 5,
  LEFT_KEY = 6,
  RIGHT_KEY = 7,
  SPACE_KEY = 8,
  TAB_KEY = 9,
  PERIOD_KEY = 10,
  COMMA_KEY = 11,
  KEY_COUNT = 12,
  SEGMENTATION = 6
};
}

GameScene::~GameScene()
{
  delete[] the_balls;
  delete[] players;
  delete[] vectors;
}

bool GameScene::initScene(ASGE::Renderer* renderer,
                          int resolution_w,
                          int resolution_h,
                          int paddle_count,
                          bool quickstart,
                          bool classic_XY_movement,
                          int playing_to,
                          int starting_speed,
                          int slidiness,
                          ASGE::Colour Player1_Colour,
                          ASGE::Colour Player2_Colour)
{
  current_game_speed = starting_speed;
  score_limit = playing_to;
  the_balls = new Ball[TEMP_MAX_BALLS];
  players = new Paddle[paddle_count];
  vectors = new pmvector;

  // helping data
  helper.paddle_thickness_x = resolution_w / 120.0f;
  helper.paddle_thickness_y = resolution_h / 36.0f;
  helper.paddle_length_x = resolution_h / 6.0f;
  helper.paddle_length_y = resolution_w / 6.0f;
  helper.paddle_gap_x = resolution_w / 180.0f;
  helper.paddle_gap_y = resolution_h / 48.0f;
  helper.screen_mid_x = resolution_h / 2.0f;
  helper.screen_mid_y = resolution_w / 2.0f;
  helper.screen_quad_y = resolution_w / 4.0f;
  helper.collision_border_x = helper.paddle_thickness_x + helper.paddle_gap_x;
  helper.collision_border_y = helper.paddle_thickness_y + helper.paddle_gap_y;
  helper.ball_size = resolution_h / 80;
  helper.remember_quickstart = quickstart;
  helper.classic_movement_scheme = classic_XY_movement;
  helper.initial_speed = starting_speed;
  helper.max_speed = starting_speed * 3.0;
  player_colour[1] = Player1_Colour;
  player_colour[0] = Player2_Colour;

  slide = slidiness;
  map_size_x = resolution_w;
  map_size_y = resolution_h;

  // Create Balls
  resetBalls(quickstart, classic_XY_movement);
  for (int i = 0; i < TEMP_MAX_BALLS; i++)
  {
    the_balls[i].loadPicture(renderer);
  }

  // limit paddle movement
  limits1[1] = resolution_h - helper.paddle_length_x;
  limits2[1] = helper.screen_mid_y - helper.paddle_length_y;
  limits3[0] = helper.screen_mid_y;
  limits3[1] = resolution_w - helper.paddle_length_y;

  // Create Paddles
  resetPaddles(resolution_w, resolution_h, paddle_count, starting_speed);
  for (int i = 0; i < paddle_count; i++)
  {
    players[i].loadPicture(renderer, i % 2 ? Player1_Colour : Player2_Colour);
  }
}

void GameScene::resetPaddles(int resolution_w,
                             int resolution_h,
                             int paddle_count,
                             int starting_speed)
{
  players[0].init(helper.paddle_thickness_x,
                  helper.paddle_length_x,
                  helper.paddle_gap_x,
                  helper.screen_mid_x - (helper.paddle_length_x / 2),
                  limits1,
                  starting_speed * helper.paddle_to_ball_speed);
  players[0].defineAxis(false);
  players[1].init(helper.paddle_thickness_x,
                  helper.paddle_length_x,
                  resolution_w - helper.collision_border_x,
                  helper.screen_mid_x - (helper.paddle_length_x / 2),
                  limits1,
                  starting_speed * helper.paddle_to_ball_speed);
  players[1].defineAxis(false);
  nr_of_paddles = paddle_count;
  if (paddle_count >= 3)
  {
    players[2].init(helper.paddle_length_y,
                    helper.paddle_thickness_y,
                    helper.screen_quad_y - (helper.paddle_length_y / 2),
                    resolution_h - helper.collision_border_y,
                    limits2,
                    starting_speed * helper.paddle_to_ball_speed);
    players[2].defineAxis(true);
  }
  if (paddle_count >= 4)
  {
    players[3].init(resolution_w / 6,
                    resolution_h / 36,
                    (helper.screen_quad_y * 3) - (helper.paddle_length_y / 2),
                    resolution_h - helper.collision_border_y,
                    limits3,
                    starting_speed * helper.paddle_to_ball_speed);
    players[3].defineAxis(true);
  }
}

void GameScene::resetBalls(bool quickstart, bool classic_XY_movement)
{
  for (int i = 0; i < TEMP_MAX_BALLS; i++)
  {
    the_balls[i].init(0.0f,
                      90.0f,
                      NORMALBALL,
                      NORMALBALL,
                      helper.ball_size,
                      (helper.screen_mid_y) - (helper.ball_size / 2.0),
                      (helper.screen_mid_x) - (helper.ball_size / 2.0),
                      classic_XY_movement);
  }
  current_ball_count = 1;
  if (quickstart)
  {
    current_ball_count++;
    the_balls[1].changeSpeed(0, -200);
  }
}

int GameScene::keyProcessing(const ASGE::KeyEvent* key)
{
  switch (key->key)
  {
    case ASGE::KEYS::KEY_W:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[W_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[W_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_S:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[S_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[S_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_A:
    {
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[A_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[A_KEY] = false;
      }
    }
    break;
    case ASGE::KEYS::KEY_D:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[D_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[D_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_UP:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[UP_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[UP_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_DOWN:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[DOWN_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[DOWN_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_LEFT:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[LEFT_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[LEFT_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_RIGHT:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[RIGHT_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[RIGHT_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_SPACE:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[SPACE_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[SPACE_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_TAB:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[TAB_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[TAB_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_PERIOD:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[PERIOD_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[PERIOD_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_COMMA:
      if (key->action == ASGE::KEYS::KEY_PRESSED)
      {
        keys_held[COMMA_KEY] = true;
      }
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        keys_held[COMMA_KEY] = false;
      }
      break;
    case ASGE::KEYS::KEY_X:
      exitstatus = 0;
  }
}

void GameScene::update(float delta)
{
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (keys_held[i])
    {
      inputProcessing(i, delta, slide);
    }
  }
  for (int i = 0; i < nr_of_paddles; i++)
  {
    players[i].updatePosition(delta, slide);
  }
  for (int i = 0; i < current_ball_count; i++)
  {
    ball_ghosts[i * 2] = the_balls[i].reportX();
    ball_ghosts[i * 2 + 1] = the_balls[i].reportY();
    the_balls[i].updatePosition(delta);
    float ball_x = the_balls[i].reportX();
    float ball_y = the_balls[i].reportY();
    float temp_ball_size = the_balls[i].reportSize();
    // COLLISION
    if (the_balls[i].reportAngle() == 90.0f)
    {
      safetyCheck(i, ball_x, ball_y, temp_ball_size);
    }
    else
    {
      smartCheck(i, ball_x, ball_y, temp_ball_size);
      // verticalWallCheck(i, ball_x, ball_y, temp_ball_size);
    }
    // SCORING
    if (ball_x <= -the_balls[i].reportSize())
    {
      increaseScore(1);
      checkScores();
    }
    if (ball_x >= map_size_x)
    {
      increaseScore(0);
      checkScores();
    }
  }
}

void GameScene::increaseScore(int scoring_player)
{
  score[scoring_player]++;
  resetBalls(helper.remember_quickstart, helper.classic_movement_scheme);
  for (int i = 0; i < current_ball_count; i++)
  {
    the_balls[i].changeColour(ASGE::COLOURS::WHITE);
  }
  resetPaddles(map_size_x, map_size_y, nr_of_paddles, helper.initial_speed);
  resetSpeed();
}

void GameScene::smartCheck(int i,
                           float ball_x,
                           float ball_y,
                           float temp_ball_size)
{
  bool corner_swaps = false;
  if (!the_balls[i].reportHeading())
  {
    corner_swaps = !corner_swaps;
  }
  if (the_balls[i].reportAngle() < 90.0f)
  {
    corner_swaps = !corner_swaps;
  }
  if (corner_swaps)
  {
    ball_ghosts[i * 2] += temp_ball_size;
  }
  checkCollision(i, corner_swaps ? ball_x + temp_ball_size : ball_x, ball_y);
  if (!corner_swaps)
  {
    ball_ghosts[i * 2] += temp_ball_size;
    ball_ghosts[i * 2 + 1] += temp_ball_size;
  }
  else
  {
    ball_ghosts[i * 2] -= temp_ball_size;
    ball_ghosts[i * 2 + 1] += temp_ball_size;
  }
  checkCollision(i,
                 corner_swaps ? ball_x : ball_x + temp_ball_size,
                 ball_y + temp_ball_size);
}

void GameScene::safetyCheck(int i,
                            float ball_x,
                            float ball_y,
                            float temp_ball_size)
{
  checkCollision(i, ball_x, ball_y);
  ball_ghosts[i * 2 + 1] += temp_ball_size;
  checkCollision(i, ball_x, ball_y + temp_ball_size);
  ball_ghosts[i * 2 + 1] -= temp_ball_size;
  ball_ghosts[i * 2] += temp_ball_size;
  checkCollision(i, ball_x + temp_ball_size, ball_y);
  ball_ghosts[i * 2 + 1] += temp_ball_size;
  checkCollision(i, ball_x + temp_ball_size, ball_y + temp_ball_size);
}

void GameScene::checkCollision(int i, float ball_x, float ball_y)
{
  float weight_x = the_balls[i].reportX() + (the_balls[i].reportSize() / 2);
  float weight_y = the_balls[i].reportY() + (the_balls[i].reportSize() / 2);
  if (ball_x <= helper.collision_border_x ||
      ball_x >= map_size_x - helper.collision_border_x - helper.ball_size)
  {
    vectors->setvalue(
      ball_x, ball_y, ball_ghosts[i * 2], ball_ghosts[i * 2 + 1]);
    collisionHandlerHorizontal(i, weight_x, weight_y);
  }
  if (ball_y < 0 ||
      (nr_of_paddles > 2 && ball_y >= helper.collision_border_y) ||
      ball_y >= map_size_y)
  {
    vectors->setvalue(
      ball_x, ball_y, ball_ghosts[i * 2], ball_ghosts[i * 2 + 1]);
    collisionHandlerVertical(i, weight_x, weight_y);
  }
}

void GameScene::collisionHandlerVertical(int ball_id,
                                         float ball_x,
                                         float ball_y)
{
  /*float collision_results =
          vectors->collision(false,
                             players[0].reportY(),
                             players[0].reportY() +
  players[0].reportSize(false), players[0].reportX() +
  players[0].reportSize(true)); if (collision_results != -1 &&
  !the_balls[i].reportHeading())
  {
      the_balls[i].changeSpeed(0, -200);
      ASGE::DebugPrinter{} << "PLAYER 1 HIT AT: " << std::endl;
      ASGE::DebugPrinter{} << "x_pos: " << ball_x << std::endl;
      ASGE::DebugPrinter{} << "y_pos: " << ball_y << std::endl;
      the_balls[i].changeColour(player_colour[0]);
      collisionHandling(i, 0, ball_x, ball_y);
  }*/
  float temp_angle = the_balls[ball_id].reportAngle();
  float collision_results = 0.0f;
  if (ball_y < map_size_y / 2)
  {
    collision_results = vectors->collision(true, 0, map_size_x, 0);
    if (collision_results != -1 && temp_angle < 90.0f)
    {
      the_balls[ball_id].setAngle(180 - temp_angle);
    }
  }
  if (ball_y > map_size_y / 2)
  {
    switch (nr_of_paddles)
    {
      case 2:
        collision_results = vectors->collision(true, 0, map_size_x, map_size_y);
        if (collision_results != -1 && temp_angle > 90.0f)
        {
          the_balls[ball_id].setAngle(180 - temp_angle);
        }
        break;
      case 3:
        break;
      case 4:
        break;
      default:
        break;
    }
  }
  ASGE::DebugPrinter{} << the_balls[ball_id].reportAngle() << std::endl;
}

void GameScene::collisionHandlerHorizontal(int i, float ball_x, float ball_y)
{
  float collision_results =
    vectors->collision(false,
                       players[0].reportY(),
                       players[0].reportY() + players[0].reportSize(false),
                       players[0].reportX() + players[0].reportSize(true));
  if (collision_results != -1 && !the_balls[i].reportHeading())
  {
    the_balls[i].changeSpeed(0, -200);
    ASGE::DebugPrinter{} << "PLAYER 1 HIT AT: " << std::endl;
    ASGE::DebugPrinter{} << "x_pos: " << ball_x << std::endl;
    ASGE::DebugPrinter{} << "y_pos: " << ball_y << std::endl;
    the_balls[i].changeColour(player_colour[0]);
    collisionHandling(i, 0, ball_x, ball_y);
    increaseSpeed(helper.initial_speed / 10.0f);
  }
  collision_results =
    vectors->collision(false,
                       players[0].reportY(),
                       players[0].reportY() + players[0].reportSize(false),
                       players[0].reportX());
  if (collision_results != -1 && !the_balls[i].reportHeading())
  {
    the_balls[i].changeSpeed(0, -200);
    ASGE::DebugPrinter{} << "PLAYER 1 HIT AT: " << std::endl;
    ASGE::DebugPrinter{} << "x_pos: " << ball_x << std::endl;
    ASGE::DebugPrinter{} << "y_pos: " << ball_y << std::endl;
    the_balls[i].changeColour(player_colour[0]);
    collisionHandling(i, 0, ball_x, ball_y);
    increaseSpeed(helper.initial_speed / 10.0f);
  }
  collision_results =
    vectors->collision(false,
                       players[1].reportY(),
                       players[1].reportY() + players[1].reportSize(false),
                       players[1].reportX() + players[1].reportSize(true));
  if (collision_results != -1 && the_balls[i].reportHeading())
  {
    the_balls[i].changeSpeed(0, -200);
    ASGE::DebugPrinter{} << "PLAYER 2 HIT AT: " << std::endl;
    ASGE::DebugPrinter{} << "x_pos: " << ball_x << std::endl;
    ASGE::DebugPrinter{} << "y_pos: " << ball_y << std::endl;
    the_balls[i].changeColour(player_colour[1]);
    collisionHandling(i, 1, ball_x, ball_y);
    increaseSpeed(helper.initial_speed / 10.0f);
  }

  collision_results =
    vectors->collision(false,
                       players[1].reportY(),
                       players[1].reportY() + players[1].reportSize(false),
                       players[1].reportX());
  if (collision_results != -1 && the_balls[i].reportHeading())
  {
    the_balls[i].changeSpeed(0, -200);
    ASGE::DebugPrinter{} << "PLAYER 2 HIT AT: " << std::endl;
    ASGE::DebugPrinter{} << "x_pos: " << ball_x << std::endl;
    ASGE::DebugPrinter{} << "y_pos: " << ball_y << std::endl;
    the_balls[i].changeColour(player_colour[1]);
    collisionHandling(i, 1, ball_x, ball_y);
    increaseSpeed(helper.initial_speed / 10.0f);
  }
}

void GameScene::inputProcessing(int input_key,
                                float delta_time,
                                int slide_slider)
{
  // TODO:
  // add strikes
  switch (input_key)
  {
    case W_KEY:
      players[0].incrementSpeed(slide_slider, delta_time);
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case S_KEY:
      players[0].incrementSpeed(-slide_slider, delta_time);
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case A_KEY:
      if (nr_of_paddles >= 3)
      {
        players[2].incrementSpeed(-slide_slider, delta_time);
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case D_KEY:
      if (nr_of_paddles >= 3)
      {
        players[2].incrementSpeed(slide_slider, delta_time);
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case UP_KEY:
      players[1].incrementSpeed(slide_slider, delta_time);
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    case DOWN_KEY:
      players[1].incrementSpeed(-slide_slider, delta_time);
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    case LEFT_KEY:
      if (nr_of_paddles >= 4)
      {
        players[3].incrementSpeed(-slide_slider, delta_time);
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    case RIGHT_KEY:
      if (nr_of_paddles >= 4)
      {
        players[3].incrementSpeed(slide_slider, delta_time);
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    case SPACE_KEY:
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case TAB_KEY:
      if (nr_of_paddles >= 3)
      {
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(false);
      }
      break;
    case PERIOD_KEY:
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    case COMMA_KEY:
      if (nr_of_paddles >= 4)
      {
      }
      if (the_balls[0].reportSpeed() == 0.0f)
      {
        startBall(true);
      }
      break;
    default:
      break;
  }
}

void GameScene::render(ASGE::Renderer* renderer)
{
  // background?
  renderer->setFont(0);
  renderer->renderText(std::to_string(score[0]),
                       (map_size_x / 2) - (map_size_x / 8),
                       (map_size_y / 2) + 7,
                       1.0f,
                       ASGE::COLOURS::DARKGRAY);
  renderer->renderText(std::to_string(score[1]),
                       (map_size_x / 2) + (map_size_x / 8),
                       (map_size_y / 2) + 7,
                       1.0f,
                       ASGE::COLOURS::DARKGRAY);
  for (int i = 0; i < nr_of_paddles; i++)
  {
    players[i].render(renderer);
  }
  for (int i = 0; i < current_ball_count; i++)
  {
    the_balls[i].render(renderer);
  }
}

void GameScene::startBall(bool positive_direction)
{
  for (int i = 0; i < current_ball_count; i++)
  {
    the_balls[i].changeSpeed(
      positive_direction ? helper.initial_speed : -helper.initial_speed, 0);
  }
}

GameScene::GameScene()
{
  player_colour[0] = ASGE::COLOURS::WHITE;
  player_colour[1] = ASGE::COLOURS::WHITE;
}

void GameScene::collisionHandling(int ball_id,
                                  int paddle_id,
                                  float impact_x,
                                  float impact_y)
{
  bool event_fired = false;
  float paddle_start = 0.0f;
  float paddle_end = 0.0f;
  float impact = 0.0f;
  if (paddle_id >= 2)
  {
    paddle_start = players[paddle_id].reportX();
    paddle_end = paddle_start + players[paddle_id].reportSize(true);
    impact = impact_x;
  }
  else
  {
    paddle_start = players[paddle_id].reportY();
    paddle_end = paddle_start + players[paddle_id].reportSize(false);
    impact = impact_y;
  }

  paddle_end -= paddle_start;
  impact -= paddle_start;

  float ratio = impact / paddle_end;
  float segmentsize = 1.0f / segmentation;
  for (int i = 0; i < segmentation; i++)
  {
    if (ratio >= i * segmentsize && ratio <= segmentsize * (i + 1))
    {
      segmentAngleChanger(ball_id,
                          i,
                          20,
                          helper.classic_movement_scheme,
                          players[paddle_id].reportSpeed());
      event_fired = true;
    }
  }

  if (!event_fired)
  {
    if (ratio < 0)
    {
      segmentAngleChanger(ball_id,
                          0,
                          20,
                          helper.classic_movement_scheme,
                          players[paddle_id].reportSpeed());
    }
    if (ratio > 0)
    {
      segmentAngleChanger(ball_id,
                          segmentation,
                          20,
                          helper.classic_movement_scheme,
                          players[paddle_id].reportSpeed());
    }
  }
}

void GameScene::segmentAngleChanger(
  int ball_id, int hit, float min_angle, bool classic_mode, float playerspeed)
{
  int midsegment = segmentation / 2;
  int distance = 0;
  float angle_per_segment = 0.0f;
  if (classic_mode)
  {
    if (segmentation % 2 == 0)
    {
      midsegment--;
      angle_per_segment = (90.0f - min_angle) / midsegment;
      for (int i = 0; i < segmentation; i++)
      {
        if (hit == i)
        {
          distance = i - midsegment;
          if (abs(distance) > abs(midsegment + 1 - i))
          {
            distance = i - midsegment - 1;
          }
        }
      }
    }
    else
    {
      angle_per_segment = (90.0f - min_angle) / midsegment;
      for (int i = 0; i < segmentation; i++)
      {
        if (hit == i)
        {
          distance = i - midsegment;
        }
      }
    }
    the_balls[ball_id].setAngle(90.0f + distance * angle_per_segment);
  }
  else
  {
    if (segmentation % 2 == 0)
    {
      midsegment--;
      angle_per_segment = (90.0f - min_angle) / midsegment;
      angle_per_segment /= 2.5f;
      for (int i = 0; i < segmentation; i++)
      {
        if (hit == i)
        {
          distance = i - midsegment;
          if (abs(distance) > abs(midsegment + 1 - i))
          {
            distance = i - midsegment - 1;
          }
        }
      }
    }
    else
    {
      angle_per_segment = (90.0f - min_angle) / midsegment;
      angle_per_segment /= 2.5f;
      for (int i = 0; i < segmentation; i++)
      {
        if (hit == i)
        {
          distance = i - midsegment;
        }
      }
    }
    the_balls[ball_id].changeAngle(
      (distance * angle_per_segment) +
      (playerspeed * 100 / 70 / current_game_speed));
    if (the_balls[ball_id].reportAngle() > 180.0f - min_angle)
    {
      the_balls[ball_id].setAngle(180.0f - min_angle);
    }
    if (the_balls[ball_id].reportAngle() < min_angle)
    {
      the_balls[ball_id].setAngle(min_angle);
    }
  }
}

int GameScene::exitingScene()
{
  return exitstatus;
}

int GameScene::checkScores()
{
  if (score[0] >= score_limit)
  {
    return 0;
  }
  if (score[1] >= score_limit)
  {
    return 1;
  }
  return -1;
}

void GameScene::increaseSpeed(float increase_by)
{
  if (current_game_speed < helper.max_speed)
  {
    current_game_speed += increase_by;
    for (int i = 0; i < nr_of_paddles; i++)
    {
      players[i].changeTopSpeed(increase_by * helper.paddle_to_ball_speed, 0);
    }
    for (int i = 0; i < current_ball_count; i++)
    {
      if (the_balls[i].reportHeading())
      {
        the_balls[i].changeSpeed(increase_by * 1.2, 0);
      }
      else
      {
        the_balls[i].changeSpeed(increase_by * -1.2, 0);
      }
    }
  }
}

void GameScene::resetSpeed()
{
  current_game_speed = helper.initial_speed;
  for (int i = 0; i < nr_of_paddles; i++)
  {
    players[i].changeTopSpeed(0, -100);
    players[i].changeTopSpeed(
      helper.initial_speed * helper.paddle_to_ball_speed, 0);
  }
  for (int i = 0; i < current_ball_count; i++)
  {
    the_balls[i].setSpeed(helper.initial_speed);
  }
}
