/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#ifndef MALLGAME_H_
#define MALLGAME_H_

#include <vector>
#include <SDL_ttf.h>

class PlanktonGameSceneInterface {
 public:
  PlanktonGameSceneInterface() {}
  virtual ~PlanktonGameSceneInterface() {}

  virtual int Initialize(const glm::vec2 &window_size) = 0;
  virtual void Finalize() = 0;
  virtual void Update(float elapsed_time, const glm::vec2 &window_size) = 0;
  virtual void Draw(const glm::vec2 &window_size) = 0;
  virtual int OnMouseButtonDown(unsigned char button, const glm::vec2 &cursor_pos) = 0;
};

class PlanktonGame {
 public:
  PlanktonGame();
  ~PlanktonGame();

  int Initialize();
  void Finalize();
  void Update(float elapsed_time, const glm::vec2 &window_size);
  void Draw(const glm::vec2 &window_size);
  int OnKeyboardDown(SDL_Keycode key, const glm::vec2 &window_size);
  int OnMouseButtonDown(unsigned char button, int x, int y, const glm::vec2 &window_size);

 private:
  std::vector<PlanktonGameSceneInterface *> scenes_;
  PlanktonGameSceneInterface *active_scene_;
  TTF_Font *font_;
};

#endif /* MALLGAME_H_ */
