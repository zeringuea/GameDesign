#include <sstream>
#include "hud.h"
#include "renderContext.h"
#include "sprite.h"

Hud::Hud(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
                    Gamedata::getInstance().getXmlFloat(name+"/scale")
           ),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/width"),
             Gamedata::getInstance().getXmlInt(name+"/height")}
  ),
  clock( Clock::getInstance() ),
  currentLength(barRect.w),
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/increment")),
  interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  deltaTime(0),
  io( IoMod::getInstance() )
{
}

void Hud::update(Uint32 ticks) {

  /*SDL_Rect rect = {0,450,900,100};
  SDL_SetRenderDrawColor(renderer, 44, 47, 56,255);
  SDL_RenderFillRect(renderer, &rect);

  //Displays controls and my name
  io.writeText("Name: Andrew Zeringue",0, 450);
  io.writeText("Movement: WASD", 350, 450);
*/
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= increment;
  }
}

void Hud::drawHeart(int x, int y) const {
  SDL_Surface* surface = SDL_LoadBMP("assets/heart.bmp");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void Hud::draw() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!

  //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  drawHeart(40, 30);
  drawHeart(70, 30);
  drawHeart(100, 30);

  //Sets a Grey rectangle for background
  SDL_Rect rect = {650,30,200,200};
  SDL_SetRenderDrawColor(renderer, 44, 47, 56,255);
  SDL_RenderFillRect(renderer, &rect);

  //Displays controls and my name
  io.writeText("Name: Andrew Zeringue",670, 40);
  io.writeText("Movement: WASD", 670, 60);
  io.writeText("Shoot: Spacebar", 670, 80);
  io.writeText("God Mode: G", 670, 160);
  io.writeText("Restart: R", 670, 180);
  io.writeText("Goal: Survive", 670, 200);

  //Find assets of hearts to use for health

  // Set the health bar background color:
  /*SDL_SetRenderDrawColor(
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor(
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = static_cast<int>(getX());
  int y = static_cast<int>(getY());
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );

  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor(
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );
  */
}
