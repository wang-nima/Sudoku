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
    
    auto label = Label::createWithTTF("Enjor your Sudoku!", "Naughty Cartoons.ttf", 40);
    label->setPosition(Point(visibleSize.width / 2,
                             visibleSize.height - 100));
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    
    movingSprite = nullptr;
    
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
        movingSprite = nullptr;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    game.regenerate(2);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            state[i][j] = game.startStatus[i][j];
        }
    }
    
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
        auto numberSprite = Sprite::create(std::to_string(i) + ".png");
        numberSprite->setPosition(boardTopLeft.x / 2,
                                  visibleSize.height - visibleSize.height / 10 * i);
        this->addChild(numberSprite);
        v.push_back(numberSprite);
    }
    
    return true;
}

void GameScene::selectSpriteForTouch(Point p) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i]->getBoundingBox().containsPoint(p)) {
            movingNumber = i + 1;
            movingSprite = v[i];
            //log("%d touched", i + 1);
        }
    }
}

void GameScene::updateMovingSpritePosition(Vec2 p) {
    if (movingSprite != nullptr) {
        Point oldPosition = movingSprite->getPosition();
        movingSprite->setPosition(oldPosition + p);
    }
}
