#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
#include "GameOverScene.h"
#include <iostream>
#include "Ghost.h"
#include "Director.cpp"
#include <vector>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
using std::vector;
using std::string;
const cocos2d::EventKeyboard::KeyCode ENTER = cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER;
const cocos2d::EventKeyboard::KeyCode UP_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW;
const cocos2d::EventKeyboard::KeyCode DOWN_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const cocos2d::EventKeyboard::KeyCode LEFT_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const cocos2d::EventKeyboard::KeyCode RIGHT_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
#include "../external/tinyxml2/tinyxml2.h"
#include "HudLayer.h"
#define NUM_OF_LEVELS 3

class Level : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int);
    static Level* create(int);

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
    void setLvl(int level) { lvl = level; }
    int getLvl(void) { return lvl; }
private:
    int lvl;
    int count;
    HudLayer *_hud;
    int _numCollected;
    cocos2d::Sprite* pacman;
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::TMXLayer *_walls;
    cocos2d::TMXLayer *_food;
    DirectorMap dir;
    MapBuilder builder;
    MapPacman map;
    int flag;
};

#endif // __LEVEL_SCENE_H__
