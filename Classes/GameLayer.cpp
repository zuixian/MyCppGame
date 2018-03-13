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

bool GameLayer::init(){
    
    //初始化父类
    if(!Layer::init()){
        return false;
    }

    this->wallTime = 0;

    this->wallList = Vector<Wall*>();
    // this->wallPool = Vector<Wall*>(MAX_WALL_COUNT);

    

    this->bgSpeed = (VISIBLE_SIZE.width)/SPRAMECOUNT;

    printf("visibleSize--->%f,%f\n",VISIBLE_SIZE.width,VISIBLE_SIZE.height);

    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
    float x = VISIBLE_SIZE.width - closeItem->getContentSize().width/2;
    float y = closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    //添加背景图片
    auto bg = Sprite::create("bg.png");
    bg->setContentSize(VISIBLE_SIZE);
    bg->setAnchorPoint(Vec2(0,0.5));
    bg->setPosition(Vec2(0, VISIBLE_SIZE.height/2));
    this->addChild(bg, 0, 1);

    auto bg1 = Sprite::create("bg.png");
    bg1->setContentSize(VISIBLE_SIZE);
    bg1->setAnchorPoint(Vec2(0,0.5));
    bg1->setPosition(Vec2(VISIBLE_SIZE.width, VISIBLE_SIZE.height/2));
    this->addChild(bg1, 0, 2);

    for(int i = 0; i < MAX_WALL_COUNT; ++i){
        Wall* wall = Wall::create();
        this->addChild(wall);
        this->wallPool.push(wall);
    }

    // auto testSprite = Sprite::create("bar1.png");
    // testSprite->setAnchorPoint(Vec2(0.5,1));
    // testSprite->setScale(0.2);
    // testSprite->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    // testSprite->setAnchorPoint(Vec2(0.5,0));
    // this->addChild(testSprite);
    

    // printf("墙体节点的坐标---X：：%f---Y：：%f\n",wall->getPositionX(),wall->getPositionY());
    
    //创建人物
    auto sprite = Hero::create("Hero.png");
    
    sprite->setPosition(Vec2(VISIBLE_SIZE.width/2, VISIBLE_SIZE.height/2 ));
    
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
    auto hero = dynamic_cast<Hero*>(this->getChildByTag(0));
    hero->keepMove();

    auto bg = this->getChildByTag(1);
    auto bg1 = this->getChildByTag(2);

    if(bg->getPositionX() <= -VISIBLE_SIZE.width){
        bg->setPositionX(VISIBLE_SIZE.width);
    }
    if(bg1->getPositionX() <= -VISIBLE_SIZE.width){
        bg1->setPositionX(VISIBLE_SIZE.width);
    }

    bg->setPositionX(bg->getPositionX() - this->bgSpeed);
    bg1->setPositionX(bg1->getPositionX() - this->bgSpeed);
}

void GameLayer::wallMove(float dt){
    this->wallTime++;
    if(this->wallTime > 70){
        this->wallTime = 0;
        Wall* wall = NULL;
        if(!this->wallPool.empty()){
            wall = this->wallPool.top();
            this->wallPool.pop();
            wall->initWall();
        }else{
            wall = Wall::create();
            wall->initWall();
        }
        wall->setWallLeftPositionX(VISIBLE_SIZE.width);
        this->wallList.pushBack(wall);
    }
    //创建墙体
    // auto wall = Wall::create();
    // wall->initWall(100);
    // 
    // this->addChild(wall);
    if(this->wallList.size() != 0 ){
        for(int i = 0; i < this->wallList.size(); ++i){
            Wall* wall = this->wallList.at(i);
            if(wall == NULL){
                continue;
            }
            if(wall->getWallRightPositionX() < 0){
                wall->deleteWall();
                this->wallPool.push(wall);
                this->wallList.erase(i);
            }else{
                wall->setWallPositionX(wall->getPositionX() - this->bgSpeed);
            }
        }
    }
}

void GameLayer::checkOver(float dt){

    auto hero = dynamic_cast<Hero*>(this->getChildByTag(0));
    
    bool isOver = false;
    for(int i = 0; i < this->wallList.size(); ++i){
        Wall* wall = this->wallList.at(i);
        isOver = GameLayer::isHeroAndWall(wall,hero);
        if(isOver) break;
    }

    //如果英雄触底，则游戏结束。
    if(hero->getPositionY() <= (VISIBLE_SIZE.height)/7 || isOver){
        this->unschedule(schedule_selector(GameLayer::heroMove));
        this->unschedule(schedule_selector(GameLayer::wallMove));
        this->unschedule(schedule_selector(GameLayer::checkOver));
    }
}

bool GameLayer::isHeroAndWall(Wall* wall,Hero* hero){
    bool result = false;
    float wlx = wall->getWallLeftPositionX();
    float wrx = wall->getWallRightPositionX();
    float wty = wall->getPositionY() + 120;
    float wby = wall->getPositionY() - 120;
    return result;
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
