//
//  WallSprite.cpp
//  MyCppGame
//
//  Created by zuixian on 2018/2/28.
//

#include "WallSprite.h"

USING_NS_CC;

float Wall::scale = 0.5;

void Wall::initWall(float position){

    Size size = Director::getInstance()->getWinSize();  

    auto wallHead = Sprite::create("bar2.png");
    auto wallBody = Sprite::create("bar1.png");
    

    Size headSize = wallHead->getContentSize();
    Size bodySize = wallBody->getContentSize();


    Sprite *tempWallBody = NULL;
    Sprite *tempWallHead = NULL;
    //绘画上部分的墙体

    //设置Y锚点放下方
    wallHead->setAnchorPoint(Point(0.5,0));
    wallBody->setAnchorPoint(Point(0.5,0));

    //上下墙的空隙为 50
    float tempPos = position - 25.0;
    wallHead->setPosition(size.width/2,tempPos);
    //翻转图片
    wallHead->setScale(0.5,-0.5);
    this->addChild(wallHead);
    tempPos = tempPos - headSize.height;
    wallBody->setPosition(size.width/2,tempPos);
    wallBody->setScale(0.5,0.5);
    this->addChild(wallBody);
    tempPos = tempPos - bodySize.height;
    while(tempPos > 0){
        tempWallBody = Wall::cloneWall(wallBody,size.width/2,tempPos);
        this->addChild(tempWallBody);
        tempPos -= bodySize.height;
    }

    //绘画下部分的墙体

    //设置Y锚点放上方
    wallHead->setAnchorPoint(Point(0.5,1));
    wallBody->setAnchorPoint(Point(0.5,1));

    tempPos = position + 25.0;
    tempWallHead = Wall::cloneWall(wallHead,size.width/2,tempPos);
    this->addChild(tempWallHead);
    tempPos += headSize.height;
    tempWallBody = Wall::cloneWall(wallBody,size.width/2,tempPos);
    this->addChild(tempWallBody);
    tempPos += bodySize.height;
    while(tempPos < size.height){
    tempWallHead->setPosition(size.width/2,tempPos);
        tempWallBody = Wall::cloneWall(wallBody,size.width/2,tempPos);
        this->addChild(tempWallBody);
        tempPos += bodySize.height;
    }
};


Sprite* Wall::cloneWall(Sprite* wall,float posX,float posY){
    Sprite* tempWall = Sprite::createWithSpriteFrame(wall->getSpriteFrame());
    tempWall->setPosition(posX,posY);
    tempWall->setScale(scale);
    return tempWall;
}