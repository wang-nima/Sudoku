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
    if ( !Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto backGroundSprite = Sprite::create("Background.jpg");
    //backGroundSprite->setPosition( Point(visibleSize.width / 2 + origin.x,
    //                                     visibleSize.height / 2 + origin.y));
    //this->addChild(backGroundSprite);
    
    
    auto label = Label::createWithTTF("Enjor your game!", "Naughty Cartoons.ttf", 40);
    label->setPosition(Point(visibleSize.width / 2 + origin.x,
                             visibleSize.height / 2 + origin.y + 100));
    this->addChild(label);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        
        auto touchPoint = touch->getLocation();
        start_x = touchPoint.x;
        start_y = touchPoint.y;
        return true;
    };
    
    touchListener->onTouchEnded = [=] (Touch* touch, Event* event) -> void {
        
        auto touchPoint = touch->getLocation();
        int end_x = touchPoint.x;
        int end_y = touchPoint.y;
        int dx = end_x - start_x;
        int dy = end_y - start_y;
        
        if (abs(dx) < 10 || abs(dy) < 10) {
            return;
        }
        
        if (abs(dx) > abs(dy)) {        //move left or right
            if (dx > 0) {
                doRight();
            } else {
                doLeft();
            }
        } else {    //up and down
            if (dy > 0) {
                doUp();
            } else {
                doDown();
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}


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
