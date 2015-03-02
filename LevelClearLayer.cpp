//
//  LevelClearLayer.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#include "LevelClearLayer.h"
#include "GameConfig.h"
#include "PuzzleGameScene.h"
#include "LevelData.h"

LevelClearLayer::LevelClearLayer()
{
    //load resource
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    setTouchEnabled(true);
    _labelUserScore = 0;
    _btnNext = 0;
    _btnReplay = 0;
    _box = 0;
    _capTitle = 0;
}

LevelClearLayer::~LevelClearLayer()
{
}


void LevelClearLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{

}

void LevelClearLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

LevelClearLayer* LevelClearLayer::create(int userScore, int levelGoal, int curLevel, ClearType type)
{
    LevelClearLayer* node = new LevelClearLayer();
    if(!node) {
        CC_SAFE_DELETE(node);
        return node;
    }
    node->autorelease();

    //some init work
    node->init(userScore, levelGoal, curLevel, type);

    return node;
}

#define add_menu(on, off, pMenu, cb)    {   \
    CCSprite * menuItemOn; \
    CCSprite * menuItemOff; \
    menuItemOn = CCSprite::createWithSpriteFrameName((on));   \
    menuItemOff = CCSprite::createWithSpriteFrameName((off));  \
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    \
                               menuItemOff, \
                               menuItemOn,  \
                               this,    \
                               menu_selector( cb ) ); \
    (pMenu) = CCMenu::create(mis, NULL); \
    this->addChild(pMenu, z_ForeGround);    \
    }

void LevelClearLayer::init(int userScore, int levelGoal, int curLevel, ClearType type)
{
    CCLayerColor::initWithColor( ccc4(20,20,100,0) );
    _curLevel = curLevel;
    //setOpacity(100);

    //char str[50];
    //sprintf(str, "Your moves: %d", userScore);
    //_labelUserScore = CCLabelTTF::create(str, "Arial", 64);
    //_labelUserScore->setPosition(ccp(design_Size_Width/2, design_Size_Height/2));
    //this->addChild(_labelUserScore);

    //box
    //_box = CCSprite::createWithSpriteFrameName("box_level_clear.png");
    _box = CCSprite::create("box_level_clear.png");
    _box->setPosition(center_position);
    this->addChild(_box, z_BackGround);


    float boxTopY = center_position.y + _box->getContentSize().height/2;
    float nextY=0;
    CCMenu *pMenu=0; 
    float spaceBetween = 50;
    float btnHeight=97;
    if(type == eSucceed) {
        //level pass
        _capTitle =  CCSprite::createWithSpriteFrameName("cap_level_succeed.png");
        _capTitle->setPosition(ccp(center_position.x, boxTopY - _capTitle->getContentSize().height/2 - 50));
        this->addChild(_capTitle, z_ForeGround);

        nextY = _capTitle->getPosition().y - _capTitle->getContentSize().height/2;

        if(_curLevel < TOTAL_LEVELS ) {
            add_menu("btn_nextlevel_on.png", "btn_nextlevel_off.png", pMenu, LevelClearLayer::nextlevel);
            pMenu->setPosition(ccp(center_position.x, nextY - btnHeight/2 - spaceBetween));
            nextY = nextY - spaceBetween - btnHeight;

            add_menu("btn_replaylevel_on.png", "btn_replaylevel_off.png", pMenu, LevelClearLayer::replay);
            pMenu->setPosition(ccp(center_position.x, nextY - btnHeight/2 - spaceBetween));
        }else{
            if( Lite_Version ) {
                add_menu("btn_fullversion_on.png", "btn_fullversion_off.png", pMenu, LevelClearLayer::getFullVersion);
                pMenu->setPosition(ccp(center_position.x, nextY - btnHeight/2 - spaceBetween));
                nextY = nextY - spaceBetween - btnHeight;
            }
        }

    }else if(type == eOutOfMoves) {
        //level fail
        _capTitle =  CCSprite::createWithSpriteFrameName("cap_level_fail.png");
        _capTitle->setPosition(ccp(center_position.x, boxTopY - _capTitle->getContentSize().height/2 - 50));
        this->addChild(_capTitle, z_ForeGround);

        nextY = _capTitle->getPosition().y - _capTitle->getContentSize().height/2;

        add_menu("btn_replaylevel_on.png", "btn_replaylevel_off.png", pMenu, LevelClearLayer::replay);
        pMenu->setPosition(ccp(center_position.x, nextY - btnHeight/2 - spaceBetween));

#if 0
        nextY = nextY - spaceBetween - btnHeight;
        if( LevelData::sharedLevelData()->hasHint(_curLevel) ) {
            add_menu("btn_hint_on.png", "btn_hint_off.png", pMenu, LevelClearLayer::showHint);
            pMenu->setPosition(ccp(center_position.x, nextY - btnHeight/2 - spaceBetween));
        }
#endif
    }
}

void LevelClearLayer::clear()
{
    this->removeAllChildren();
    _box = CCSprite::create("box_level_clear.png");
    _box->setPosition(center_position);
    this->addChild(_box, z_BackGround);
}

void LevelClearLayer::showHint()
{
    char fn[50];
    this->clear();
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
            menu_selector( LevelClearLayer::replay ) ); 
    CCMenu *hintMenu = CCMenu::create(mis, NULL);
    this->addChild(hintMenu, z_ForeGround);
    hintMenu->setPosition(ccp(center_position.x, boxBottomY + 40));
}

void LevelClearLayer::replay(CCObject* pSender)
{
    sound_play_button;
    CCScene* newScene = PuzzleGameScene::scene(_curLevel, _curLevel, false);
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void LevelClearLayer::nextlevel(CCObject* pSender)
{
    sound_play_button;
    //To Do: check max level
    CCScene* newScene = CCTransitionMoveInR::create(0.2f, PuzzleGameScene::scene(_curLevel + 1, _curLevel, false));
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void LevelClearLayer::getFullVersion(CCObject* pSender)
{
    CCApplication::sharedApplication()->openURL("https://itunes.apple.com/us/app/squarecraft/id967120548");
}


