/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "PlanktonMain.h"

#include <algorithm>
#include <sstream>
#include <mmsystem.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>

#include "plankton/PlanktonGame.h"
#include "plankton/PlanktonTweakerContext.h"
#include "util/logging/Logger.h"
#include "util/measurement/FPSCounter.h"
#include "util/wrapper/glgraphics_wrap.h"
#include "util/macro_util.h"

static const std::string kWindowCaption = "Plankton - The Motel on Jupiter";
static const Uint32 kWindowWidth = 800;
static const Uint32 kWindowHeight = 600;
static const Uint32 kIdealFrameRate = 60;

static const glm::vec2 kWindowSize = glm::vec2(kWindowWidth, kWindowHeight);
static const int kGameLoopInterval = 1000 / kIdealFrameRate;
static const float kGameLoopIntervalSec = 1.0f / kIdealFrameRate;
static const int kFPSCountSamplingTime = 1000;

static SDL_Window *window = nullptr;
static SDL_GLContext context = nullptr;
static TwBar *tw_bar = nullptr;
static PlanktonGame game;
static FPSCounter fps_counter(kFPSCountSamplingTime);

static void PlanktonCleanUp();

int PlanktonMain(int argc, char *argv[], const char *config_path) {
  UNUSED(argc);
  UNUSED(argv);
  UNUSED(config_path);

  LOGGER.Info("Set up the application");

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOGGER.Error("Failed to initialize SDL video system (errmsg: %s)", SDL_GetError());
    return -1;
  }

  // Initialize TTF font drawing library
  if (TTF_Init() != 0) {
    LOGGER.Error("Failed to initialize SDL_ttf (errmsg: %s)", TTF_GetError());
    PlanktonCleanUp();
    return -1;
  }

  // Enable double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Create the window
  window = SDL_CreateWindow(kWindowCaption.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, kWindowWidth, kWindowHeight,
                            SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    LOGGER.Error("Failed to create SDL window (errmsg: %s)", SDL_GetError());
    PlanktonCleanUp();
    return -1;
  }

  // Create OpenGL context
  context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    LOGGER.Error("Failed to create SDL context for OpenGL (errmsg: %s)", SDL_GetError());
    PlanktonCleanUp();
    return -1;
  }

  // Initialize the tweaker library
  if (TwInit(TW_OPENGL, NULL) == 0) {
    LOGGER.Error("Failed to initialize the tweaker library (errmsg: %s)", TwGetLastError());
    PlanktonCleanUp();
    return -1;
  }
  if (TwWindowSize(kWindowWidth, kWindowHeight) == 0) {
    LOGGER.Error("Failed to set the window size to tweaker (errmsg: %s)", TwGetLastError());
    PlanktonCleanUp();
    return -1;
  }
  tw_bar = TwNewBar("TweakMenu");
  std::stringstream tw_def;
  tw_def << "TweakMenu position='" << 550 << " " << 10 <<
      "' size='" << 240 << " " << 580 << "' color='41 126 231' iconified=true";
  TwDefine(tw_def.str().c_str());
  if (TwAddVarRO(tw_bar, "SYSTEM_ACTUAL_FRAME_RATE", TW_TYPE_INT8,
                 &(tweaker_ctx.system_actual_fps),
                 "group='System' label='Actual Frame Rate'") == 0) {
    LOGGER.Warn("Failed to add a tweak variable for actual-FPS (errmsg: %s)", TwGetLastError());
  }

  // Initialize the game
  int ret = game.Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the game objects (ret: %d)");
    PlanktonCleanUp();
    return -1;
  }

  // Set the minimum timer resolution
  timeBeginPeriod(1);

  // Execute the mainloop
  int loop_stat = 0;
  while (true) {
    int start_tick = SDL_GetTicks();

    // check event
    bool escape_loop = false;
    SDL_Event event;
    while (SDL_PollEvent(&event) == 1) {
      if (TwEventSDL20(&event) != 0) {
        continue;
      }
      switch (event.type) {
        case SDL_QUIT:
          escape_loop = true;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            escape_loop = true;
          } else {
            game.OnKeyboardDown(event.key.keysym.sym, kWindowSize);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          game.OnMouseButtonDown(event.button.button, event.button.x, event.button.y,
                                 kWindowSize);
          break;
      }
    }
    if (escape_loop) {
      break;
    }

    // Update the game
    game.Update(kGameLoopIntervalSec, kWindowSize);

    // Draw the objects
    game.Draw(kWindowSize);
    if (TwDraw() == 0) {
      LOGGER.Error("Failed to draw the tweaker (errmsg: %s)", TwGetLastError());
      loop_stat = -1;
      break;
    }
    SDL_GL_SwapWindow(window);

    int finish_tick = SDL_GetTicks();
    int exec_tick = finish_tick - start_tick;
    Sleep(std::max<int>(kGameLoopInterval - exec_tick, 1));

    fps_counter.Update(SDL_GetTicks());
    tweaker_ctx.system_actual_fps = fps_counter.fps();
  }

  // Reset the minimum timer resolution
  timeEndPeriod(1);

  PlanktonCleanUp();
  return loop_stat;
}

static void PlanktonCleanUp() {
  LOGGER.Info("Clean up the application");

  game.Finalize();
  if (TwRemoveAllVars(tw_bar) == 0) {
    LOGGER.Warn("Failed to remove all tweaker varibables (errmsg: %s)",
                TwGetLastError());
  }
  if (TwTerminate() == 0) {
    // Ignore the error to terminate the tweaker
    // because the tweaker library may be not initialized
  }
  if (context != nullptr) {
    SDL_GL_DeleteContext(context);
  }
  if (window != nullptr) {
    SDL_DestroyWindow(window);
  }
  TTF_Quit();
  SDL_Quit();
}
