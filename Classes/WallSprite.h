//
//  WallSprite.h
//  MyCppGame
//
//  Created by zuixian on 2018/2/28.
//
#include "cocos2d.h"
#ifndef WallSprite_h
#define WallSprite_h
#define WALL_SCALE 0.2

class Wall : public cocos2d::Sprite{

private:

    float wallWidth;

    float wallTopMinY;

    float wallBottomMaxY;
    
    cocos2d::Rect* tRect;

    cocos2d::Rect* bRect;

    Sprite* cloneWall(Sprite* wall,float posY,int direction);

public:

    static float lastOffset;

    //Y轴相对于中点的偏移量
    void initWall();

    float getRandomOffset();

    void setWallPositionX(float x);

    void setWallLeftPositionX(float x);

    float getWallLeftPositionX();

    float getWallRightPositionX();

    cocos2d::Rect* getWallTopRect();

    cocos2d::Rect* getWallBottomRect();

    void deleteWall();

    CREATE_FUNC(Wall);
};

#endif /* WallSprite_h */
