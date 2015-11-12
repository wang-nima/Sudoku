#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"
#include <CCUserDefault.h>

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
    
// music related code
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("mc.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mc.mp3", true);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("close1.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("close2.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("click.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Droplet.wav");
    
    
// menu buttons
    MenuItemFont::setFontSize(80);
    menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    menu_item_1->setColor(Color3B::BLACK);
    menu_item_1->setPosition(visibleSize.width / 2, visibleSize.height / 11 * 6 );
    
    menu_item_2 = MenuItemFont::create(map[difficulty]);
    menu_item_2->setColor(Color3B::BLACK);
    menu_item_2->setPosition(visibleSize.width / 2, visibleSize.height / 11 * 5 );
    
    auto menu = Menu::create(menu_item_1, menu_item_2, nullptr);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);
    
// select difficulty button
    
    difficulty = 0;
    UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
    
    auto left = Button::create("left.png");
    left->setPosition( Point(visibleSize.width / 2 - 200, visibleSize.height / 11 * 5) );
    
    left->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::BEGAN) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Droplet.wav");
            difficulty--;
            if (difficulty == -1) {
                difficulty = 4;
            }
            UserDefault::getInstance()->setIntegerForKey("difficulty", difficulty);
            menu_item_2->setString(map[difficulty]);
        }
    });
    this->addChild(left);

    auto right = Button::create("right.png");
    right->setPosition( Point(visibleSize.width / 2 + 200, visibleSize.height / 11 * 5) );
    
    right->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::BEGAN) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Droplet.wav");
            difficulty = ( difficulty + 1 ) % 5;
            UserDefault::getInstance()->setIntegerForKey("difficulty", difficulty);
            menu_item_2->setString(map[difficulty]);
        }
    });
    this->addChild(right);
    
// set up label
    
    auto title = Label::createWithTTF("Sudoku", "Naughty Cartoons.ttf", 156);
    title->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 300);
    title->setColor(Color3B::BLACK);
    this->addChild(title);
    
    
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
                CCLOG("mute button pressed");
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
                mute->loadTextureNormal("unmute.png");
                musicPlaying = false;
            } else {
                CCLOG("unmute button pressed");
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
        if (type == ui::Widget::TouchEventType::BEGAN) {
            CCLOG("replay button pressed");
            edgeNode->setPhysicsBody(nullptr);
            replay->setPhysicsBody(nullptr);
            mute->setPhysicsBody(nullptr);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("close1.wav");
            this->schedule(schedule_selector(MainMenuScene::resetEdge), 1.0f, 0, 6);
        }
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
            Point touchLocation = touch->getLocation();
            Point oldTouchLocation = touch->getPreviousLocation();
            Vec2 translation = touchLocation - oldTouchLocation;
            this->updateMovingSpritePosition(translation);
        }
    };
    touchListener->onTouchEnded = [&] (Touch* touch, Event* event) -> void {
        if (movingSprite) {
            movingSprite = nullptr;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void MainMenuScene::GoToGameScene (cocos2d::Ref *sender) {
    auto scene = GameScene::createScene();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
    //CCLOG("Main menu scene %d", difficulty);
    CCLOG("Main menu scene %d", UserDefault::getInstance()->getIntegerForKey("difficulty"));
    //Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
    Director::getInstance()->pushScene( TransitionSlideInR::create(TRANSITION_TIME, scene));
}

// random drop number cell
void MainMenuScene::drop(float dt)
{
    int start_x_pos = rand() % WIDTH;
    int number = rand() % 8 + 1;
    auto number_sprite = Sprite::create(std::to_string(number) + ".png");
    number_sprite->setPosition(start_x_pos, HEIGHT - 50);
    auto spriteBody = PhysicsBody::createBox( number_sprite->getContentSize(), PhysicsMaterial( 1, 0.5, 0 ) );
    number_sprite->setPhysicsBody( spriteBody );
    cells.insert(number_sprite);
    this->addChild(number_sprite);
}

// called when replay button pressed
void MainMenuScene::resetEdge(float dt) {
    replay->setPhysicsBody(replayEdge);
    mute->setPhysicsBody(buttonEdge);
    edgeNode->setPhysicsBody(edgeBody);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("close2.wav");
    cells.clear();
}

// for onTouchBegan method, set movingSprite
void MainMenuScene::selectSpriteForTouch(Point p) {
    for (auto it = cells.begin(); it != cells.end(); ++it) {
        if ((*it)->getBoundingBox().containsPoint(p)) {
            movingSprite = *it;
            return;
        }
    }
}

// for onTouchMoved
void MainMenuScene::updateMovingSpritePosition(Vec2 p) {
    Point oldPosition = movingSprite->getPosition();
    movingSprite->setPosition(oldPosition + p);
}