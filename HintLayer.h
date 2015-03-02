//
//  HintLayer.h
//  SquareCraft Lite
//
//  Created by Jian Zhang on 2015-03-01.
//
//

#ifndef __SquareCraft_Lite__HintLayer__
#define __SquareCraft_Lite__HintLayer__

#include "cocos2d.h"
#include "GridLayer.h"

USING_NS_CC;

class HintLayer : public CCLayerColor {
    public:
        ~HintLayer();
        HintLayer();

        virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

        static HintLayer* create(int curLevel, GridLayer *gridLayer);

        void init(int curLevel, GridLayer *gridLayer);
        int _curLevel;

    private:
        GridLayer *_gridLayer;
        CCLabelTTF *_labelUserScore;

        CCSprite *_box;
        CCSprite *_btnOk;

        void ok(CCObject* pSender);
};

#endif /* defined(__SquareCraft_Lite__HintLayer__) */

