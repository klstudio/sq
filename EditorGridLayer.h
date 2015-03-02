//
//  EditorGridLayer.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__EditorGridLayer__
#define __SquareCraft__EditorGridLayer__

#include "cocos2d.h"
#include "GameConfig.h"
#include "GridLayer.h"
#include "LevelData.h"
#include <stdio.h>
#include <string>

using namespace std;

USING_NS_CC;

class EditorGridLayer : public GridLayer {
    public:
        enum{
            Editing,
            Testing
        }_editorState;
        ~EditorGridLayer();
        EditorGridLayer();

        virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void step(float dt);
        virtual void loadLevel(int n);

        static EditorGridLayer *create();

        bool pointInGrid(int r, int c, CCPoint p);

    private:
        //level is modified
        bool _levelModified;
        //square buttons
        Square *_sqNormalYellow;
        Square *_sqNormalRed;
        Square *_sqNormalPurple;
        Square *_sqNormalGreen;

        //drawing squares
        Square *_sqDrawing;

        //current choice
        Square *_sqCur;

};


#endif /* defined(__SquareCraft__EditorGridLayer__) */
