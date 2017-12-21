//
//  GameLayer.cpp
//  MyCppGame
//
//  Created by zuixian on 2017/12/17.
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Hero.h"

USING_NS_CC;

bool GameLayer::init(){
    
    //初始化父类
    if(!Layer::init()){
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
    float y = origin.y + closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    //添加背景图片
    auto bg = Sprite::create("bg.png");
    bg->setContentSize(visibleSize);
    bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(bg);
    
    // add "HelloWorld" splash screen"
    auto sprite = Hero::create("Hero.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0, 0);
    
    this->schedule(schedule_selector(GameLayer::heroMove));
    
    return true;
}

void GameLayer::onEnter(){
    Layer::onEnter();
    printf("GameLayer onEnter");
    
    auto listener = EventListenerKeyboard::create();
    
    auto hero = getChildByTag(0);
    
    listener->onKeyReleased = [](EventKeyboard::KeyCode keyCode,Event* event){
        auto target = static_cast<Hero *>(event->getCurrentTarget());
        auto pos = target->getPosition();
        printf("Key with keycode %d released\n",keyCode);
        printf("%d",EventKeyboard::KeyCode::KEY_SPACE);
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_SPACE:
                target->setInitialSpeed(20);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                target->setPosition(Vec2(pos.x,pos.y+20));
                printf("pos %f,%f\n",pos.x,pos.y);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                target->setPosition(Vec2(pos.x,pos.y-20));
                printf("pos %f,%f\n",pos.x,pos.y);
                break;
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                target->setPosition(Vec2(pos.x-20,pos.y));
                printf("pos %f,%f\n",pos.x,pos.y);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                target->setPosition(Vec2(pos.x+20,pos.y));
                printf("pos %f,%f\n",pos.x,pos.y);
                break;
        }
    };
    
    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, hero);
}

void GameLayer::heroMove(float dt){
    auto hero = dynamic_cast<Hero*>(this->getChildByTag(0));
    hero->keepMove();
}

void GameLayer::onExit(){
    Layer::onExit();
    printf("GameLyer onExit");
    
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    printf("onTouchEnded");
}

void GameLayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
}
