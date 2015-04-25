#include "Pacman.h"

Sprite* Pacman::create()
{
    auto pacman = Sprite::create("sprites/Pacman.png");
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
    return pacman;
}
