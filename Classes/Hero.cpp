//
//  Hero.cpp
//  MyCppGame
//
//  Created by zuixian on 2017/12/20.
//

#include "Hero.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Hero* Hero::create(const std::string &filename){
    Hero* hero = new Hero();
    if(hero && hero->initWithFile(filename)){
        hero->autorelease();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

bool Hero::init(){
    
    if(!Sprite::init()){
        return false;
    }
    
    gravitySpeed = 9.8;
    speed = 0;
    initialSpeed = 0;
    
    return true;
};

void Hero::keepMove(){
    auto posY = this->getPositionY();
    initialSpeed += gravitySpeed;
    speed += initialSpeed;
    this->setPositionY(posY + speed);
};

void Hero::setInitialSpeed(float s){
    initialSpeed = s;
};
