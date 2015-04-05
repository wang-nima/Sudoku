#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include <unordered_set>

class MainMenuScene: public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainMenuScene);
private:
    void GoToGameScene (cocos2d::Ref *sender);
    bool musicPlaying = true;
    cocos2d::PhysicsWorld *world;
    void setPhysicsWorld(cocos2d::PhysicsWorld *world) {
        this->world = world;
    }
    void drop(float dt);
};

#endif 
