//
//  Hero.h
//  MyCppGame
//
//  Created by zuixian on 2017/12/20.
//
#include "cocos2d.h"

#ifndef Hero_h
#define Hero_h

class Hero : public cocos2d::Sprite {
    
private:
    
    float initialSpeed;
    float gravitySpeed;
    float speed;
    
public:
    virtual bool init();
    
    static Hero* create(const std::string &filename);
    
    void keepMove();
    
    void setInitialSpeed(float s);
    
    CREATE_FUNC(Hero);
};

#endif /* Hero_h */
