#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include "../external/tinyxml2/tinyxml2.h"
#include <vector>
#include <cstring>
#include <stdio.h>
using std::vector;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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


    auto func = [] () { log("lambda"); };
    func();

    Vector<MenuItem*> MenuItems;
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                                origin.y + closeItem->getContentSize().height/2));
    auto restartLabel1 = Label::createWithTTF(labelConfig, "Restart");
    auto restartItem = MenuItemLabel::create(
                                            restartLabel1,
                                            CC_CALLBACK_1(HelloWorld::menuRestartCallback, this));
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
        wall->setPosition(Vec2(x, y));
        this->addChild(wall, 0);
        pWallElement = pWallElement->NextSiblingElement("wall");
    }

    auto coinFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Food.png");
    tinyxml2::XMLElement* pCoinElement = doc.FirstChildElement("lvl")->FirstChildElement("map")->FirstChildElement("food")->FirstChildElement("coin");
    vector<Sprite*> coin;
    for(size_t i = 0; pCoinElement != nullptr; i++) {
        pCoinElement->QueryFloatAttribute("x", &x);
        pCoinElement->QueryFloatAttribute("y", &y);
        coin.push_back(Sprite::createWithSpriteFrame(coinFrame));
        coin[i]->setPosition(Vec2(x, y));
        this->addChild(coin[i], 0);
        pCoinElement = pCoinElement->NextSiblingElement("coin");
    }
//    tinyxml2::XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
//    tinyxml2::XMLText* textNode = titleElement->FirstChild()->ToText();
//    printf( "Name of play (2): %s\n", textNode->Value() );

    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("sprites/sprites.plist");

//    auto pacmanFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Pacman.png");
//    auto pacman = Sprite::createWithSpriteFrame(pacmanFrame);
    auto pacman = Sprite::create();
//    auto pacman = Sprite::createWithSpriteFrameName("Pacman.png");
//    auto pacman = Sprite::create("icon.png", Rect(0, 0, 20, 20));
    pacman->setPosition(Vec2(300,250));
    pacman->setRotation(0);
    pacman->setScale(2);
    pacman->setAnchorPoint(Vec2(0.5, 0.5));
//    pacman->setColor(Color3B::BLUE);
    this->addChild(pacman, 1);

    Vector<SpriteFrame*> animPacman;
    animPacman.reserve(2);
    animPacman.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("Pacman.png"));
    animPacman.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("PacmanRound.png"));
    Animation* animation = Animation::createWithSpriteFrames(animPacman, 0.25f);
    Animate* animate = Animate::create(animation);
    pacman->runAction(RepeatForever::create(animate));

    auto callbackRotate = CallFunc::create([](){
        log("Rotated!");
    });
    auto moveBy1 = MoveBy::create(2, Vec2(200,0));
    auto moveRotate1 = RotateTo::create(0,-90.0f);
    auto moveBy2 = MoveBy::create(2, Vec2(0,150));
    auto delay = DelayTime::create(0.25);
    pacman->runAction(Sequence::create(moveBy1, moveRotate1, callbackRotate, moveBy2, nullptr));

    auto pinkyGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("PinkyGhost.png");
    auto pinkyGhost = Sprite::createWithSpriteFrame(pinkyGhostFrame);
//    auto ghost = Sprite::createWithSpriteFrameName("Pinkyghost.png");
//    auto ghost = Sprite::create("icon.png", Rect(0, 80, 20, 20));
    pinkyGhost->setPosition(Vec2(150,200));
    pinkyGhost->setRotation(0);
    pinkyGhost->setScale(2);
    pinkyGhost->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(pinkyGhost, 0);
    auto moveBy3 = MoveBy::create(2, Vec2(30,0));
    auto moveBy4 = MoveBy::create(2, Vec2(0,-80));
    pinkyGhost->runAction(Sequence::create(moveBy3, delay, moveBy4, nullptr));

    auto blueGhostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("BlueGhost.png");
    auto blueGhost = Sprite::createWithSpriteFrame(blueGhostFrame);
    blueGhost->setPosition(Vec2(700,visibleSize.height - blueGhost->getContentSize().height));
    blueGhost->setScale(2);
    blueGhost->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(blueGhost, 0);
    auto move1 = MoveBy::create(2, Vec2(0, - (visibleSize.height - 2*blueGhost->getContentSize().height)));
    auto move2 = move1->reverse();
    auto move_ease_in = EaseBounceOut::create(move1->clone());
    auto move_ease_in_back = EaseBounceOut::create(move2->clone());
    auto seq1 = Sequence::create(move_ease_in, delay->clone(), move_ease_in_back, delay->clone(), nullptr);
    blueGhost->runAction(RepeatForever::create(seq1));
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
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

void HelloWorld::menuRestartCallback(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}
