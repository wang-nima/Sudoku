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
// make background white
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
    game.regenerate(0);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            state[i][j] = game.startStatus[i][j];
        }
    }
    
// add game board
    board = Sprite::create("board.png");
    board->setPosition(center);
    this->addChild(board);
    
// calculate sizes and positions
    auto boardSize = board->getContentSize();
    auto boardPosition = board->getPosition();
    boardTopLeft.x = boardPosition.x - boardSize.width / 2;
    boardTopLeft.y = boardPosition.y + boardSize.height / 2;
    cellLength = boardSize.height / 9;

// add number in fix cell
    
    emptyCellinBoardCount = 0;
    
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
            } else {
                emptyCellinBoardCount++;
            }
        }
    }
    
// create candidate number cell
    for (int i = 1; i <= 9 ; i++) {
        numberSprite *numberCell = numberSprite::create(to_string(i) + ".png");
        numberCell->num = i;
        numberCell->inBoard = false;
        initPosition.push_back(Point(boardTopLeft.x / 2,
                                     visibleSize.height - visibleSize.height / 10 * i));
        numberCell->setPosition(initPosition[i-1]);
        this->addChild(numberCell);
        moveAbleCell.push_back(numberCell);
    }
    
// set up touch event listener using lambda expression, c++11 feature
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        Point touchPosition = touch->getLocation();
        selectSpriteForTouch(touchPosition);
        if (movingSprite) {
            if (movingSprite->inBoard == true) {
                state[movingSprite->currentRow][movingSprite->currentColumn] = 0;
                emptyCellinBoardCount++;
            }
        }
        return true;
    };
    touchListener->onTouchMoved = [&] (Touch* touch, Event* event) -> void {
        if (movingSprite) {
            Point touchLocation = touch->getLocation();
            Point oldTouchLocation = touch->getPreviousLocation();
            Vec2 translation = touchLocation - oldTouchLocation;
            this->updateMovingSpritePosition(translation);
        }
    };
    touchListener->onTouchEnded = [&] (Touch* touch, Event* event) -> void {
        if (movingSprite) {
            Point touchLocation = touch->getLocation();
            adjustPosition(touchLocation);
            movingSprite = nullptr;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

// for onTouchBegan method, set movingSprite
void GameScene::selectSpriteForTouch(Point p) {
    for (int i = 0; i < moveAbleCell.size(); i++) {
        if (moveAbleCell[i]->getBoundingBox().containsPoint(p)) {
            movingSprite = moveAbleCell[i];
        }
    }
}

// for onTouchMoved
void GameScene::updateMovingSpritePosition(Vec2 p) {
    Point oldPosition = movingSprite->getPosition();
    movingSprite->setPosition(oldPosition + p);
}

// for onTouchEnd
void GameScene::adjustPosition(Point locationBeforeAdjust) {
    // move into boarrd cell
    if (board->getBoundingBox().containsPoint(locationBeforeAdjust)) {
        int x = locationBeforeAdjust.x;
        int y = locationBeforeAdjust.y;
        int top_left_x = boardTopLeft.x;
        int top_left_y = boardTopLeft.y;
        int row_count_x = ( x - top_left_x ) / cellLength;
        int row_count_y = ( top_left_y - y ) / cellLength;
        if (state[row_count_y][row_count_x] == 0) {
            int new_x = ( row_count_x + 0.5 ) * cellLength + top_left_x;
            int new_y = top_left_y - ( row_count_y + 0.5 ) * cellLength;
            
            state[row_count_y][row_count_x] = movingSprite->num;
            if (movingSprite->inBoard == false) {           // drag number cell from left into board
                addNewNumberCell(movingSprite->num);
            }
            movingSprite->inBoard = true;
            movingSprite->currentRow = row_count_y;
            movingSprite->currentColumn = row_count_x;
            emptyCellinBoardCount--;
            if (emptyCellinBoardCount == 0) {
                if (checkGameBoard()) {
                    CCLOG("success");
                } else {
                    CCLOG("some thing is not correct");
                }
            }
            auto action = MoveTo::create(0.2, Point(new_x, new_y));
            movingSprite->runAction(action);
            return;
        }
    }
    int num = movingSprite->num;
    movingSprite->inBoard = false;
    movingSprite->currentColumn = movingSprite->currentRow = -1;
    auto action = MoveTo::create(0.2, initPosition[num-1]);
    movingSprite->runAction(action);
}

void GameScene::addNewNumberCell(int num) {
    numberSprite *newNumCell = numberSprite::create(to_string(num) + ".png");
    newNumCell->setPosition(initPosition[num-1]);
    newNumCell->num = num;
    newNumCell->inBoard = false;
    newNumCell->currentRow = newNumCell->currentColumn = -1;
    moveAbleCell.push_back(newNumCell);
    this->addChild(newNumCell);
}

bool GameScene::checkGameBoard() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game.endStatus[i][j] != state[i][j]) {
                return false;
            }
        }
    }
    return true;
}
