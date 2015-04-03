#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene: public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainMenuScene);
private:
    void GoToGameScene (cocos2d::Ref *sender);
    bool musicPlaying = true;

};

#endif 
