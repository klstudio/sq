//
//  TutorialLaye.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#include "TutorialLayer.h"
#define show_and_increase_square(i) \
    { \
    Square *sq = (Square*) _tipSquares[ i ]; \
    sq->setVisible(true); \
    i += 1; \
    }

TutorialLayer::TutorialLayer()
{

}

TutorialLayer::~TutorialLayer()
{

}

TutorialLayer* TutorialLayer::create(int level, GridLayer* gridLayer)
{
    TutorialLevel1* node = new TutorialLevel1();
    if(!node) {
        CC_SAFE_DELETE(node);
        return node;
    }
    node->autorelease();
    node->_level = level;
    node->_gridLayer = gridLayer;
    node->initTutorial();
    return node;
}

void TutorialLayer::show(int step)
{

}

void TutorialLayer::hide(int step)
{

}

void TutorialLayer::initTutorial()
{
    CCLog("TutorialLayer::initTutorial gets called!");
}


////////////////////// Level 1 //////////////////////////////////////////////
void TutorialLevel1::initTutorial()
{
    //create and add guiding hand animation

    //create and add description text
    //_captionDraw = CCSprite::createWithSpriteFrameName("cap_draw.png");
    _captionDraw = CCSprite::create("cap_draw.png");
    _captionDraw->setPosition(cap_draw_position);
    this->addChild(_captionDraw, z_ForeGround2);
    _captionDraw->setVisible(false);

    //_captionLaunch = CCSprite::createWithSpriteFrameName("cap_launch.png");
    _captionLaunch = CCSprite::create("cap_launch.png");
    _captionLaunch->setPosition(cap_launch_position);
    this->addChild(_captionLaunch, z_ForeGround2);
    _captionLaunch->setVisible(false);

    //_captionGesture = CCSprite::createWithSpriteFrameName("box_tut_gesture.png");
    _captionGesture = CCSprite::create("box_tut_gesture.png");
    _captionGesture->setPosition(cap_gesture_position);
    this->addChild(_captionGesture, z_ForeGround);
    //this->addChild(_captionGesture, z_ForeGround2);
    _captionGesture->setVisible(false);
    //_captionGesture->setOpacity(100);

    //_captionGoal = CCSprite::createWithSpriteFrameName("cap_goal.png");
    _captionGoal = CCSprite::create("cap_goal.png");
    _captionGoal->setPosition(cap_goal_position);
    this->addChild(_captionGoal, z_ForeGround);
    _captionGoal->setVisible(false);

    //finger for animation
    _finger = CCSprite::createWithSpriteFrameName("finger.png");
    _finger->setPosition(_gridLayer->_grid[16][2].pos + ccp(20, 0));
    this->addChild(_finger, z_ForeGround2);
    _finger->setVisible(false);

    //squares
#define Add_TipSquare(r, c)   \
    sq = Square::create(Square::eCraft); \
    _tipSquares[i] = sq; \
    i++; \
    sq->setPosition(_gridLayer->_grid[r][c].pos); \
    this->addChild(sq, z_MiddleGround);  \
    sq->setOpacity(100); \
    sq->setVisible(false);

    Square *sq;
    int i=0;

    Add_TipSquare(15, 2);
    Add_TipSquare(15, 3);
    Add_TipSquare(15, 4);
    Add_TipSquare(15, 5);
    Add_TipSquare(14, 5);
    Add_TipSquare(14, 4);

    _curStep = -1;
    _curSquareIdx = 0;
    _numSteps = 3;
}

void TutorialLevel1::show(int step)
{
    if(_curStep == step ) return;
    switch(step) {
        case 1:
            showStep1();
            break;
        case 2:
            showStep2();
            break;
        case 3:
            showStep3();
            break;
        default:
            break;
    };
    _curStep = step;

}

void TutorialLevel1::hide(int step)
{
    if(_curStep > 0 && _curStep <= _numSteps) {
        switch(step) {
            case 1:
                hideStep1();
                break;
            case 2:
                hideStep2();
                break;
            case 3:
                hideStep3();
                break;
            default:
                break;
        };
    }
    
}

