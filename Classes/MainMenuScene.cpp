#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if ( !LayerColor::initWithColor(Color4B::WHITE)) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    menu_item_1->setColor(Color3B::BLACK);
    menu_item_1->setFontSize(100);
    
    auto menu = Menu::create(menu_item_1, nullptr);
    menu->setPosition( Point(visibleSize.width / 2,
                             visibleSize.height / 2) );
    this->addChild(menu);
    
    auto title = Label::createWithTTF("Sudoku", "Naughty Cartoons.ttf", 156);
    title->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 300);
    title->setColor(Color3B::BLACK);
    this->addChild(title);
    
    
    return true;
}

void MainMenuScene::GoToGameScene (cocos2d::Ref *sender) {
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}