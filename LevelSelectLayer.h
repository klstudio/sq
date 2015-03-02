#ifndef __LevelSelectLayer__
#define __LevelSelectLayer__

#include "cocos2d.h"

USING_NS_CC;

//using namespace CocosDenshion;

enum {
    kTagButtonOn,
    kTagButtonOff
};

class LevelSelectLayer : public CCLayer {
public:
    
    ~LevelSelectLayer();
    LevelSelectLayer(bool editorMode);
    
    static cocos2d::CCScene* scene(bool editorMode);
    
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
private:
    CCSize _screenSize;
    CCMenu * _levelMenu;
    CCArray * _levels;
    CCArray * _levelLabels;

    bool _editorMode;
    
    CCSprite * _btnBack;
    CCSprite * _btnNext;
    
    int _levelsCompleted;
    int _firstIndex;
    
    void createScreen(void);
    void createMenu(void);
    
};

#endif 
