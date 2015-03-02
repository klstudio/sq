//
//  LevelClearLayer.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__LevelClearLayer__
#define __SquareCraft__LevelClearLayer__

#include "cocos2d.h"
#include "SoundPlay.h"
USING_NS_CC;

class LevelClearLayer : public CCLayerColor {
    public:
        enum ClearType{
            eSucceed,
            eOutOfMoves,
            eCompleted  //level cleared with more moves
        }_type;
        ~LevelClearLayer();
        LevelClearLayer();

        virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

        static LevelClearLayer* create(int userScore, int levelGoal, int curLevel, ClearType type);

        void init(int userScore, int levelGoal, int curLevel, ClearType type);
        int _curLevel;

    private:
        CCLabelTTF *_labelUserScore;

        CCSprite *_btnNext;
        CCSprite *_btnReplay;
        CCSprite *_box;
        CCSprite *_capTitle;

        void replay(CCObject* pSender);
        void nextlevel(CCObject* pSender);
        void clear();
        void showHint();
        void getFullVersion(CCObject* pSender);
};


#endif /* defined(__SquareCraft__LevelClearLayer__) */

