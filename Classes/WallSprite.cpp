//
//  WallSprite.cpp
//  MyCppGame
//
//  Created by zuixian on 2018/2/28.
//

#include "WallSprite.h"

USING_NS_CC;

float Wall::lastOffset = 0;

void Wall::initWall(){

    this->tRect = new cocos2d::Rect();
    this->bRect = new cocos2d::Rect();

    float offset = Wall::getRandomOffset();

    Size size = Director::getInstance()->getWinSize();  

    this->setPositionY(size.height/2+offset);

    auto wallHead = Sprite::create("bar2.png");
    auto wallBody = Sprite::create("bar1.png");
    

    Size headSize = wallHead->getContentSize();
    Size bodySize = wallBody->getContentSize();

    this->wallWidth = headSize.width;

    Sprite *tempWallBody = NULL;
    Sprite *tempWallHead = NULL;
    //绘画上部分的墙体

    //设置Y锚点放下方
    wallHead->setAnchorPoint(Vec2(0.5,0));
    wallBody->setAnchorPoint(Vec2(0.5,0));

    this->wallTopMinY = size.height/2+offset + 120;
    this->wallBottomMaxY = size.height/2+offset - 120;

    //上下墙的空隙为 240
    float tempPos = 120.0;
    wallHead->setPositionY(tempPos);
    //翻转图片
    wallHead->setScale(WALL_SCALE,-WALL_SCALE);
    this->addChild(wallHead);
    // printf("上部分的第一个：：%f",tempPos);
    wallBody->setPositionY(tempPos);
    wallBody->setScale(WALL_SCALE);
    this->addChild(wallBody);
    tempPos = tempPos + bodySize.height*WALL_SCALE - 1;
    while(tempPos < size.height/2-offset){
        tempWallBody = Wall::cloneWall(wallBody,tempPos,0);
        this->addChild(tempWallBody);
        tempPos += bodySize.height*WALL_SCALE - 1;
    }

    //绘画下部分的墙体

    tempPos = - 120.0;
    // printf("临时位置：%f\n",tempPos);
    tempWallHead = Wall::cloneWall(wallHead,tempPos,1);
    // printf("下部分头部位置--X：：%f--Y：：%f\n",tempWallHead->getPosition().x,tempWallHead->getPosition().y);
    this->addChild(tempWallHead);
    tempPos -= headSize.height*WALL_SCALE;
    tempWallBody = Wall::cloneWall(wallBody,tempPos,1);
    this->addChild(tempWallBody);
    tempPos -= bodySize.height*WALL_SCALE - 1;
    while(tempPos > -size.height/2-offset){
        tempWallBody = Wall::cloneWall(wallBody,tempPos,1);
        this->addChild(tempWallBody);
        tempPos -= bodySize.height*WALL_SCALE - 1;
    }
};


Sprite* Wall::cloneWall(Sprite* wall,float posY,int direction){
    Sprite* tempWall = Sprite::createWithSpriteFrame(wall->getSpriteFrame());
    //根据墙的位置来决定锚点的位置。上部分的墙锚点为0，下部分的墙锚点为1.方便定位。
    tempWall->setAnchorPoint(Vec2(0.5,direction));
    tempWall->setScale(WALL_SCALE);
    tempWall->setPositionY(posY);
    return tempWall;
}

float Wall::getRandomOffset(){
    Size size = Director::getInstance()->getWinSize(); 
    srand ((unsigned)time(nullptr));
    int sign = rand() % 2 + 1;
    int num = rand() % 100 + 1;
    num = sign == 1 ? num : -num;
    float offset = abs(lastOffset + num);
    while(offset > size.height*2/3){
        sign = rand() % 2 + 1;
        num = rand() % 100 + 1;
        num = sign == 1 ? num : -num;
        offset = abs(lastOffset + num);
    }
    return lastOffset + num;
}

void Wall::setWallPositionX(float x){
    this->setPositionX(x);
}

void Wall::setWallLeftPositionX(float x){
    this->setPositionX(x+this->wallWidth/2);
}

float Wall::getWallLeftPositionX(){
    float p = this->getPositionX() - this->wallWidth/2;
    return p;
}

float Wall::getWallRightPositionX(){
    float p = this->getPositionX() + this->wallWidth/2;
    return p;
}

cocos2d::Rect* Wall::getWallTopRect(){

    Size size = Director::getInstance()->getWinSize();  

    float tMinX = Wall::getWallLeftPositionX();
    float tMinY = this->wallTopMinY;
    float width = this->wallWidth;
    float height = size.height - tMinY;

    this->tRect->setRect(tMinX,tMinY,width,height);

    return this->tRect;
}

cocos2d::Rect* Wall::getWallBottomRect(){

    float tMinX = Wall::getWallLeftPositionX();
    float tMinY = 0;
    float width = this->wallWidth;
    float height = this->wallBottomMaxY;

    this->tRect->setRect(tMinX,tMinY,width,height);

    return this->bRect;
}

void Wall::deleteWall(){
    this->removeAllChildren();
}