void TutorialLevel1::showStep1()
{
    if(_curStep != -1) {
        CCLog("line %d tutoriallevel1 _curStep != -1 ", __LINE__);
    }
    _captionDraw->setVisible(true);
    _finger->setVisible(true);
    _finger->setOpacity(255);

    //add ok menu button
    CCSprite * menuItemOn=0; 
    CCSprite * menuItemOff=0; 
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_tut_ok_off.png");   
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_tut_ok_on.png");  
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    
            menuItemOff, 
            menuItemOn, 
            this,    
            menu_selector( TutorialLevel1::showDrawDone ) ); 
    _drawMenu = CCMenu::create(mis, NULL); 
    _drawMenu->setPosition(ccp(cap_draw_position.x, cap_draw_position.y ));
    this->addChild(_drawMenu, z_ForeGround2);    

    show_and_increase_square(_curSquareIdx);
    CCAction *moveFinger = CCSequence::create(
                            CCFadeIn::create(1.0f),
                            CCMoveBy::create(0.4, ccp(square_SideLen, 0)),
                            CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::moveFingerDone)),
                            CCMoveBy::create(0.4, ccp(square_SideLen, 0)),
                            CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::moveFingerDone)),
                            CCMoveBy::create(0.4, ccp(square_SideLen, 0)),
                            CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::moveFingerDone)),
                            CCMoveBy::create(0.4, ccp(0, square_SideLen)),
                            CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::moveFingerDone)),
                            CCMoveBy::create(0.4, ccp(-square_SideLen, 0)),
                            CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::moveFingerDone)),
                            NULL);
    _finger->runAction( (CCAction*)moveFinger->copy()->autorelease() );
    _captionDraw->setOpacity(255);
    _captionDraw->runAction(CCFadeIn::create(1.0f));
    _drawMenu->runAction(CCFadeIn::create(1.0f));
    _gridLayer->pause();
    this->_curStep = 1;

}

void TutorialLevel1::hideStep1()
{
    if(_curStep == 1) {
        _finger->stopAllActions();
        _finger->setVisible(false);
        _captionDraw->setVisible(false);

        for(int i=0; i<6; ++i) {
            Square *sq = (Square*)_tipSquares[i];
            sq->setVisible(false);
        }
        _curSquareIdx = 0;
        _drawMenu->removeFromParent();
        _curStep == -1;
    }
}

void TutorialLevel1::showStep2()
{
    if(_curStep != -1) {
        CCLog("line %d tutoriallevel1 _curStep != -1 ", __LINE__);
    }
    //_gridLayer->pause();
    //_captionLaunch->setVisible(true);
    _captionLaunch->setOpacity(255);

    //add ok menu button
    CCSprite * menuItemOn=0; 
    CCSprite * menuItemOff=0; 
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_tut_ok_off.png");   
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_tut_ok_on.png");  
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    
            menuItemOff, 
            menuItemOn, 
            this,    
            menu_selector( TutorialLevel1::showLaunchDone ) ); 
    _launchMenu = CCMenu::create(mis, NULL); 
    _launchMenu->setPosition(ccp(cap_launch_position.x, cap_launch_position.y + 30 ));
    _launchMenu->setVisible(false);
    CCAction *delay_fadein = CCSequence::create(
                                CCDelayTime::create(5.0f),
                                CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::step2DelayDone)),
                                CCFadeIn::create(0.5f),
                                NULL );
    //_launchMenu->runAction(CCFadeIn::create(0.5f));
    //_captionLaunch->runAction(CCFadeIn::create(0.5f));
    _launchMenu->runAction( (CCAction*)delay_fadein->copy()->autorelease() );
    _captionLaunch->runAction( (CCAction*)delay_fadein->copy()->autorelease() );
    this->addChild(_launchMenu, z_ForeGround2);    
    this->_curStep = 2;
}

void TutorialLevel1::step2DelayDone(CCNode* node)
{
    _captionLaunch->setVisible(true);
    _launchMenu->setVisible(true);
    _gridLayer->pause();
}

