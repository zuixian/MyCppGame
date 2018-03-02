//
//  WallSprite.cpp
//  MyCppGame
//
//  Created by zuixian on 2018/2/28.
//

#include "WallSprite.h"

USING_NS_CC;

float Wall::scale = 0.2;

void Wall::initWall(){

    Size size = Director::getInstance()->getWinSize();  

    auto wallHead = Sprite::create("bar2.png");
    auto wallBody = Sprite::create("bar1.png");
    

    Size headSize = wallHead->getContentSize();
    Size bodySize = wallBody->getContentSize();


    Sprite *tempWallBody = NULL;
    Sprite *tempWallHead = NULL;
    //绘画上部分的墙体

    //设置Y锚点放下方
    wallHead->setAnchorPoint(Vec2(0.5,0));
    wallBody->setAnchorPoint(Vec2(0.5,0));

    //上下墙的空隙为 50
    float tempPos = 100.0;
    wallHead->setPositionY(tempPos);
    //翻转图片
    wallHead->setScale(scale,-scale);
    this->addChild(wallHead);
    // tempPos = tempPos + headSize.height*scale;
    printf("上部分的第一个：：%f",tempPos);
    wallBody->setPositionY(tempPos);
    wallBody->setScale(scale);
    this->addChild(wallBody);
    tempPos = tempPos + bodySize.height*scale - 1;
    while(tempPos < size.height/2){
        tempWallBody = Wall::cloneWall(wallBody,tempPos,0);
        this->addChild(tempWallBody);
        tempPos += bodySize.height*scale - 1;
    }

    //绘画下部分的墙体

    //设置Y锚点放上方
    tempPos = - 100.0;
    printf("临时位置：%f\n",tempPos);
    tempWallHead = Wall::cloneWall(wallHead,tempPos,1);
    printf("下部分头部位置--X：：%f--Y：：%f\n",tempWallHead->getPosition().x,tempWallHead->getPosition().y);
    this->addChild(tempWallHead);
    tempPos -= headSize.height*scale;
    tempWallBody = Wall::cloneWall(wallBody,tempPos,1);
    this->addChild(tempWallBody);
    tempPos -= bodySize.height*scale - 1;
    while(tempPos > -size.height/2){
        tempWallBody = Wall::cloneWall(wallBody,tempPos,1);
        this->addChild(tempWallBody);
        tempPos -= bodySize.height*scale - 1;
    }
};


Sprite* Wall::cloneWall(Sprite* wall,float posY,int direction){
    Sprite* tempWall = Sprite::createWithSpriteFrame(wall->getSpriteFrame());
    tempWall->setAnchorPoint(Vec2(0.5,direction));
    tempWall->setScale(scale);
    tempWall->setPositionY(posY);
    return tempWall;
}