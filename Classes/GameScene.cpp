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
    auto center = Point(visibleSize.width / 2, visibleSize.height / 2);
    
    auto label = Label::createWithTTF("Enjor your game!", "Naughty Cartoons.ttf", 40);
    label->setPosition(Point(visibleSize.width / 2,
                             visibleSize.height - 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
    };
    touchListener->onTouchEnded = [=] (Touch* touch, Event* event) -> void {
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    game.regenerate(2);
    
    //add game board
    auto board = Sprite::create("board.png");
    board->setPosition(center);
    this->addChild(board);
    
    auto boardSize = board->getContentSize();
    //CCLOG("%f %f", boardSize.height, boardSize.width);
    auto boardPosition = board->getPosition();
    //CCLOG("%f %f", boardPosition.x, boardPosition.y);
    Point boardTopLeft(boardPosition.x - boardSize.width / 2,
                       boardPosition.y + boardSize.height / 2);
    int cellLength = boardSize.height / 9;
    
    
    
    
    return true;
}
