#include "Ghost.h"

cocos2d::Sprite *Ghost::create()
{
    auto ghost1 = Sprite::create("sprites/RedGhost.png");
    auto physicsBody1 = cocos2d::PhysicsBody::createCircle(ghost1->getContentSize().width / 2 - 5);
    physicsBody1->setDynamic(false);
    physicsBody1->setGravityEnable(false);
    physicsBody1->setCategoryBitmask(1);
    physicsBody1->setCollisionBitmask(3);
    physicsBody1->setContactTestBitmask(1);
    ghost1->setPhysicsBody(physicsBody1);
    ghost1->setPosition(cocos2d::Vec2(340,250));
    ghost1->setRotation(0);
    ghost1->setScale(1);
    ghost1->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    ghost1->setTag(20);
    return ghost1;
}
