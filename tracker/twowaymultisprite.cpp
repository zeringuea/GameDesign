#include "twowaymultisprite.h"
#include "gameData.h"
#include "imageFactory.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	int section = numberOfFrames / 2;
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		currentFrame++;
		timeSinceLastFrame = 0;
		if(getVelocityX() > 0)
		{
			currentFrame = currentFrame % section;
		}
		else{
			if(currentFrame == numberOfFrames)
			{
				currentFrame = section++;
			}
		}
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
										Gamedata::getInstance().getXmlFloat(name+"/scale")
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWayMultiSprite::TwoWayMultiSprite(TwoWayMultiSprite const& s) :
	Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{}

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& t)
{
	Drawable::operator=(t);
	images = t.images;
	currentFrame = t.currentFrame;
	numberOfFrames = t.currentFrame;
	frameInterval = t.frameInterval;
	timeSinceLastFrame = t.timeSinceLastFrame;
	worldWidth = t.worldWidth;
	worldHeight = t.worldHeight;
	return *this;
}

void TwoWayMultiSprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

}
