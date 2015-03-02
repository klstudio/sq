#include "LevelSelectLayer.h"
#include "PuzzleGameScene.h"
#include "StartMenuLayer.h"
#include "UserData.h"
#include "GameConfig.h"
#include "SoundPlay.h"

//this is for testing purposes... so imagine we have 100 levels!

#define UD (UserData::sharedUserData())

LevelSelectLayer::~LevelSelectLayer()
{
    CC_SAFE_RELEASE(_levels);
    CC_SAFE_RELEASE(_levelLabels);
}

LevelSelectLayer::LevelSelectLayer(bool editorMode)
{
    //load resource
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _screenSize = CCDirector::sharedDirector()->getWinSize();

    _editorMode = editorMode;
	//load user data for number of levels completed
    //_levelsCompleted = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelsCompleted");
    _levelsCompleted = UD->getNumLevelsUnlocked();
    _firstIndex = 1 + (floor(_levelsCompleted/LEVELS_PER_SCREEN) * LEVELS_PER_SCREEN);
    if(_firstIndex > TOTAL_LEVELS) {
        _firstIndex = 1;
    }
    createScreen();
    setTouchEnabled( true );
}

void LevelSelectLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {

     CCTouch *touch = (CCTouch *)touches->anyObject();

     if (touch) {
         CCPoint tap = touch->getLocation();
         int count = _levels->count();
         CCSprite * btn;
         //track touch on level buttons
         for (int i = 0; i < count; i++) {
             btn = (CCSprite *) _levels->objectAtIndex(i);
             if (ccpDistance(tap, btn->getPosition()) <=  btn->boundingBox().size.width * 0.8f) {
                 if( (_firstIndex+i) <= UD->getNumLevelsUnlocked() ) {
                     if (btn->getTag() == kTagButtonOff) {
                         btn->setDisplayFrame(
                                 CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("level_button_on.png")
                                 );
                         return;
                     }
                 }
             }
         }
     }
}

void LevelSelectLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {

    CCTouch *touch = (CCTouch *)touches->anyObject();

    if (touch) {
        CCPoint tap = touch->getLocation();
        int count = _levels->count();
        CCSprite * btn;
        //track touch on level buttons
        for (int i = 0; i < count; i++) {
            btn = (CCSprite *) _levels->objectAtIndex(i);
            if (ccpDistance(tap, btn->getPosition()) <= btn->boundingBox().size.width * 0.8f) {
                 if( (_firstIndex+i) <= UD->getNumLevelsUnlocked() ) {
                    sound_play_button;
                    // Add code here switch to game play scene
                    CCScene* newScene = CCTransitionMoveInR::create(0.2f, PuzzleGameScene::scene(_firstIndex + i, _levelsCompleted, _editorMode));
                    CCDirector::sharedDirector()->replaceScene(newScene);
                    return;
                 }
            }
        }

        //navigation buttons
        //back
        if (ccpDistance(tap, _btnBack->getPosition()) <=  _btnBack->getTextureRect().size.width) {
            if (_firstIndex == 1) {
                CCScene* newScene = CCTransitionMoveInL::create(0.2f, StartMenuLayer::scene());
                CCDirector::sharedDirector()->replaceScene(newScene);
            } else {
                _firstIndex -= LEVELS_PER_SCREEN;
                createMenu();
                _btnNext->setVisible(true);

            }
            sound_play_button;

        //next
        } else if (ccpDistance(tap, _btnNext->getPosition()) <=  _btnNext->getTextureRect().size.width) {
            if (_firstIndex < TOTAL_LEVELS - LEVELS_PER_SCREEN) {
                _firstIndex += LEVELS_PER_SCREEN;
                createMenu();
            }
            sound_play_button;
        }

    }
}

CCScene* LevelSelectLayer::scene(bool editorMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // add layer as a child to scene
    CCLayer* layer = new LevelSelectLayer(editorMode);
    scene->addChild(layer);
    layer->release();

    return scene;
}


void LevelSelectLayer::createScreen() {
    //if we load this scene first, load sprite frames
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");

    //build level picker menu
    int cols = level_cols;
    int rows = level_rows;
    int i = _firstIndex;

    CCSprite * menuItem;
    CCLabelTTF * label;
    CCPoint position;

    _levelLabels = CCArray::createWithCapacity(LEVELS_PER_SCREEN);
    _levels = CCArray::createWithCapacity(LEVELS_PER_SCREEN);

    _levelLabels->retain();
    _levels->retain();

    CCSprite * bg = CCSprite::create("bg2.png");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, z_BackGround2);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            position = ccp(_screenSize.width * 0.17f +
                    c * 160,
                    _screenSize.height * 0.85f -
                    185 * r );
            if( !UD->isLevelUnlocked(i) ) {
                menuItem = CCSprite::createWithSpriteFrameName("level_button_off.png");
                menuItem->setTag(kTagButtonOff);
            } else {
                menuItem = CCSprite::createWithSpriteFrameName("level_button_on.png");
                menuItem->setTag(kTagButtonOn);
            }
            menuItem->setPosition(position);
            //add the background sprite to an array so we track touch collision
            _levels->addObject(menuItem);
            this->addChild(menuItem, z_MiddleGround);

            char szValue[10] = {0};
            sprintf(szValue, "%i", (int) i);
            label = CCLabelTTF::create(szValue, "Times New Roman", _screenSize.width * 0.1f);
            label->setPosition(position);
            //add the labels to an array so we can update them
            _levelLabels->addObject(label);

            this->addChild(label, z_ForeGround);
            i++;
        }
    }

    //navigation buttons
    _btnBack = CCSprite::createWithSpriteFrameName("btn_levelpage_back.png");
    _btnBack->setPosition(ccp(_screenSize.width*0.15f, _screenSize.height*0.1f));
    this->addChild(_btnBack);

    _btnNext = CCSprite::createWithSpriteFrameName("btn_levelpage_next.png");
    _btnNext->setPosition(ccp(_screenSize.width*0.85f, _screenSize.height*0.1f));
    this->addChild(_btnNext);

    if(_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN + 1) {
        _btnNext->setVisible(false);
    }

}

//update the level labels with the current displayed range (1-25, 26-50, 51-75, 76-100)
void LevelSelectLayer::createMenu() {

    int count = _levelLabels->count();
    CCSprite * btn;
    CCLabelTTF * label;
    int index = _firstIndex;
    for (int  i = 0; i <  count; i++) {
        label = (CCLabelTTF * ) _levelLabels->objectAtIndex(i);
        btn = (CCSprite * ) _levels->objectAtIndex(i);
        if( UD->isLevelUnlocked(index) ) {
            btn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("level_button_on.png"));
            btn->setTag(kTagButtonOn);
        } else {
            btn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("level_button_off.png"));
            btn->setTag(kTagButtonOff);
        }

        char szValue[10] = {0};
        sprintf(szValue, "%i", index);
        label->setString(szValue);
        index++;
    }

    if(_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN +1) {
        _btnNext->setVisible(false);
    }
}

