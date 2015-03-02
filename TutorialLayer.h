//
//  TutorialLayer.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__TutorialLayer__
#define __SquareCraft__TutorialLayer__

#include "cocos2d.h"
#include "GameConfig.h"
#include "Square.h"
#include "GridLayer.h"

USING_NS_CC;

//tutorial level 1 config////////////////////////////////
//2x4 drawing squares
//drawing area Ymax_centre = bottom+4*60
//left = grid left + 2*60
#define cap_draw_position ccp(grid_Left+4*square_SideLen, grid_Bottom+4*square_SideLen+128+32)
#define cap_launch_position ccp(grid_Left+8*square_SideLen - 100, grid_Bottom+5*square_SideLen+100-square_SideLen)
#define cap_gesture_position ccp(grid_Left+5*square_SideLen, grid_Bottom+8*square_SideLen)
#define cap_goal_position ccp( cap_moves_position.x - 100, cap_moves_position.y - 340  )

/////////////////////////////////////////////////////////

class TutorialLayer : public CCLayer
{
    public:
        TutorialLayer();
        virtual ~TutorialLayer();
        static TutorialLayer* create(int level, GridLayer* gridLayer);
        virtual void initTutorial();
        virtual void show(int step);    //step 1 --> step 2 --> step 3
        virtual void hide(int step);
        int _numSteps;  //how many steps this tutorial has
        int _curStep;   // -1 : no step running

    protected:
        int _level; //only work with certain level
};

//basic
class TutorialLevel1 : public TutorialLayer
{
    public:
        virtual void initTutorial();
        virtual void show(int step);
        virtual void hide(int step);

        CCSprite *_captionDraw;
        CCSprite *_captionLaunch;
        CCSprite *_captionGesture;
        CCSprite *_captionGoal;
        CCMenu *_gestureMenu;
        CCMenu *_goalMenu;
        CCMenu *_drawMenu;
        CCMenu *_launchMenu;
        CCSprite *_finger;
        Square *_tipSquares[10];
        int _curSquareIdx;
        GridLayer* _gridLayer;

        void showStep1();
        void showStep2();
        void showStep3();
        void showGoal();

        void hideStep1();
        void hideStep2();
        void hideStep3();

        //callfuncN_selector
        void moveFingerDone(CCNode* node);
        void moveFingerDone1(CCNode* node);
        void step2DelayDone(CCNode* node);
        void showDone(CCNode* node);
        void showGestureDone(CCObject* pSender);
        void showGoalDone(CCObject* pSender);
        void showDrawDone(CCObject* pSender);
        void showLaunchDone(CCObject* pSender);
};

#endif

