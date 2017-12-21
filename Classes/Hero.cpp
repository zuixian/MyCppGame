//
//  Hero.cpp
//  MyCppGame
//
//  Created by zuixian on 2017/12/20.
//

#include "Hero.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

float Hero::gravitySpeed = -0.098;
int Hero::hitSpeed = 5;

Hero* Hero::create(const std::string &filename){
    Hero* hero = new Hero();
    
    if(hero && hero->initWithFile(filename)){
        hero->autorelease();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

void Hero::keepMove(){
    // spCount++;
    // if(spCount == 10){
    //     spCount = 0;
    //     initialSpeed = 0;
    // }
    auto posY = this->getPositionY();
    initialSpeed += gravitySpeed;
    speed = initialSpeed;
    this->setPositionY(posY + speed);
};

void Hero::setInitialSpeed(float s){
    initialSpeed = s;
};