void TutorialLevel1::hideStep2()
{
    if(_curStep == 2) {
        _captionLaunch->stopAllActions();
        _launchMenu->stopAllActions();

        _captionLaunch->setVisible(false);
        _launchMenu->removeFromParent();

        //for(int i=0; i<6; ++i) {
        //    Square *sq = (Square*) _tipSquares[i];
        //   sq->setVisible(false);
        //}
        _curStep = -1;
    }
}

void TutorialLevel1::showStep3()
{
    if(_curStep != -1) {
        CCLog("line %d tutoriallevel1 _curStep != -1 ", __LINE__);
    }
    _gridLayer->pause();
    _captionGesture->setVisible(true);
    _captionGesture->setOpacity(255);
    _captionGesture->runAction(CCFadeIn::create(0.5f));

    //_captionGoal->setVisible(true);
    //_captionGoal->runAction(CCFadeIn::create(0.5f));

    //CCDelayTime *delay = CCDelayTime::create(8.0f);
    //this->runAction(CCSequence::create(delay,
    //                                   CCCallFuncN::create(this, callfuncN_selector(TutorialLevel1::showDone)),
    //                                   NULL
    //                                  ) );

    //add ok button
    CCSprite * menuItemOn=0; 
    CCSprite * menuItemOff=0; 
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_tut_ok_off.png");   
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_tut_ok_on.png");  
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    
                               menuItemOff, 
                               menuItemOn, 
                               this,    
                               menu_selector( TutorialLevel1::showGestureDone ) ); 
    _gestureMenu = CCMenu::create(mis, NULL); 
    _gestureMenu->setPosition(ccp(cap_gesture_position.x, cap_gesture_position.y - 340));
    this->addChild(_gestureMenu, z_ForeGround2);    
    this->_curStep = 3;
}

void TutorialLevel1::hideStep3()
{
    if(_curStep == 3) {
        _captionGesture->setVisible(false);
        _captionGoal->setVisible(false);
        _gestureMenu->removeFromParent();
        _curStep = -1;
    }
}

void TutorialLevel1::moveFingerDone(CCNode* node)
{
    Square *sq = (Square*) _tipSquares[ _curSquareIdx ];
    sq->setVisible(true);
    _curSquareIdx += 1;
    if(_curSquareIdx >= 6) {
        _curSquareIdx = 0;
        //_gridLayer->resume();
        //add ok button
    }
}

void TutorialLevel1::moveFingerDone1(CCNode* node)
{
    Square *sq = (Square*) _tipSquares[ _curSquareIdx ];
    sq->setVisible(true);
    _curSquareIdx += 1;
    if(_curSquareIdx >= 6) {
        _curSquareIdx = 0;
    }
}

void TutorialLevel1::showDone(CCNode* node)
{
    _gridLayer->resume();
}

void TutorialLevel1::showGestureDone(CCObject* pSender)
{
    hideStep3();
    showGoal();
    //_gridLayer->resume();
}

void TutorialLevel1::showDrawDone(CCObject* pSender)
{
    hideStep1();
    _gridLayer->resume();
    showStep2();
}

void TutorialLevel1::showLaunchDone(CCObject* pSender)
{
    hideStep2();
    if( _gridLayer->isPaused() ) {
        _gridLayer->resume();
    }
}

void TutorialLevel1::showGoal()
{
    _captionGoal->setVisible(true);
    _captionGoal->runAction(CCFadeIn::create(0.5f));

    //add ok button
    CCSprite * menuItemOn=0; 
    CCSprite * menuItemOff=0; 
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_tut_ok_off.png");   
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_tut_ok_on.png");  
    CCMenuItemSprite * mis = CCMenuItemSprite::create(    
                               menuItemOff, 
                               menuItemOn, 
                               this,    
                               menu_selector( TutorialLevel1::showGoalDone ) ); 
    _goalMenu = CCMenu::create(mis, NULL); 
    _goalMenu->setPosition(ccp(cap_goal_position.x, cap_goal_position.y - 160));
    this->addChild(_goalMenu, z_ForeGround2);    

}

void TutorialLevel1::showGoalDone(CCObject* pSender)
{
    _captionGoal->setVisible(false);
    _goalMenu->removeFromParent();
    _gridLayer->resume();
}

