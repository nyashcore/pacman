#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "../external/tinyxml2/tinyxml2.h"
#include <vector>
#include <cstring>
#include <stdio.h>
using std::vector;

class Level : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuRestartCallback(cocos2d::Ref* pSender);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    // implement the "static create()" method manually
    CREATE_FUNC(Level);
};

#endif // __LEVEL_SCENE_H__
