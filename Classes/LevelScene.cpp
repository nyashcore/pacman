#include "LevelScene.h"
#include "StartMenuScene.h"

USING_NS_CC;

Level* Level::create(int level) {

    Level *sc = new Level();
    sc->setLvl(level);

    if (sc->init()) {
        sc->autorelease();
    } else
        sc = NULL;

    return sc;
}

Scene* Level::createScene(int level)
{
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NO);
    auto layer = Level::create(level);
    scene->addChild(layer);
    HudLayer *hud = new HudLayer();
    hud->init();
    scene->addChild(hud);
    layer->_hud = hud;


    return scene;
}

bool Level::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    TTFConfig labelConfig;
    labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
    labelConfig.fontSize = 24;
    labelConfig.glyphs = GlyphCollection::DYNAMIC;
    labelConfig.outlineSize = 0;
    labelConfig.customGlyphs = nullptr;
    labelConfig.distanceFieldEnabled = false;

    builder.setLvl(this->lvl);
    map = dir.createMap(builder);
    pacman = map.pacman->getPacman();
    _tileMap = map._tileMap;
    _walls = map._walls;
    _food = map._food;
    count = _tileMap->getProperty("count").asByte();
    numOfDumb = _tileMap->getProperty("numOfDumb").asByte();
    numOfClev = _tileMap->getProperty("numOfClev").asByte();
    addChild(_tileMap, 0, 99);
    Vector<MenuItem*> MenuItems;
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Level::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                                origin.y + closeItem->getContentSize().height/2));
	MenuItems.pushBack(closeItem);
    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    char topLabel[10];
    sprintf(topLabel, "level %d", this->lvl);
    auto label = Label::createWithTTF(labelConfig, topLabel);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    label->enableOutline(Color4B::GRAY, 1);
    this->addChild(label, 1);

    auto nodecache = SpriteFrameCache::getInstance();
    nodecache->addSpriteFramesWithFile("nodes/nodes.plist");
    auto wallFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Wall.png");

    tinyxml2::XMLDocument doc;
    doc.LoadFile("../../../Resources/levels.xml");
    tinyxml2::XMLElement* pWallElement = doc.FirstChildElement("lvl")->FirstChildElement("map");

    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("sprites/sprites.plist");

    this->addChild(pacman, 1);

    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
    animFrames.pushBack(SpriteFrame::create("sprites/Pacman.png", Rect(0,0,31,31)));
    animFrames.pushBack(SpriteFrame::create("sprites/PacmanRound.png", Rect(0,0,31,31)));
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);

    pacman->runAction(RepeatForever::create(animate));
    for(int i = 1; i < numOfDumb+numOfClev+1; i++) {
        Ghost ghost1;
        auto ohMyGodWhatAGhost = ghost1.create(i);
        this->addChild(ohMyGodWhatAGhost, 1);

        auto moveBy1 = MoveBy::create(2/(0.1+i), Vec2((i+1)*60,0));
        auto moveBy2 = MoveBy::create(2/(0.1+i), Vec2(0,-60*(i+1)));
        auto moveBy12 = MoveBy::create(2/(0.1+i), Vec2(-60*(i+1),0));
        auto moveBy21 = MoveBy::create(2/(0.1+i), Vec2(0,60*(i+1)));
        auto seq12 = Sequence::create(moveBy1, moveBy2, moveBy12, moveBy21, nullptr);
        Action* act = RepeatForever::create(seq12);
        act->setTag(2);
        ohMyGodWhatAGhost->runAction(act);
    }

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Level::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Level::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, pacman);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto delay = DelayTime::create(1.0);
//    auto pinkyGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("PinkyGhost.png");
//    auto pinkyGhost = Sprite::createWithSpriteFrame(pinkyGhostFrame);
//    pinkyGhost->setPosition(Vec2(150,200));
//    pinkyGhost->setRotation(0);
//    pinkyGhost->setScale(1);
//    pinkyGhost->setAnchorPoint(Vec2(0.5, 0.5));
//    this->addChild(pinkyGhost, 0);
//    auto moveBy3 = MoveBy::create(2, Vec2(30,0));
//    auto moveBy4 = MoveBy::create(2, Vec2(0,-80));
//    pinkyGhost->runAction(Sequence::create(moveBy3, delay, moveBy4, nullptr));
//
//    auto blueGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("BlueGhost.png");
//    auto blueGhost = Sprite::createWithSpriteFrame(blueGhostFrame);
//    blueGhost->setPosition(Vec2(700,visibleSize.height - blueGhost->getContentSize().height/2));
//    blueGhost->setScale(1);
//    blueGhost->setAnchorPoint(Vec2(0.5, 0.5));
//    this->addChild(blueGhost, 0);
//    auto move1 = MoveBy::create(2, Vec2(0,-(visibleSize.height - blueGhost->getContentSize().height)));
//    auto move2 = move1->reverse();
//    auto move_ease_in = EaseBounceOut::create(move1->clone());
//    auto move_ease_in_back = EaseBounceOut::create(move2->clone());
//    auto seq1 = Sequence::create(move_ease_in, delay->clone(), move_ease_in_back, delay->clone(), nullptr);
//    blueGhost->runAction(RepeatForever::create(seq1));

    this->schedule(schedule_selector(Level::onKeyHold));

    return true;
}


