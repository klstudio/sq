#include "PuzzleGameScene.h"
#include "GridLayer.h"
#include "EditorGridLayer.h"
#include "GameConfig.h"
#include "LevelSelectLayer.h"
#include "StartMenuLayer.h"

USING_NS_CC;

CCScene* PuzzleGameScene::scene(int level, int levelsCompleted, bool editorMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    PuzzleGameScene *layer = PuzzleGameScene::create(level, levelsCompleted, editorMode );

    // add layer as a child to scene
    scene->addChild(layer);
    layer->release();

    // return the scene
    return scene;
}

PuzzleGameScene::PuzzleGameScene()
{
}

PuzzleGameScene::~PuzzleGameScene()
{

}

PuzzleGameScene* PuzzleGameScene::create(int level, int levelsCompleted, bool editorMode)
{
        PuzzleGameScene* pscene = new PuzzleGameScene();
        pscene->_level = level;
        pscene->_levelsCompleted = levelsCompleted;
        pscene->_editorMode = editorMode;

        //init should be called at the last step
        pscene->init();
        return pscene;
}


// on "init" you need to initialize your instance
bool PuzzleGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    _screenSize = CCDirector::sharedDirector()->getWinSize();

    CCSize designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
    CCLog("designResolutionSize w = %f , h = %f", designSize.width , designSize.height);
    CCLog("visibleSize w = %f, h = %f", visibleSize.width, visibleSize.height);
    CCLog("visible origin x = %f, y = %f", origin.x, origin.y);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
#if 0
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(PuzzleGameScene::menuCloseCallback));

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));


    //load resource
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, z_ForeGround);
#endif

    //background
    CCSprite * bg = CCSprite::create("bg1.png");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, z_BackGround2);

    // add grid player( the main play layer)
    if(!_editorMode) {
        _gridLayer = GridLayer::create();
        
        //add replay button
        addReplayButton();
        addOptionButton();
    }else{
        //level editor mode
        _gridLayer = EditorGridLayer::create();
        addOptionButton();
    }

    _gridLayer->setPosition(CCPointZero);
    this->addChild(_gridLayer, z_BackGround);
    //_gridLayer->openLevelDataFile();
    loadLevel(_level);

    this->scheduleUpdate();
    return true;
}

void PuzzleGameScene::addReplayButton()
{
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    menuItemOn = CCSprite::createWithSpriteFrameName("replay_button_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("replay_button_on.png");

    CCMenuItemSprite * replayItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(PuzzleGameScene::replay));
    //replayItem->setPosition(ccp(30, 60));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	replayItem->setPosition(ccp(origin.x+45+5, origin.y + design_Size_Height-60-10));
    CCMenu *pMenu = CCMenu::create(replayItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu, z_BackGround);
}


void PuzzleGameScene::replay(CCObject* pSender)
{
    CCScene *newScene = PuzzleGameScene::scene(_level, _levelsCompleted, false);
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void PuzzleGameScene::addOptionButton()
{
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_option_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_option_on.png");

    CCMenuItemSprite * optionItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(PuzzleGameScene::option));
    //replayItem->setPosition(ccp(30, 60));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	optionItem->setPosition(ccp(origin.x+design_Size_Width-54-5, origin.y + design_Size_Height-60-10));
    CCMenu *pMenu = CCMenu::create(optionItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu, z_BackGround);
}

void PuzzleGameScene::option(CCObject* pSender)
{
        //To Do: switch to level menu in release mode
        //CCScene* newScene = CCTransitionMoveInL::create(0.2f,  StartMenuLayer::scene());
        CCScene* newScene = CCTransitionMoveInL::create(0.2f,  LevelSelectLayer::scene(false));
        CCDirector::sharedDirector()->replaceScene(newScene);
}

void PuzzleGameScene::menuCloseCallback(CCObject* pSender)
{
    //if(!_editorMode) {
        //CCScene* newScene = CCTransitionMoveInL::create(0.2f,  LevelSelectLayer::scene());
        CCScene* newScene = CCTransitionMoveInL::create(0.2f,  StartMenuLayer::scene());
        CCDirector::sharedDirector()->replaceScene(newScene);
    //}
}

void PuzzleGameScene::update(float dt)
{
    _gridLayer->step(dt);
}

void PuzzleGameScene::loadLevel(int n)
{
    _gridLayer->loadLevel(n);
}


