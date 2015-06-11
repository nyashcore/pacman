#include "GameOverScene.h"
#include "LevelScene.h"
USING_NS_CC;

Scene* GameOver::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOver::create();
    scene->addChild(layer);
    return scene;
}

bool GameOver::init()
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

    Vector<MenuItem*> MenuItems;
    auto closeLabel = Label::createWithTTF(labelConfig, "Quit");
    auto closeItem = MenuItemLabel::create(
                                           closeLabel,
                                           CC_CALLBACK_1(GameOver::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height/3 - 30));
	MenuItems.pushBack(closeItem);
    auto restartLabel1 = Label::createWithTTF(labelConfig, "Restart");
    auto restartItem = MenuItemLabel::create(
                                            restartLabel1,
                                            CC_CALLBACK_1(GameOver::menuRestartCallback, this));
    restartItem->setPosition(Vec2(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height/3));
	MenuItems.pushBack(restartItem);

    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = Label::createWithTTF(labelConfig, "Game Over!");
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    label->enableOutline(Color4B::GRAY, 1);
    this->addChild(label, 1);

    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);

    return true;
}


void GameOver::menuCloseCallback(Ref* pSender)
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
void GameOver::menuRestartCallback(Ref* pSender)
{
    auto scene = Level::createScene(1);
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}
