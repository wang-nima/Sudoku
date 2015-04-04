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
    auto boardPosition = board->getPosition();
    Point boardTopLeft(boardPosition.x - boardSize.width / 2,
                       boardPosition.y + boardSize.height / 2);
    int cellLength = boardSize.height / 9;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int temp = game.startStatus[i][j];
            if (temp != 0) {
                auto fixedCell = Sprite::create(std::to_string(temp) + ".png");
                fixedCell->setAnchorPoint(Vec2(0, 1));
                fixedCell->setPosition(boardTopLeft.x + j * cellLength, boardTopLeft.y - i * cellLength);
                this->addChild(fixedCell);
            }
        }
    }
    
    
    
    
    return true;
}
