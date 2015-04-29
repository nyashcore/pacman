#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
#include "GameOverScene.cpp"
#include <iostream>
#include "Ghost.cpp"
#include "Pacman.cpp"
#include <vector>
#include <iterator>
#include <iostream>
using std::vector;
const EventKeyboard::KeyCode ENTER = EventKeyboard::KeyCode::KEY_KP_ENTER;
const EventKeyboard::KeyCode UP_ARROW = EventKeyboard::KeyCode::KEY_UP_ARROW;
const EventKeyboard::KeyCode DOWN_ARROW = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
#include "../external/tinyxml2/tinyxml2.h"
#include "HudLayer.cpp"

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
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyHold(float);
	vector<EventKeyboard::KeyCode> heldKeys;
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    // implement the "static create()" method manually
    CREATE_FUNC(Level);
    void update(float) override;
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);
private:
    int count;
    HudLayer *_hud;
    int _numCollected;
    cocos2d::Sprite* pacman;
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::TMXLayer *_walls;
    cocos2d::TMXLayer *_food;
    int flag;
};

#endif // __LEVEL_SCENE_H__
