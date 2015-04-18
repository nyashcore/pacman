#include "LevelScene.h"

USING_NS_CC;

Scene* Level::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Level::create();
//    layer->setPhyWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
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
     auto map = TMXTiledMap::create("map/pacman.tmx");
    addChild(map, 0, 99);
    Vector<MenuItem*> MenuItems;
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Level::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                                origin.y + closeItem->getContentSize().height/2));
    auto restartLabel1 = Label::createWithTTF(labelConfig, "Restart");
    auto restartItem = MenuItemLabel::create(
                                            restartLabel1,
                                            CC_CALLBACK_1(Level::menuRestartCallback, this));
    restartItem->setPosition(Vec2(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height/3));
	MenuItems.pushBack(closeItem);
	MenuItems.pushBack(restartItem);
    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = Label::createWithTTF(labelConfig, "Hello, Bitches!");
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
//    label->enableShadow(Color4B::WHITE);
//    label->enableGlow(Color4B::YELLOW);
    label->enableOutline(Color4B::GRAY, 1);
    this->addChild(label, 1);

    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);



    auto nodecache = SpriteFrameCache::getInstance();
    nodecache->addSpriteFramesWithFile("nodes/nodes.plist");
    auto wallFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Wall.png");

    tinyxml2::XMLDocument doc;
    doc.LoadFile("../../../Resources/levels.xml");
    float x, y;
    tinyxml2::XMLElement* pWallElement = doc.FirstChildElement("lvl")->FirstChildElement("map")->FirstChildElement("walls")->FirstChildElement("wall");
    for(size_t i = 0; pWallElement != nullptr; i++) {
        pWallElement->QueryFloatAttribute("x", &x);
        pWallElement->QueryFloatAttribute("y", &y);
        auto wall = Sprite::createWithSpriteFrame(wallFrame);
        auto physicsBody = PhysicsBody::createBox(Size(32.0f , 32.0f ), PhysicsMaterial(0.1f, 1.0f, 0.0f));
        physicsBody->setGravityEnable(false);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(3);
        physicsBody->setCollisionBitmask(1);
        physicsBody->setContactTestBitmask(1);
        wall->setPhysicsBody(physicsBody);
        wall->setPosition(Vec2(x, y));
        wall->setTag(10);
        this->addChild(wall, 0);
        pWallElement = pWallElement->NextSiblingElement("wall");
    }

    auto coinFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Food.png");
    tinyxml2::XMLElement* pCoinElement = doc.FirstChildElement("lvl")->FirstChildElement("map")->FirstChildElement("food")->FirstChildElement("coin");
    for(size_t i = 0; pCoinElement != nullptr; i++) {
        pCoinElement->QueryFloatAttribute("x", &x);
        pCoinElement->QueryFloatAttribute("y", &y);
        auto coin = Sprite::createWithSpriteFrame(coinFrame);
        coin->setPosition(Vec2(x, y));
        this->addChild(coin, 0);
        pCoinElement = pCoinElement->NextSiblingElement("coin");
    }
//    tinyxml2::XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
//    tinyxml2::XMLText* textNode = titleElement->FirstChild()->ToText();
//    printf( "Name of play (2): %s\n", textNode->Value() );

    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("sprites/sprites.plist");

    auto pacmanFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Pacman.png");
    auto pacman = Sprite::createWithSpriteFrame(pacmanFrame);
//    physicsBody->setDynamic(false);
    auto physicsBody = PhysicsBody::createCircle(pacman->getContentSize().width / 2 - 1);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(3);
    physicsBody->setCollisionBitmask(1);
    physicsBody->setContactTestBitmask(1);
    pacman->setPhysicsBody(physicsBody);
//    auto pacman = Sprite::createWithSpriteFrameName("Pacman.png");
    pacman->setPosition(Vec2(300,250));
    pacman->setRotation(0);
    pacman->setScale(1);
    pacman->setAnchorPoint(Vec2(0.5, 0.5));
    pacman->setTag(15);
//    pacman->setColor(Color3B::BLUE);
    this->addChild(pacman, 1);

    Vector<SpriteFrame*> animPacman;
    animPacman.reserve(2);
    animPacman.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("Pacman.png"));
    animPacman.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("PacmanRound.png"));
    Animation* animation = Animation::createWithSpriteFrames(animPacman, 0.25f);
    Animate* animate = Animate::create(animation);
    pacman->runAction(RepeatForever::create(animate));

//    physicsBody1->setDynamic(false);
    auto pacman1 = Sprite::create("CloseNormal.png");
    auto physicsBody1 = PhysicsBody::createCircle(pacman1->getContentSize().width / 2 - 5);
    physicsBody1->setDynamic(false);
    physicsBody1->setGravityEnable(false);
    physicsBody1->setCategoryBitmask(1);
    physicsBody1->setCollisionBitmask(3);
    physicsBody1->setContactTestBitmask(1);
    pacman1->setPhysicsBody(physicsBody1);
    pacman1->setPosition(Vec2(340,250));
    pacman1->setRotation(0);
    pacman1->setScale(1);
    pacman1->setAnchorPoint(Vec2(0.5, 0.5));
    pacman1->setTag(20);