void Level::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TfARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool Level::onContactBegin(cocos2d::PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if( nodeA && nodeB ) {
        if( nodeA->getTag() == 20 || nodeB->getTag() == 20 ) {
//            pacman->stopAllActionsByTag(1);
            nodeB->stopAllActionsByTag(2);
            nodeA->stopAllActionsByTag(2);
            nodeA->stopAllActionsByTag(1);
            nodeB->stopAllActionsByTag(1);
            Pacman::setInstance();
            pacman->stopAllActionsByTag(1);
            _numCollected = -1;
            auto scene = GameOver::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        } else if( nodeA->getTag() == 10 || nodeB->getTag() == 10 ) {
            if( nodeA->getTag() == 15 ) {
                nodeA->stopAllActionsByTag(1);
            } else if ( nodeB->getTag() == 15 ) {
                nodeB->stopAllActions();
            }
        }
    }
}

void Level::update(float delta){
    if(_numCollected == this->count) {
        _numCollected = -1;
        if(lvl < NUM_OF_LEVELS) {
            Pacman::setInstance();
            pacman->stopAllActionsByTag(1);
            auto scene = Level::createScene(lvl+1);
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        } else {
            Pacman::setInstance();
            pacman->stopAllActionsByTag(1);
            auto scene = GameOver::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        }
    }
    auto position = pacman->getPosition();
    if (position.x  < 0 - (pacman->getBoundingBox().size.width / 2)) {
      position.x = this->getBoundingBox().getMaxX() + pacman->getBoundingBox().size.width/2;
    }
    if (position.x > this->getBoundingBox().getMaxX() + pacman->getBoundingBox().size.width/2) {
      position.x = 0;
    }
    if (position.y < 0 - (pacman->getBoundingBox().size.height / 2)) {
      position.y = this->getBoundingBox().getMaxY() + pacman->getBoundingBox().size.height/2;
    }
    if (position.y > this->getBoundingBox().getMaxY() + pacman->getBoundingBox().size.height/2) {
      position.y = 0;
    }
    pacman->setPosition(position);

    if (flag == 1) {    // флаг
        position.x -= 17;
        if(position.x > 0) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _walls->getTileGIDAt(tileCoord);
            if (tileGid) {
                pacman->stopAllActionsByTag(1);
                flag = 0;
            }
        } else { return; }
        position.x += 17;
        if(position.x < Director::getInstance()->getVisibleSize().width) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _food->getTileGIDAt(tileCoord);
            if(tileGid) {
                _food->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
            }
        }
    }
    if (flag == 2) {
        position.x += 17;
        if(position.x < Director::getInstance()->getVisibleSize().width) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _walls->getTileGIDAt(tileCoord);
            if (tileGid) {
                pacman->stopAllActionsByTag(1);
                flag = 0;
            }
        } else { return; }
        position.x -= 17;
        if(position.x > 0) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _food->getTileGIDAt(tileCoord);
            if(tileGid) {
                _food->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
            }
        }
    }
    if (flag == 3) {    // просто потому что мы можем
        position.y += 17;
        if(position.y < 640) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _walls->getTileGIDAt(tileCoord);
            if (tileGid) {
                pacman->stopAllActionsByTag(1);
                flag = 0;
            }
        } else { return; }
        position.y -= 17;
        if(position.y > 0) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _food->getTileGIDAt(tileCoord);
            if(tileGid) {
                _food->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
            }
        }
    }
    if (flag == 4) {
        position.y -= 17;
        if(position.y > 0) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _walls->getTileGIDAt(tileCoord);
            if (tileGid) {
                pacman->stopAllActionsByTag(1);
                flag = 0;
            }
        } else { return; }
        position.y += 17;
        if(position.y < 640) {
            Point tileCoord = tileCoordForPosition(position);
            int tileGid = _food->getTileGIDAt(tileCoord);
            if(tileGid) {
                _food->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
            }
        }
    }
}

