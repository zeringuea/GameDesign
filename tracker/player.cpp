#include "player.h"
#include "chunk.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  collision(false),
  GodMode(false),
  initialVelocity(getVelocity()),
  explosion(nullptr),
  jumpHeight(50),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullet(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s),
  collision(s.collision),
  GodMode(s.GodMode),
  initialVelocity(s.getVelocity()),
  explosion(s.explosion),
  jumpHeight(s.jumpHeight),
  bulletName(s.bulletName),
  bullet(s.bullet),
  bulletSpeed(s.bulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(0)
  { }

Player& Player::operator=(const Player& s) {
  TwoWayMultiSprite::operator=(s);
  collision = s.collision;
  GodMode = s.GodMode;
  initialVelocity = s.initialVelocity;
  explosion = s.explosion;
  jumpHeight = s.jumpHeight;
  bulletName = s.bulletName;
  bullet = s.bullet;
  bulletSpeed = s.bulletSpeed;
  bulletInterval = s.bulletInterval;
  timeSinceLastBullet = s.timeSinceLastBullet;
  return *this;
}

void Player::stop() {
  setVelocityX( 0.93*getVelocityX() );
  if(isJumping){
    setVelocityY(getVelocityY() + 10);
    if(getY() >= 320)
      isJumping = false;
  }
  else{
    setVelocityY(0);
    setY(320);
  }
}

void Player::explode() {
  if ( !explosion ) {
    Sprite
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame], getScale());
    explosion = new ExplodingSprite(sprite);
  }
}

void Player::shoot() {
  // See if it's time to shoot a bullet:
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x;
    float y = getY()+getScaledHeight() - 30;
    if ( vel[0] > 0 ) {
      x = getX()+getScaledWidth()-10;
      vel[0] += bulletSpeed;
    }
    else {
      x = getX();
      vel[0] -= bulletSpeed;
    }
    bullet.shoot(Vector2f(x, y), vel );
    timeSinceLastBullet = 0;
  }
}


void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  if ( getY() > 0 && isJumping == false) {
    isJumping = true;
    setVelocityY(-initialVelocity[1] - 20);
  }
}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::toggleGodMode(){
  if(GodMode == true)
    GodMode = false;
  else
    GodMode = true;
}

bool Player::getGodMode(){
  return GodMode;
}

bool Player::shot(const Drawable* obj){
  if (bullet.collidedWith(obj))
    return true;
  return false;
}

void Player::draw(){
  if ( explosion ) {
    explosion->draw();
    return;
  }
  images[currentFrame]->draw(getX(), getY(), getScale());
  bullet.draw();
}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
  explosion->update(ticks);
  if ( explosion->chunkCount() == 0 ) {
    delete explosion;
    explosion = NULL;
  }
  return;
}
  timeSinceLastBullet += ticks;
  bullet.update(ticks);
  if ( !collision ) TwoWayMultiSprite::update(ticks);
  stop();
}
