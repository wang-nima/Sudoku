#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "sudokuentity.h"
#include "numberSprite.h"

using namespace cocos2d;
using namespace std;

class GameScene: public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
private:
    SudokuEntity game;
    
    int state[9][9];
    
    numberSprite *movingSprite;
    
    vector<numberSprite*> moveAbleCell;     // include cell in the left and in board
    
    vector<Point> initPosition;
    
    Point boardTopLeft;
    
    int cellLength;
    
    Sprite *board;
    
    void selectSpriteForTouch(Point touchLocation);
    
    void updateMovingSpritePosition(Vec2 p);
    
    void adjustPosition(Point locationBeforeAdjust);
    
    void addNewNumberCell(int num);         // create number cell num at left side
    
    int emptyCellinBoardCount;
    
    bool checkGameBoard();
    
    Label *label;
};

#endif // __HELLOWORLD_SCENE_H__