Point Level::tileCoordForPosition(Point position)
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return Point(x, y);
}

void Level::onKeyHold(float interval){

    if(std::find(heldKeys.begin(), heldKeys.end(), UP_ARROW) != heldKeys.end()){
        Point position = pacman->getPosition();
        position.y += 32;
        position.x -= 15;
        Point tileCoord;
        int tileGid1, tileGid;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid = _walls->getTileGIDAt(tileCoord);
        } else {tileGid = 0;}
        position.x += 30;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid1 = _walls->getTileGIDAt(tileCoord);
        } else {tileGid1 = 0;}
        if(!tileGid && !tileGid1){
            heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), UP_ARROW), heldKeys.end());
            auto moveUp = MoveBy::create(0.3, Vec2(0, 32));
            Action* action = RepeatForever::create(moveUp);
            action->setTag(1);
            pacman->stopAllActionsByTag(1);
            pacman->setRotation(-90.0f);
            pacman->runAction(action);
            flag = 3;
            this->scheduleUpdate();
        }
    }
    if(std::find(heldKeys.begin(), heldKeys.end(), DOWN_ARROW) != heldKeys.end()){
        Point position = pacman->getPosition();
        position.y -= 32;
        position.x -= 15;
        Point tileCoord;
        int tileGid1, tileGid;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid = _walls->getTileGIDAt(tileCoord);
        } else {tileGid = 0;}
        position.x += 30;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid1 = _walls->getTileGIDAt(tileCoord);
        } else {tileGid1 = 0;}
        if(!tileGid && !tileGid1){
            heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), DOWN_ARROW), heldKeys.end());
            auto moveDown = MoveBy::create(0.3, Vec2(0, -32));
            Action* action = RepeatForever::create(moveDown);
            action->setTag(1);
            pacman->stopAllActionsByTag(1);
            pacman->setRotation(90.0f);
            pacman->runAction(action);
            flag = 4;
            this->scheduleUpdate();
        }
    }
    if(std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()){
        Point position = pacman->getPosition();
        position.x += 32;
        position.y -= 15;
        Point tileCoord;
        int tileGid, tileGid1;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid = _walls->getTileGIDAt(tileCoord);
        } else {tileGid = 0;}
        position.y += 30;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid1 = _walls->getTileGIDAt(tileCoord);
        } else {tileGid1 = 0;}
        if(!tileGid && !tileGid1){
            heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW), heldKeys.end());
            auto moveRight = MoveBy::create(0.3, Vec2(32,0));
            Action* action = RepeatForever::create(moveRight);
            action->setTag(1);
            pacman->stopAllActionsByTag(1);
            pacman->setRotation(0.0f);
            pacman->runAction(action);
            flag = 2;
            this->scheduleUpdate();
        }
    }
    if(std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()){
        Point position = pacman->getPosition();
        position.x -= 32;
        position.y -= 15;
        Point tileCoord;
        int tileGid, tileGid1;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid = _walls->getTileGIDAt(tileCoord);
        } else {tileGid = 0;}
        position.y += 30;
        if(position.x >= 0 && position.y >= 0 && position.y < Director::getInstance()->getVisibleSize().height && position.x < Director::getInstance()->getVisibleSize().width) {
            tileCoord = tileCoordForPosition(position);
            tileGid1 = _walls->getTileGIDAt(tileCoord);
        } else {tileGid1 = 0;}
        if(!tileGid && !tileGid1){
            heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), LEFT_ARROW), heldKeys.end());
            auto moveLeft = MoveBy::create(0.3, Vec2(-32, 0));
            Action* action = RepeatForever::create(moveLeft);
            action->setTag(1);
            pacman->stopAllActionsByTag(1);
            pacman->setRotation(180.0f);
            pacman->runAction(action);
            flag = 1;
            this->scheduleUpdate();
        }
    }

}

void Level::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
			heldKeys.push_back(keyCode);
}

void Level::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}
