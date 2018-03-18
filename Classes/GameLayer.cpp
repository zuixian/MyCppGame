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

    this->bgSpeed = (VISIBLE_SIZE.width)/SPRAMECOUNT;

    printf("visibleSize--->%f,%f\n",VISIBLE_SIZE.width,VISIBLE_SIZE.height);

    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
    float x = VISIBLE_SIZE.width - closeItem->getContentSize().width/2;
    float y = closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));
    
    restartItem = MenuItemImage::create("51.png","51.png",
                                        CC_CALLBACK_1(GameLayer::menuRestartCallback, this));
    restartItem->setPosition(Vec2(VISIBLE_SIZE.width/2,VISIBLE_SIZE.height/2));
    restartItem->setScale(0.1);
    restartItem->setVisible(false);
    restartItem->setEnabled(false);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem,restartItem, NULL);
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

    //创建人物
    auto sprite = Hero::create("Hero.png");
    sprite->setPosition(Vec2(VISIBLE_SIZE.width/2, VISIBLE_SIZE.height/2 ));
    this->addChild(sprite, 0, 0);
    
    this->schedule(schedule_selector(GameLayer::heroMove));
    this->schedule(schedule_selector(GameLayer::wallMove));
    this->schedule(schedule_selector(GameLayer::checkOver));
    
    return true;
}

void GameLayer::reset(){
    //背景图片重置
    auto bg = this->getChildByTag(1);
    auto bg1 = this->getChildByTag(2);

    bg->setPosition(Vec2(0, VISIBLE_SIZE.height/2));
    bg1->setPosition(Vec2(VISIBLE_SIZE.width, VISIBLE_SIZE.height/2));

    //墙体重置
    if(this->wallList.size() != 0 ){
        for(int i = 0; i < this->wallList.size(); ++i){
            Wall* wall = this->wallList.at(i);
            if(wall == NULL){
                continue;
            }
            wall->deleteWall();
            wall->removeFromParent();
            this->wallList.erase(i);
        }
    }

    for(int i = 0; i < this->wallPool.size(); ++i){
        auto wall = this->wallPool.top();
        this->wallPool.pop();
        wall->removeFromParent();
    }

    for(int i = 0; i < MAX_WALL_COUNT; ++i){
        Wall* wall = Wall::create();
        this->addChild(wall);
        this->wallPool.push(wall);
    }

    //人物重置
    auto hero = static_cast<Hero *>(getChildByTag(0));
    hero->setPosition(Vec2(VISIBLE_SIZE.width/2, VISIBLE_SIZE.height/2 ));
    hero->setInitialSpeed(0);

    //激活键盘事件
    listener->setEnabled(true);

    //将开始按钮隐藏
    restartItem->setEnabled(false);
    restartItem->setVisible(false);

    //启动游戏循环
    this->schedule(schedule_selector(GameLayer::heroMove));
    this->schedule(schedule_selector(GameLayer::wallMove));
    this->schedule(schedule_selector(GameLayer::checkOver));
}

void GameLayer::onEnter(){
    Layer::onEnter();
    printf("GameLayer onEnter");
    
    listener = EventListenerKeyboard::create();
    
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
        if(wall->getWallLeftPositionX() < hero->getPositionX() + hero->getContentSize().width/2
        && wall->getWallRightPositionX() > hero->getPositionX() - hero->getContentSize().width/2)
            isOver = GameLayer::isHeroAndWall(wall,hero);
        if(isOver) break;
    }

    //如果英雄触底，则游戏结束。
    if(hero->getPositionY() <= (VISIBLE_SIZE.height)/7 || isOver){
        this->unschedule(schedule_selector(GameLayer::heroMove));
        this->unschedule(schedule_selector(GameLayer::wallMove));
        this->unschedule(schedule_selector(GameLayer::checkOver));
        listener->setEnabled(false);
        restartItem->setVisible(true);
        restartItem->setEnabled(true);
    }
}

bool GameLayer::isHeroAndWall(Wall* wall,Hero* hero){
    bool result = false;
    float wty = wall->getWallTopPositionY();
    float wby = wall->getWallBottomPositionY();
    float hty = hero->getPositionY() + hero->getContentSize().height/2 - 2;
    float hby = hero->getPositionY() - hero->getContentSize().height/2 + 2;
    if(hty > wty || hby < wby)
        result = true;
    return result;
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    printf("onTouchEnded");
}

void GameLayer::menuRestartCallback(Ref* pSender)
{
    GameLayer::reset();
}

void GameLayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
