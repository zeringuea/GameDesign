#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "sprite.h"
#include "twowaymultisprite.h"
#include "bullets.h"
#include "explodingSprite.h"

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&) = default;
  SmartSprite& operator=(const SmartSprite&) = default;

  virtual ~SmartSprite() { }

  virtual void shoot();
  virtual void draw();
  virtual void update(Uint32 ticks);
  virtual bool shot(const Drawable*);

  void explode();
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  virtual const Image* getImage() const { return image; }
private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;

  std::string bulletName;
  Bullets bullet;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;
  ExplodingSprite* explosion;
  const Image* image;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
