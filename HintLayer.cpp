//
//  HintLayer.cpp
//  SquareCraft Lite
//
//  Created by Jian Zhang on 2015-03-01.
//
//

#include "HintLayer.h"

HintLayer::HintLayer()
{
    _curLevel = 0;
    _gridLayer = 0;
}

HintLayer::~HintLayer()
{

}

void HintLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{

}

void HintLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{

}

HintLayer* create(int curLevel, GridLayer *gridLayer)
{
    HintLayer* node = new HintLayer();
    if(!node) {
        CC_SAFE_DELETE(node);
        return node;
    }
    node->autorelease();

    node->init(curLevel, gridLayer);
    return node;
}

void HintLayer::init(int curLevel, GridLayer *gridLayer)
{
    _gridLayer = gridLayer;
    _curLevel = curLevel;

    _box = CCSprite::create("box_level_hint.png");
    _box->setPosition(center_position);
    this->addChild(_box, z_BackGround);

    float boxTopY = center_position.y + _box->getContentSize().height/2;
    

}

