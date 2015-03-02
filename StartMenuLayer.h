//
//  StartMenulayer.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__StartMenuLayer__
#define __SquareCraft__StartMenuLayer__

#include "cocos2d.h"
#include "GameConfig.h"

USING_NS_CC;

class StartMenuLayer : public CCLayer {
    public:
        ~StartMenuLayer();
        StartMenuLayer();

        virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

        static cocos2d::CCScene *scene();
        void showLevels();
        void showEditor();
        void resumePlay();
        void getFullVersion();

    private:
        void createScreen();
         CCMenu* _mainMenu;
};


#endif /* defined(__SquareCraft__StartMenuLayer__) */
