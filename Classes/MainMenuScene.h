#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include <unordered_set>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <ctime>       /* time */
using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

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
    
    void resetEdge(float dt);
    
    PhysicsBody *edgeBody;
    
    Node *edgeNode;
    
    Button *replay;
    
    PhysicsBody *replayEdge;
    
    Button *mute;
    
    PhysicsBody *buttonEdge;
    
    unordered_set<Sprite*> cells;
    
    Sprite *movingSprite = nullptr;
    
    void selectSpriteForTouch(Point touchLocation);
    
    void updateMovingSpritePosition(Vec2 p);
    
    MenuItemFont *menu_item_1;
    
    MenuItemFont *menu_item_2;
};

#endif 
