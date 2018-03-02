//
//  WallSprite.h
//  MyCppGame
//
//  Created by zuixian on 2018/2/28.
//
#include "cocos2d.h"
#ifndef WallSprite_h
#define WallSprite_h

class Wall : public cocos2d::Sprite{

private:

    Sprite* cloneWall(Sprite* wall,float posY,int direction);

public:

    static float scale;

    //Y轴相对于中点的偏移量
    void initWall(float offset);

    void setWallPositionX(float x);

    void deleteWall();

    CREATE_FUNC(Wall);
};

#endif /* WallSprite_h */
