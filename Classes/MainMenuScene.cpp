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
    

    spriteBody = PhysicsBody::createBox( Sprite::create("1.png")->getContentSize(), PhysicsMaterial( 1, 0.5, 0 ) );
    spriteBody->retain();
    
// music related code
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("mc.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mc.mp3", true);
    
    
    mute = Button::create("mute.png");
    mute->setPosition( Point(visibleSize.width - 100, 100) );
// protect the button not block by number rain
    buttonEdge = PhysicsBody::createEdgeBox(Size(mute->getVirtualRendererSize().width + 30,
                                                 mute->getVirtualRendererSize().height + 30));
    buttonEdge->retain();
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
    edgeBody->retain();
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
// intentally increase ref count, when set nullptr will not be freed
    replayEdge->retain();
    replay->setPhysicsBody(replayEdge);
    
    replay->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        CCLOG("replay button pressed");
        edgeNode->setPhysicsBody(nullptr);
        replay->setPhysicsBody(nullptr);
        mute->setPhysicsBody(nullptr);
        this->schedule(schedule_selector(MainMenuScene::resetEdge), 1.0f, 0, 6);
    });
    this->addChild(replay);
    
// add animation, create bounding box
    
    srand ((unsigned int)time(0));
    this->schedule(schedule_selector(MainMenuScene::drop), 1.0f, CC_REPEAT_FOREVER, 0);

// handle touch for number cell
// set up touch event listener using lambda expression, c++11 feature
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        Point touchPosition = touch->getLocation();
        selectSpriteForTouch(touchPosition);
        return true;
    };
    touchListener->onTouchMoved = [&] (Touch* touch, Event* event) -> void {
        if (movingSprite) {
            movingSprite->setPhysicsBody(nullptr);
            Point touchLocation = touch->getLocation();
            Point oldTouchLocation = touch->getPreviousLocation();
            Vec2 translation = touchLocation - oldTouchLocation;
            this->updateMovingSpritePosition(translation);
        }
    };
    touchListener->onTouchEnded = [&] (Touch* touch, Event* event) -> void {
        if (movingSprite) {
            movingSprite->setPhysicsBody(spriteBody);
            movingSprite = nullptr;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void MainMenuScene::GoToGameScene (cocos2d::Ref *sender) {
    auto scene = GameScene::createScene();
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}

// random drop number cell
void MainMenuScene::drop(float dt)
{
    int start_x_pos = rand() % WIDTH;
    int number = rand() % 8 + 1;
    auto number_sprite = Sprite::create(std::to_string(number) + ".png");
    number_sprite->setPosition(start_x_pos, HEIGHT - 50);
    number_sprite->setPhysicsBody( spriteBody );
    cells.insert(number_sprite);
    this->addChild(number_sprite);
}

// called when replay button pressed
void MainMenuScene::resetEdge(float dt) {
    replay->setPhysicsBody(replayEdge);
    mute->setPhysicsBody(buttonEdge);
    edgeNode->setPhysicsBody(edgeBody);
// make searching fast by remove cell out of view
    cells.clear();
}

// for onTouchBegan method, set movingSprite
void MainMenuScene::selectSpriteForTouch(Point p) {
    for (auto it = cells.begin(); it != cells.end(); ++it) {
        if ((*it)->getBoundingBox().containsPoint(p)) {
            movingSprite = *it;
            break;
        }
    }
}

// for onTouchMoved
void MainMenuScene::updateMovingSpritePosition(Vec2 p) {
    Point oldPosition = movingSprite->getPosition();
    movingSprite->setPosition(oldPosition + p);
}