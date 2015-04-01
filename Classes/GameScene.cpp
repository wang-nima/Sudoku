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
    
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){
        // your code
        doUp();
        return true; // if you are consuming it
    };
    
    touchListener->onTouchEnded = [=](Touch* touch, Event* event){
        // your code
    };
    
    return true;
}


//滑向上下左右的方法
bool GameScene::doUp()
{
    log("doUp");
    return true;
}

bool GameScene::doDown()
{
    log("doDown");
    return true;
}

bool GameScene::doLeft()
{
    log("doLeft");
    return true;
}

bool GameScene::doRight()
{
    log("doRight");
    return true;
}
