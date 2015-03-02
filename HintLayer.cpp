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
    char fn[50];

    _gridLayer = gridLayer;
    _curLevel = curLevel;

    _box = CCSprite::create("box_level_hint.png");
    _box->setPosition(center_position);
    this->addChild(_box, z_BackGround);

    float boxTopY = center_position.y + _box->getContentSize().height/2;

    //add hint pic
    sprintf(fn, "hint_level%d.png", _curLevel);
    CCSprite *spHint = CCSprite::create( fn );
    spHint->setPosition(center_position);
    this->addChild(spHint, z_ForeGround);

    float boxBottomY = center_position.y - _box->getContentSize().height/2;

    //ok button
    CCSprite * menuItemOn=0; 
    CCSprite * menuItemOff=0; 
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_tut_ok_off.png");   
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_tut_ok_on.png");  
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    
            menuItemOff, 
            menuItemOn, 
            this,    
            menu_selector( HintLayer::ok ) ); 
    CCMenu *hintMenu = CCMenu::create(mis, NULL);
    this->addChild(hintMenu, z_ForeGround);
    hintMenu->setPosition(ccp(center_position.x, boxBottomY + 40));

    //pause game
    _gridLayer->pause();
}

void HintLayer::ok(CCObject* pSender)
{
    //resume game
    _gridLayer->resume();
    
    //remove hint layer
    this->removeFromParentAndCleanup(1);
}


