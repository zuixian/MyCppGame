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
    
    float initialSpeed = 0;
    float speed = 0;
    int spCount = 0;
    cocos2d::Rect* rect;
    
public:
    
    static float gravitySpeed;
    static int hitSpeed;

    static Hero* create(const std::string &filename);
    
    void keepMove();
    
    void setInitialSpeed(float s);

    CREATE_FUNC(Hero);
};

#endif /* Hero_h */
