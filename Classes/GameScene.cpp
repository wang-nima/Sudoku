#include "GameScene.h"
#include "Definitions.h"
#include <cmath>
#include <vector>

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
    
// add label
    auto label = Label::createWithTTF("Enjor your Sudoku!", "Naughty Cartoons.ttf", 40);
    label->setPosition(Point(visibleSize.width / 2,
                             visibleSize.height - 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    
// initalize some class member
    movingSprite = nullptr;
    game.regenerate(2);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            state[i][j] = game.startStatus[i][j];
        }
    }
    
// add game board
    auto board = Sprite::create("board.png");
    board->setPosition(center);
    this->addChild(board);
    
// calculate sizes and positions
    auto boardSize = board->getContentSize();
    auto boardPosition = board->getPosition();
    boardTopLeft.x = boardPosition.x - boardSize.width / 2;
    boardTopLeft.y = boardPosition.y + boardSize.height / 2;
    cellLength = boardSize.height / 9;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int temp = game.startStatus[i][j];
            if (temp != 0) {
                auto fixedCell = Label::createWithTTF(std::to_string(temp), "squarefont.ttf", 50);
                fixedCell->setColor(Color3B::BLACK);
                fixedCell->setAnchorPoint(Vec2(0.5, 0.5));
                fixedCell->setPosition(boardTopLeft.x + ( j + 0.5 ) * cellLength,
                                       boardTopLeft.y - ( i + 0.5 ) * cellLength);
                this->addChild(fixedCell);
            }
        }
    }
    
    for (int i = 1; i <= 9 ; i++) {
        numberSprite *numberCell = numberSprite::create(std::to_string(i) + ".png");
        numberCell->num = i;
        numberCell->inBoard = false;
        initPosition.push_back(Point(boardTopLeft.x / 2,
                                     visibleSize.height - visibleSize.height / 10 * i));
        numberCell->setPosition(initPosition[i-1]);
        this->addChild(numberCell);
        v.push_back(numberCell);
    }
    
// set up touch event listener using lambda, c++11 feature
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        Point touchPosition = touch->getLocation();
        selectSpriteForTouch(touchPosition);
        return true;
    };
    touchListener->onTouchMoved = [&] (Touch* touch, Event* event) -> void {
        Point touchLocation = touch->getLocation();
        Point oldTouchLocation = touch->getPreviousLocation();
        Vec2 translation = touchLocation - oldTouchLocation;
        this->updateMovingSpritePosition(translation);
    };
    touchListener->onTouchEnded = [&] (Touch* touch, Event* event) -> void {
        Point touchLocation = touch->getLocation();
        adjustPosition(touchLocation);
        movingSprite = nullptr;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void GameScene::selectSpriteForTouch(Point p) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i]->getBoundingBox().containsPoint(p)) {
            movingSprite = v[i];
        }
    }
}

void GameScene::updateMovingSpritePosition(Vec2 p) {
    if (movingSprite != nullptr) {
        Point oldPosition = movingSprite->getPosition();
        movingSprite->setPosition(oldPosition + p);
    }
}

void GameScene::adjustPosition(Point locationBeforeAdjust) {
    int x = locationBeforeAdjust.x;
    int y = locationBeforeAdjust.y;
    int top_left_x = boardTopLeft.x;
    int top_left_y = boardTopLeft.y;
    int new_x = ( (x - top_left_x) / cellLength + 0.5 ) * cellLength + top_left_x;
    int new_y = top_left_y - ( (top_left_y - y) / cellLength + 0.5 ) * cellLength;
    //CCLOG("%d %d", x, y);
    //CCLOG("%d %d", boardTopLeft.x, boardTopLeft.y);
    //CCLOG("%d %d", top_left_x, top_left_y);
    //CCLOG("adjust function %d %d", new_x, new_y);
    movingSprite->setPosition(Point(new_x, new_y));
}
