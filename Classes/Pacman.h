#include "cocos2d.h"
using namespace cocos2d;

class Pacman;  // опережающее объявление

class PacmanDestroyer
{
  private:
    Pacman* pacmanInstance;
  public:
    ~PacmanDestroyer();
    void initialize( Pacman* p );
};

class Pacman : public cocos2d::Sprite {
public:
    cocos2d::Sprite *getPacman() {
        return pacman;
    }
    static void setInstance() { pacmanInstance = 0; }
    static Pacman* getInstance();
private:
    static Pacman *pacmanInstance;
    static PacmanDestroyer destroyer;
    cocos2d::Sprite *pacman;
    Pacman() {
        pacman = Sprite::create("sprites/Pacman.png");
        Vector<SpriteFrame*> animFrames;
        animFrames.reserve(2);
        animFrames.pushBack(SpriteFrame::create("sprites/Pacman.png", Rect(0,0,31,31)));
        animFrames.pushBack(SpriteFrame::create("sprites/PacmanRound.png", Rect(0,0,31,31)));
        // create the animation out of the frames
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        Animate* animate = Animate::create(animation);
        auto physicsBody = PhysicsBody::createCircle(pacman->getContentSize().width / 2 - 1);
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(3);
        physicsBody->setCollisionBitmask(1);
        physicsBody->setContactTestBitmask(1);

        pacman->setPhysicsBody(physicsBody);
    //    auto pacman = Sprite::createWithSpriteFrameName("Pacman.png");
        pacman->setPosition(Vec2(128+16, 128+16+32));
        pacman->setRotation(0);
        pacman->setScale(1);
        pacman->setAnchorPoint(Vec2(0.5, 0.5));
        pacman->setTag(15);
    }
     friend class PacmanDestroyer;
};
