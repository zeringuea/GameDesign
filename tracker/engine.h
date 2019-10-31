#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "world.h"
#include "hud.h"
#include "viewport.h"
#include "collisionStrategy.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "sound.h"
#include "lights.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World stage;
  World hills;

  Hud* display;
  Viewport& viewport;

  std::vector<SmartSprite*> sprites;
  int currentSprite;
  SubjectSprite* Samus;

  bool makeVideo;
  bool collision;
  bool showHUD;
  MidPointCollisionStrategy* midpoint;
  Sound sound;
  int score;
  bool alive;
  Lights lights;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
  void checkForExplosion();
};
