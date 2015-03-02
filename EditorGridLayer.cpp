//
//  EditorGridLayer.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "EditorGridLayer.h"
#include "GameConfig.h"

using namespace std;

EditorGridLayer::EditorGridLayer()
{
    setTouchEnabled(true);
    _editorState = Editing;

    //init square buttons
    float x=square_SideLen*0.5;
    float y=design_Size_Height - square_SideLen;
    _sqNormalYellow = Square::create(Square::eNormalYellow);
    _sqNormalYellow->setPosition(ccp(x, y));
    this->addChild(_sqNormalYellow,z_ForeGround);

    _sqCur = Square::create(Square::eNormalYellow);
    _sqCur->setPosition(ccp(square_SideLen*0.5, grid_Bottom+square_SideLen*0.5));
    this->addChild(_sqCur,z_ForeGround);

    x += square_SideLen;
    _sqNormalRed = Square::create(Square::eNormalRed);
    _sqNormalRed->setPosition(ccp(x, y));
    this->addChild(_sqNormalRed,z_ForeGround);

    x += square_SideLen;
    _sqNormalPurple = Square::create(Square::eNormalPurple);
    _sqNormalPurple->setPosition(ccp(x, y));
    this->addChild(_sqNormalPurple,z_ForeGround);

    x += square_SideLen;
    _sqNormalGreen = Square::create(Square::eNormalGreen);
    _sqNormalGreen->setPosition(ccp(x, y));
    this->addChild(_sqNormalGreen,z_ForeGround);

    x += square_SideLen;
    _sqDrawing = Square::create(Square::eDrawing);
    _sqDrawing->setPosition(ccp(x,y));
    this->addChild(_sqDrawing, z_ForeGround);

    _levelModified = false;
}


EditorGridLayer::~EditorGridLayer()
{
    LevelEntry le;
    int bestMoves = _parMoves < 100 ? _parMoves : 100 ;
    if(bestMoves == 0 ) bestMoves = 100;
    if(_levelModified) {
        setLevelEntry(&le, _grid, bestMoves);
        LevelData::sharedLevelData()->setLevel(&le, _curLevel);
        LevelData::sharedLevelData()->writeLevelDataFile();
    }
}

bool EditorGridLayer::pointInGrid(int i, int j, CCPoint p)
{
    CCPoint pos = _grid[i][j].pos;
    float sx = pos.x;
    float sy = pos.y;
    float w = square_SideLen;
    float h = square_SideLen;
    float l = sx - w/2;
    float r = sx + w/2;
    float b = sy - h/2;
    float t = sy + h/2;

    if( (p.x > l) && (p.x < r) && (p.y < t) && (p.y > b) )
        return true;
    return false;

}

void EditorGridLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if(_editorState == Testing) {
        GridLayer::ccTouchesBegan(touches, event);
    }else{
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        if (touch) {
            if(_sqNormalYellow->inSquare(tap)) {
                _sqCur->changeType(Square::eNormalYellow);
            }else if(_sqNormalRed->inSquare(tap)) {
                _sqCur->changeType(Square::eNormalRed);
            }else if(_sqNormalPurple->inSquare(tap)) {
                _sqCur->changeType(Square::eNormalPurple);
            }else if(_sqNormalGreen->inSquare(tap)) {
                _sqCur->changeType(Square::eNormalGreen);
            }else if(_sqDrawing->inSquare(tap)) {
                _sqCur->changeType(Square::eDrawing);
            }else{
                //touch on grid
                for(int r=0; r<grid_Row; ++r) {
                    for(int c=0; c<grid_Column; ++c) {
                        if(pointInGrid(r, c, tap)) {
                            if(_grid[r][c].sq) {
                                Square *s = _grid[r][c].sq;
                                s->removeFromParent();
                                _grid[r][c].sq = 0;
                            }else{
                                Square *s = Square::create(_sqCur->_type);
                                _grid[r][c].sq = s;
                                s->setPosition(_grid[r][c].pos);
                                this->addChild(s, z_BackGround);
                                s->setRC(r, c);
                            }
                            
                            if(!_levelModified) {
                                _levelModified = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

void EditorGridLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if(_editorState == Testing) {
        return GridLayer::ccTouchesMoved(touches, event);
    }
}

void EditorGridLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if(_editorState == Testing) {
        GridLayer::ccTouchesEnded(touches, event);
    }
}

EditorGridLayer* EditorGridLayer::create()
{
    EditorGridLayer *node = new EditorGridLayer();
    if(!node) {
        CC_SAFE_DELETE(node);
        return node;
    }
    node->autorelease();
    return node;
}

void EditorGridLayer::step(float dt)
{
    if(_editorState == Testing) {
        GridLayer::step(dt);
    }
}

void EditorGridLayer::loadLevel(int n)
{
    LevelEntry le;
    memset(&le, 0, sizeof(LevelEntry));

    //try to load it from
    if(!LevelData::sharedLevelData()->getLevel(&le, n)) {
        CCLog("load level %d fail", n);
        //continue to create new level
        _parMoves = 100;
    }else{
        _parMoves = le.bestMoves;
    }

    for(int i=0; i<nRow; ++i) {
        for(int j=0; j<nColumn; ++j) {
            if(le.g[i][j] != 0) {
                Square *s = Square::create((Square::SquareType)le.g[i][j]);
                s->setPosition(_grid[i][j].pos);
                this->addChild(s, z_BackGround);
                s->setRC(i, j);
                _grid[i][j].sq = s;
            }
        }
    }

    _editorState = Editing;
    _curLevel = n;

}

