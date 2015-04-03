#include "GameScene.h"
#include "Definitions.h"
#include <cmath>

USING_NS_CC;
using namespace cocos2d::ui;


Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !LayerColor::initWithColor(Color4B::WHITE)) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto backGroundSprite = Sprite::create("Background.jpg");
    //backGroundSprite->setPosition( Point(visibleSize.width / 2 + origin.x,
    //                                     visibleSize.height / 2 + origin.y));
    //this->addChild(backGroundSprite);
    
    
    auto label = Label::createWithTTF("Enjor your game!", "Naughty Cartoons.ttf", 40);
    label->setPosition(Point(visibleSize.width / 2,
                             visibleSize.height + 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
    };
    
    touchListener->onTouchEnded = [=] (Touch* touch, Event* event) -> void {
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}
