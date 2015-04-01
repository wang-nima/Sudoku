#include "GameScene.h"
#include "Definitions.h"
#include <UIButton.h>

USING_NS_CC;
using namespace cocos2d::ui;


Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backGroundSprite = Sprite::create("Background.jpg");
    backGroundSprite->setPosition( Point(visibleSize.width / 2 + origin.x,
                                         visibleSize.height / 2 + origin.y));
    this->addChild(backGroundSprite);
    
    auto uButton = Button::create();
    uButton->setTouchEnabled(true);
    uButton->loadTextures("cocosgui/animationbuttonnormal.png", "cocosgui/animationbuttonpressed.png", "");
    uButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(0, -50));
    this->addChild(uButton);
    
    
    return true;
}
