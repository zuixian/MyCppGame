//
//  GameLayer.cpp
//  MyCppGame
//
//  Created by zuixian on 2017/12/17.
//

#include "GameLayer.h"
#include "Hero.h"
#include "WallSprite.h"

USING_NS_CC;

int GameLayer::sprameCount = 240;

bool GameLayer::init(){
    
    //初始化父类
    if(!Layer::init()){
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    printf("visibleSize--->%f,%f\n",visibleSize.width,visibleSize.height);
    printf("origin---->%f,%f\n",origin.x,origin.y);

    
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
    
    //添加背景图片
    auto bg = Sprite::create("bg.png");
    bg->setContentSize(visibleSize);
    bg->setAnchorPoint(Vec2(0,0.5));
    bg->setPosition(Vec2(0, visibleSize.height/2 + origin.y));
    this->addChild(bg, 0, 1);

    auto bg1 = Sprite::create("bg.png");
    bg1->setContentSize(visibleSize);
    bg1->setAnchorPoint(Vec2(0,0.5));
    bg1->setPosition(Vec2(visibleSize.width + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(bg1, 0, 2);

    bgSpeed = (visibleSize.width + origin.x)/sprameCount;

    

    // auto testSprite = Sprite::create("bar1.png");
    // testSprite->setAnchorPoint(Vec2(0.5,1));
    // testSprite->setScale(0.2);
    // testSprite->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    // testSprite->setAnchorPoint(Vec2(0.5,0));
    // this->addChild(testSprite);
    

    // printf("墙体节点的坐标---X：：%f---Y：：%f\n",wall->getPositionX(),wall->getPositionY());
    
    //创建人物
    auto sprite = Hero::create("Hero.png");
    
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(sprite, 0, 0);
    
    this->schedule(schedule_selector(GameLayer::heroMove));
    this->schedule(schedule_selector(GameLayer::wallMove));
    this->schedule(schedule_selector(GameLayer::checkOver));
    
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
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_SPACE:
                target->setInitialSpeed(Hero::hitSpeed);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                target->setPosition(Vec2(pos.x,pos.y+20));
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                target->setPosition(Vec2(pos.x,pos.y-20));
                break;
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                target->setPosition(Vec2(pos.x-20,pos.y));
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                target->setPosition(Vec2(pos.x+20,pos.y));
                break;
        }
    };
    
    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, hero);
}

void GameLayer::onExit(){
    Layer::onExit();
    printf("GameLyer onExit");
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void GameLayer::heroMove(float dt){
    //TODO 将这两个变量变成全局变量
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto hero = dynamic_cast<Hero*>(this->getChildByTag(0));
    hero->keepMove();

    auto bg = this->getChildByTag(1);
    auto bg1 = this->getChildByTag(2);

    if(bg->getPositionX() <= -visibleSize.width - origin.x){
        bg->setPositionX(visibleSize.width + origin.x);
    }
    if(bg1->getPositionX() <= -visibleSize.width - origin.x){
        bg1->setPositionX(visibleSize.width + origin.x);
    }

    bg->setPositionX(bg->getPositionX() - bgSpeed);
    bg1->setPositionX(bg1->getPositionX() - bgSpeed);
}

void GameLayer::wallMove(float dt){
    //创建墙体
    // auto wall = Wall::create();
    // wall->initWall(100);
    // wall->setWallPositionX(visibleSize.width/2 + origin.x);
    // this->addChild(wall);
}

void GameLayer::checkOver(float dt){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto hero = dynamic_cast<Hero*>(this->getChildByTag(0));

    //如果英雄触底，则游戏结束。
    if(hero->getPositionY() <= (visibleSize.height + origin.y)/7){
        this->unschedule(schedule_selector(GameLayer::heroMove));
    }
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
}