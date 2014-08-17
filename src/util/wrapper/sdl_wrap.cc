/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include <SDL_ttf.h>

int RenderTTFText(TTF_Font *Font, const SDL_Color& Color,
                  const glm::vec2 &Position,  const char *Text)
{
  /*Create some variables.*/
  SDL_Surface *Message = TTF_RenderText_Blended(Font, Text, Color);
  if (Message == nullptr) {
    return -1;
  }

  /*Generate an OpenGL 2D texture from the SDL_Surface*.*/
  unsigned Texture = 0;
  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, Message->pixels);

  /*Draw this texture on a quad with the given xyz coordinates.*/
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2fv(glm::value_ptr(Position));
  glTexCoord2f(1.0f, 0.0f);
  glVertex2fv(glm::value_ptr(Position + glm::vec2(Message->w, 0.0f)));
  glTexCoord2f(1.0f, 1.0f);
  glVertex2fv(glm::value_ptr(Position + glm::vec2(Message->w, Message->h)));
  glTexCoord2f(0.0f, 1.0f);
  glVertex2fv(glm::value_ptr(Position + glm::vec2(0.0f, Message->h)));
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  /*Clean up.*/
  glDeleteTextures(1, &Texture);
  SDL_FreeSurface(Message);

  return 0;
}
