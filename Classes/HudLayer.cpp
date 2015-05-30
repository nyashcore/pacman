#include "HudLayer.h"

using namespace cocos2d;

bool HudLayer::init()
{
    if (Layer::init()) {
//        Size winSize = Director::sharedDirector()->getWinSize();

        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        TTFConfig labelConfig;
        labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
        labelConfig.fontSize = 24;
        labelConfig.glyphs = GlyphCollection::DYNAMIC;
        labelConfig.outlineSize = 0;
        labelConfig.customGlyphs = nullptr;
        labelConfig.distanceFieldEnabled = false;
        _label = Label::createWithTTF(labelConfig, "0");
        _label->setPosition(Vec2(5*_label->getContentSize().width/2, visibleSize.height - _label->getContentSize().height/2));
        this->addChild(_label);
    }

    return true;
}

void HudLayer::numCollectedChanged(int numCollected)
{
    String *labelCollected = new String();
    labelCollected->initWithFormat("%d",numCollected);
    _label->setString(labelCollected->getCString());
}
