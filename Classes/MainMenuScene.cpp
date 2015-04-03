#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <ctime>       /* time */

USING_NS_CC;
using namespace cocos2d::ui;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -200));
    auto layer = MainMenuScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
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
    auto center = Point(visibleSize.width / 2, visibleSize.height / 2);
    CCLOG("%f %f",visibleSize.width, visibleSize.height);
    
    auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    menu_item_1->setColor(Color3B::BLACK);
    menu_item_1->setFontSize(100);
    
    auto menu = Menu::create(menu_item_1, nullptr);
    menu->setPosition(center);
    this->addChild(menu);
    
    auto title = Label::createWithTTF("Sudoku", "Naughty Cartoons.ttf", 156);
    title->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 300);
    title->setColor(Color3B::BLACK);
    this->addChild(title);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("mc.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mc.mp3");
    
    auto mute = Button::create("mute.png");
    mute->setPosition( Point(origin.x + visibleSize.width - 100,
                             origin.y + 100) );
    mute->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::BEGAN) {
            if (musicPlaying) {
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
                mute->loadTextureNormal("unmute.png");
                musicPlaying = false;
            } else {
                CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
                mute->loadTextureNormal("mute.png");
                musicPlaying = true;
            }    
        }
        
    });
    this->addChild(mute);
    
    //add animation
    
    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    auto edgeNode = Node::create();
    edgeNode->setPosition( center );
    edgeNode->setPhysicsBody( edgeBody );
    this->addChild( edgeNode );
    
    srand (time(0));
    this->schedule(schedule_selector(MainMenuScene::drop), 1.0f, CC_REPEAT_FOREVER, 0);
    
    
    return true;
}

void MainMenuScene::GoToGameScene (cocos2d::Ref *sender) {
    auto scene = GameScene::createScene();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}

void MainMenuScene::drop(float dt)
{
    int start_x_pos = rand() % WIDTH;
    int number = rand() % 8 + 1;
    auto number_sprite = Sprite::create(std::to_string(number) + ".png");
    number_sprite->setPosition(start_x_pos, HEIGHT - 50);
    auto spriteBody = PhysicsBody::createBox( number_sprite->getContentSize(), PhysicsMaterial( 1, 0.5, 0 ) );
    number_sprite->setPhysicsBody( spriteBody );
    this->addChild(number_sprite);
}