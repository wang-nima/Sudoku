#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "sudokuentity.h"
#include "numberSprite.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"
#include <cmath>
#include <vector>
#include "unordered_set"


class GameScene: public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
private:
    void setPhysicsWorld(cocos2d::PhysicsWorld *world) {
        this->world = world;
    }
    cocos2d::PhysicsWorld *world;
    
    SudokuEntity game;
    
    int state[9][9];
    
    numberSprite *movingSprite;
    
    vector<numberSprite*> moveAbleCell;     // include cell in the left and in board
    
    std::vector<cocos2d::Point> initPosition;
    
    cocos2d::Point boardTopLeft;
    
    int cellLength;
    
    Sprite *board;
    
    void selectSpriteForTouch(cocos2d::Point touchLocation);
    
    void updateMovingSpritePosition(Vec2 p);
    
    void adjustPosition(cocos2d::Point locationBeforeAdjust);
    
    void addNewNumberCell(int num);         // create number cell num at left side
    
    int emptyCellinBoardCount;
    
    bool checkGameBoard();
    
    void resetBoard();
    
    Label *label;
    
    bool musicPlaying;
    
    cocos2d::ui::Button *replay;
    
    //PhysicsBody *drop;
    
    void showAnswer();
    
    void share();
    
    bool answerShowing = false;
    
    std::unordered_set<Label*> ans;            // for delete ans
    
    cocos2d::ui::Button *goBackToMainMenu;
};

#endif // __HELLOWORLD_SCENE_H__
