#ifndef PLAYER__H
#define PLAYER__H

#include "twowaymultisprite.h"
#include "explodingSprite.h"
#include "bullets.h"
// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  void explode();
  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void draw();
  void shoot();
  bool shot(const Drawable*);

  void right();
  void left();
  void up();
  void down();
  void stop();
  Bullets getBullet(){return bullet;}
  void toggleGodMode();
  bool getGodMode();
private:
  bool collision;
  bool GodMode;
  bool isJumping = false ;
  Vector2f initialVelocity;
  ExplodingSprite* explosion;
  int jumpHeight;

  std::string bulletName;
  Bullets bullet;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;
};
#endif
