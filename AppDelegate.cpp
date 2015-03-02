#include "AppDelegate.h"
#include "GameConfig.h"
#include "StartMenuLayer.h"
#include "LevelSelectLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCSize screenSize = pEGLView->getFrameSize();
    //set design size
    CCSize designSize = CCSizeMake(design_Size_Width, design_Size_Height);

    float screenRatio = float(screenSize.width) / float(screenSize.height);
    float designRatio = float(designSize.width) / float(designSize.height);

    if( fabs(designRatio - screenRatio) < 0.05 ) {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit);
    } else {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
        pDirector->setBackground("bbg.png");
    }

    //load audio effect
    SimpleAudioEngine::sharedEngine()->preloadEffect( "button.wav" );
    SimpleAudioEngine::sharedEngine()->preloadEffect( "levelFailed.wav" );
    SimpleAudioEngine::sharedEngine()->preloadEffect( "levelSolved.wav" );
    SimpleAudioEngine::sharedEngine()->preloadEffect( "squareTouched.wav" );
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.6f);
    

    // create a scene. it's an autorelease object
    //CCScene *pScene = LevelSelectLayer::scene();
    CCScene *pScene = StartMenuLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
