#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "sudokuentity.h"

class GameScene: public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    SudokuEntity game;
    int state[9][9];
};

#endif // __HELLOWORLD_SCENE_H__
