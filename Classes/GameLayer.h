//
//  GameLayer.h
//  MyCppGame
//
//  Created by zuixian on 2017/12/17.
//

#include "cocos2d.h"

#ifndef GameLayer_h
#define GameLayer_h

class GameLayer : public cocos2d::Layer {
    
public:
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    
    virtual void onEnter();
    virtual void onExit();
    
    void heroMove(float dt);
    
    CREATE_FUNC(GameLayer);
};

#endif /* GameLayer_h */
