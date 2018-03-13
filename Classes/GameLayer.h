//
//  GameLayer.h
//  MyCppGame
//
//  Created by zuixian on 2017/12/17.
//

#include "cocos2d.h"
#include "WallSprite.h"
#include <stack>

#ifndef GameLayer_h
#define GameLayer_h

#define MAX_WALL_COUNT 10
#define SPRAMECOUNT 240
#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()

class GameLayer : public cocos2d::Layer {

private:
    float bgSpeed;

    int wallTime;

    // std::stack<Wall*> wallList;
    std::stack<Wall*> wallPool;
    cocos2d::Vector<Wall*> wallList;
    // cocos2d::Vector<Wall*> wallPool;
    
public:

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    
    virtual void onEnter();
    virtual void onExit();
    
    void heroMove(float dt);
    void wallMove(float dt);
    void checkOver(float dt);

    bool isHeroAndWall(Wall* wall,Hero* hero);
    
    CREATE_FUNC(GameLayer);
};

#endif /* GameLayer_h */
