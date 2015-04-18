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
    pacman->setPosition(100, 100);
    return pacman;
}
