#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"
#include <math.h>

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { setVelocityX( -abs(getVelocityX()) );  }
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) );  }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) );  }


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos,
  int w, int h) :
  Sprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullet(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0),
    explosion(nullptr),
  image(ImageFactory::getInstance().getImage("Metroid"))

  {
  }


  void SmartSprite::shoot() {
  	if (timeSinceLastBullet > bulletInterval) {
      Vector2f vel = getVelocity();
      float x = getX();
      float y = getY();
      float distance = sqrt(pow(playerPos[0] - x, 2) + pow(playerPos[1] - y, 2));
      vel[0] = bulletSpeed * (playerPos[0] - x)/distance;
      vel[1] = bulletSpeed * (playerPos[1] - y)/distance;
      bullet.shoot(Vector2f(x, y), vel );
  		timeSinceLastBullet = 0;
    }
  }

bool SmartSprite::shot(const Drawable* obj){
  if (bullet.collidedWith(obj))
    return true;
  return false;
}


void SmartSprite::draw(){
  if(explosion){
    explosion->draw();
    return;
  }
  image->draw(getX(),getY(), getScale());
  bullet.draw();
}

void SmartSprite::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), image, getScale());
    explosion = new ExplodingSprite(sprite);
  }
}

void SmartSprite::update(Uint32 ticks) {
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Sprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );
  timeSinceLastBullet += ticks;
  bullet.update(ticks);
  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      shoot();
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}
