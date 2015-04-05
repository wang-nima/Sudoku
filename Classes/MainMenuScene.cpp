#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

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
    auto center = Point(visibleSize.width / 2, visibleSize.height / 2);
    
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
    
// music related code
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("mc.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mc.mp3", true);
    
    
    mute = Button::create("mute.png");
    mute->setPosition( Point(visibleSize.width - 100, 100) );
// protect the button not block by number rain
    buttonEdge = PhysicsBody::createEdgeBox(Size(mute->getVirtualRendererSize().width + 30,
                                                      mute->getVirtualRendererSize().height + 30));
    mute->setPhysicsBody(buttonEdge);
    
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
    
// set the bounding box for the hole screen
    edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    edgeNode = Node::create();
    edgeNode->setPosition( center );
    edgeNode->setPhysicsBody( edgeBody );
    this->addChild( edgeNode );
    
// refresh number rain
    replay = Button::create("replay.png");
    replay->setPosition( Point(visibleSize.width - 100, 250) );
    
// protect replay button
    replayEdge = PhysicsBody::createEdgeBox(Size(replay->getVirtualRendererSize().width + 30,
                                                      replay->getVirtualRendererSize().height + 30));
    replay->setPhysicsBody(replayEdge);
    
    replay->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        CCLOG("replay button pressed");
        edgeNode->setPhysicsBody(nullptr);
        replay->setPhysicsBody(nullptr);
        mute->setPhysicsBody(nullptr);
        //this->schedule(schedule_selector(MainMenuScene::drop), 1.0f, CC_REPEAT_FOREVER, 0);
    });
    this->addChild(replay);
    
// add animation, create bounding box
    
    
    srand (time(0));
    this->schedule(schedule_selector(MainMenuScene::drop), 1.0f, CC_REPEAT_FOREVER, 0);
    
    return true;
}

void MainMenuScene::GoToGameScene (cocos2d::Ref *sender) {
    auto scene = GameScene::createScene();
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
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