#ifndef HUD__H
#define HUD__H
#include "SDL.h"
#include "gameData.h"
#include "clock.h"
#include "drawable.h"
#include "ioMod.h"
#include "drawable.h"
#include <vector>

class Hud : public Drawable {
public:
  Hud(const std::string&);
  void draw() const;
  void update(Uint32);

  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;

  virtual int getScaledWidth() const  {
    return barRect.w;
  }
  virtual int getScaledHeight() const
  {
    return barRect.h;
  }
  virtual const Image* getImage() const
  {
    throw std::string("No image in HUD");
  }

  virtual const SDL_Surface* getSurface() const{
    return NULL;
  }

  void drawHeart(int , int ) const;
private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  Clock& clock;
  int currentLength;
  int increment;
  float interval; // elapsed time for each decrement (1000ms=1sec)
  int deltaTime;

  IoMod& io;
};
#endif
