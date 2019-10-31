#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <SDL.h>
#include "sprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "sound.h"




Engine::~Engine() {
  auto it = sprites.begin();
  while(it != sprites.end())
  {
    delete* it;
    it = sprites.erase(it);
  }
  std::cout << "Terminating program" << std::endl;
  delete Samus;
  delete midpoint;
  delete display;
}

Engine::Engine() :
  rc( &RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  stage("stage", Gamedata::getInstance().getXmlInt("stage/factor") ),
  hills("hills", Gamedata::getInstance().getXmlInt("hills/factor") ),
  display(new Hud("display")),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  Samus(new SubjectSprite("Samus")),
  makeVideo( false ),
  collision(false),
  showHUD(true),
  midpoint(new MidPointCollisionStrategy),
  sound(),
  score(),
  alive(),
  lights()
{
  Samus->setScale(1.5);
  Viewport::getInstance().setObjectToTrack(Samus);
  Vector2f pos = Samus->getPosition();
  int w = Samus->getScaledWidth();
  int h = Samus->getScaledHeight();
  score = 0;
  alive = true;
  for(int x = 0; x < 5; x++){
    sprites.emplace_back(new SmartSprite("Metroid", pos, w, h));
    Samus->attach(sprites[x]);
  }

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  hills.draw();
  stage.draw();

  Samus->draw();

  for(auto* x: sprites)
  {
    x->draw();
  }

  lights.draw();

  if(showHUD){
    display->draw();

  std::stringstream ss;
  ss << "FPS: " << clock.getFps() << " ";
  io.writeText(ss.str(), 670, 100);
  ss.str("");
  ss <<"PlayerPool: " << Samus->getBullet().bulletcount();
  io.writeText(ss.str(), 670, 120);
  ss.str("");
  ss <<"Score: " << score;
  io.writeText(ss.str(), 670, 140);
  ss.str("");

  if(!alive){
    io.writeText("You Died!", 394, 20);
    io.writeText("Press r to play again!", 360, 50);
  }
}
  //Draws the FPS count
  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::checkForExplosion()
{
  for(auto* x: sprites){
    if( x->shot(Samus) && Samus->getGodMode() == false){
      Samus->explode();
      sound[0];
      alive = false;
    }
    if(Samus->shot(x)){
      x->explode();
      sound[0];
      score++;
    }
    if(midpoint->execute(*Samus, *x) && Samus->getGodMode() == false)
    {
      x->explode();
      Samus->explode();
      sound[0];
    }
    if(midpoint->execute(*Samus, *x) && Samus->getGodMode() == true){
      x->explode();
      sound[0];
    }

  }
}
void Engine::update(Uint32 ticks) {
  Samus->update(ticks);

  for(auto* x: sprites)
  {
    x->update(ticks);
  }

  hills.update();
  stage.update();
  lights.update();
  viewport.update(); // always update viewport last

  if(showHUD){
    display->update(ticks);
  }


}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while (it != sprites.end() ) {
    if(midpoint->execute(*Samus, **it) && *it){
      SmartSprite* doa = *it;
      Samus->detach(doa);
      it = sprites.erase(it);
      delete doa;
    }
    else ++it;
  }
}




bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }

        if(keystate[SDL_SCANCODE_R]){
          return true;
        }

        if(keystate[SDL_SCANCODE_F1] && !showHUD)
        {
          showHUD = true;
        }
        else if(keystate[SDL_SCANCODE_F1] && showHUD)
        {
          showHUD = false;
        }

        if (keystate[SDL_SCANCODE_W]) {
            static_cast<Player*>(Samus)->up();
        }

        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    //Handles player movement
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(Samus)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(Samus)->right();
      }

      if(keystate[SDL_SCANCODE_R]){
        return true;
      }

      if (keystate[SDL_SCANCODE_SPACE]){
        static_cast<Player*>(Samus)->shoot();
      }
      if ( keystate[SDL_SCANCODE_G] ) {
        static_cast<Player* >(Samus)->toggleGodMode();
      }
      checkForExplosion();
      checkForCollisions();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}

/*void Engine::switchSprite(){
  if(currentSprite == 0)
  {
    Viewport::getInstance().setObjectToTrack(Samus);
    currentSprite++;
  }
  else
  {
    ++currentSprite;
    currentSprite = currentSprite % sprites.size();
    Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
    if((unsigned)currentSprite > sprites.size())
      currentSprite = 0;
  }
}*/