//    pacman->setColor(Color3B::BLUE);
    this->addChild(pacman1, 1);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Level::onKeyPressed, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, pacman);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
//    schedule(CC_SCHEDULE_SELECTOR(PhysicsDemoCollisionProcessing::tick), 0.3f);

    auto delay = DelayTime::create(0.25);
    auto pinkyGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("PinkyGhost.png");
    auto pinkyGhost = Sprite::createWithSpriteFrame(pinkyGhostFrame);
    pinkyGhost->setPosition(Vec2(150,200));
    pinkyGhost->setRotation(0);
    pinkyGhost->setScale(1);
    pinkyGhost->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(pinkyGhost, 0);
    auto moveBy3 = MoveBy::create(2, Vec2(30,0));
    auto moveBy4 = MoveBy::create(2, Vec2(0,-80));
    pinkyGhost->runAction(Sequence::create(moveBy3, delay, moveBy4, nullptr));

    auto blueGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("BlueGhost.png");
    auto blueGhost = Sprite::createWithSpriteFrame(blueGhostFrame);
    blueGhost->setPosition(Vec2(700,visibleSize.height - blueGhost->getContentSize().height/2));
    blueGhost->setScale(1);
    blueGhost->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(blueGhost, 0);
    auto move1 = MoveBy::create(2, Vec2(0,-(visibleSize.height - blueGhost->getContentSize().height)));
    auto move2 = move1->reverse();
    auto move_ease_in = EaseBounceOut::create(move1->clone());
    auto move_ease_in_back = EaseBounceOut::create(move2->clone());
    auto seq1 = Sequence::create(move_ease_in, delay->clone(), move_ease_in_back, delay->clone(), nullptr);
    blueGhost->runAction(RepeatForever::create(seq1));

    return true;
}


void Level::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Level::menuRestartCallback(Ref* pSender)
{
    auto scene = Level::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(1, scene));
}

bool Level::onContactBegin(cocos2d::PhysicsContact& contact)
{

    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if( nodeA && nodeB ) {
        if( nodeA->getTag() == 20 || nodeB->getTag() == 20 ) {
            auto scene = GameOver::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInT::create(1, scene));
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
   auto position = pacman->getPosition();
   if (position.x  < 0 - (pacman->getBoundingBox().size.width / 2))
      position.x = this->getBoundingBox().getMaxX() + pacman->getBoundingBox().size.width/2;
   if (position.x > this->getBoundingBox().getMaxX() + pacman->getBoundingBox().size.width/2)
      position.x = 0;
   if (position.y < 0 - (pacman->getBoundingBox().size.height / 2))
      position.y = this->getBoundingBox().getMaxY() + pacman->getBoundingBox().size.height/2;
   if (position.y > this->getBoundingBox().getMaxY() + pacman->getBoundingBox().size.height/2)
      position.y = 0;
   pacman->setPosition(position);
}

void Level::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
     switch(keyCode){
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A: {
            auto moveLeft = MoveBy::create(0.2, Vec2(-32, 0));
            Action* action = RepeatForever::create(moveLeft);
            action->setTag(1);
            event->getCurrentTarget()->stopAllActionsByTag(1);
            event->getCurrentTarget()->setRotation(180.0f);
            event->getCurrentTarget()->runAction(action);
            this->scheduleUpdate();
            break;
        }
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D: {
            auto moveRight = MoveBy::create(0.2, Vec2(32,0));
            Action* action = RepeatForever::create(moveRight);
            action->setTag(1);
            event->getCurrentTarget()->stopAllActionsByTag(1);
            event->getCurrentTarget()->setRotation(0.0f);
            event->getCurrentTarget()->runAction(action);
            this->scheduleUpdate();
            break;
        }
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W: {
            auto moveUp = MoveBy::create(0.2, Vec2(0, 32));
            Action* action = RepeatForever::create(moveUp);
            action->setTag(1);
            event->getCurrentTarget()->stopAllActionsByTag(1);
            event->getCurrentTarget()->setRotation(-90.0f);
            event->getCurrentTarget()->runAction(action);
            this->scheduleUpdate();
            break;
        }
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S: {
            auto moveDown = MoveBy::create(0.2, Vec2(0, -32));
            Action* action = RepeatForever::create(moveDown);
            action->setTag(1);
            event->getCurrentTarget()->stopAllActionsByTag(1);
            event->getCurrentTarget()->setRotation(90.0f);
            event->getCurrentTarget()->runAction(action);
            this->scheduleUpdate();
            break;
        }
    }
}
