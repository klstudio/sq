//
//  StartMenuLayer.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#include "StartMenuLayer.h"
#include "LevelSelectLayer.h"
#include "PuzzleGameScene.h"
#include "GameConfig.h"
#include "SoundPlay.h"
#include "UserData.h"

#define UD (UserData::sharedUserData())

StartMenuLayer::StartMenuLayer()
{
    createScreen();
    setTouchEnabled(true);
}

StartMenuLayer::~StartMenuLayer()
{
}


void StartMenuLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{

}

void StartMenuLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void StartMenuLayer::resumePlay()
{
    sound_play_button;
    // Add code here switch to game play scene
    int level = UD->getNumLevelsUnlocked();
    CCScene* newScene = CCTransitionMoveInR::create(0.2f, PuzzleGameScene::scene(level, level, false));
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void StartMenuLayer::showLevels()
{
    sound_play_button;
    CCScene *pScene = LevelSelectLayer::scene(false);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void StartMenuLayer::showEditor()
{
    //create and run level editor scene
    CCScene *pScene = LevelSelectLayer::scene(true);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void StartMenuLayer::getFullVersion()
{
    CCApplication::sharedApplication()->openURL("https://itunes.apple.com/us/app/squarecraft/id967120548");
}

void StartMenuLayer::createScreen()
{
    //load resource
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");

    CCSprite * bg = CCSprite::create("bg2.png");
    bg->setPosition(ccp(design_Size_Width * 0.5f, design_Size_Height * 0.5f));
    this->addChild(bg, z_BackGround2);

    //title
    CCSprite *title = CCSprite::createWithSpriteFrameName("title.png");
    title->setPosition(ccp(design_Size_Width*0.5f, design_Size_Height*0.8f));
    this->addChild(title, z_BackGround);

    if(Lite_Version) {
        CCSprite *liteTitle = CCSprite::createWithSpriteFrameName("lite_title.png");
        liteTitle->setPosition(ccp(design_Size_Width*0.70f, design_Size_Height*0.8f - 230));
        this->addChild(liteTitle, z_BackGround);
    }

     //create menu
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_play_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_play_off.png");

    CCMenuItemSprite * playItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               //menu_selector(StartMenuLayer::showLevels));
                               menu_selector(StartMenuLayer::resumePlay));

#if 0
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_editor_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_editor_on.png");
    CCMenuItemSprite * editorItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(StartMenuLayer::showEditor));

    _mainMenu = CCMenu::create(playItem, editorItem, NULL);
#endif
    _mainMenu = CCMenu::create(playItem, NULL);
    _mainMenu->alignItemsVerticallyWithPadding(10);
    _mainMenu->setPosition(ccp(design_Size_Width*0.5f, design_Size_Height*0.35f));
    this->addChild(_mainMenu, z_ForeGround);

    if(Lite_Version) {
        CCMenu *fullVersionMenu;
        menuItemOn = CCSprite::createWithSpriteFrameName("btn_fullversion_on.png");
        menuItemOff = CCSprite::createWithSpriteFrameName("btn_fullversion_off.png");
        CCMenuItemSprite * fullVersionItem = CCMenuItemSprite::create(
                menuItemOff,
                menuItemOn,
                this,
                menu_selector(StartMenuLayer::getFullVersion));

        fullVersionMenu = CCMenu::create(fullVersionItem, NULL);
        fullVersionMenu->setPosition(ccp(design_Size_Width*0.5f, design_Size_Height*0.35f - 300));
        this->addChild(fullVersionMenu, z_ForeGround);
    }

}

CCScene* StartMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // add layer as a child to scene
    CCLayer* layer = new StartMenuLayer();
    scene->addChild(layer);
    layer->release();

    return scene;
}

