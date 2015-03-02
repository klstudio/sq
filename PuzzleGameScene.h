#ifndef __PUZZLEGAME_SCENE_H__
#define __PUZZLEGAME_SCENE_H__

#include "cocos2d.h"
#include "GridLayer.h"

USING_NS_CC;

class PuzzleGameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int level, int levelsCompleted, bool editorMode);

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    //CREATE_FUNC(PuzzleGameScene);
    static PuzzleGameScene *create(int level, int levelsCompleted, bool editorMode);

    void update(float dt);
    void loadLevel(int n);

    PuzzleGameScene();
    ~PuzzleGameScene();

    bool _editorMode;
    CCSize _screenSize;

private:
    GridLayer *_gridLayer;    //level stage
    int _level;
    int _levelsCompleted;

    void addReplayButton();
    void addOptionButton();
    void replay(CCObject* pSender);
    void option(CCObject* pSender);
};

#endif //__PUZZLEGAME_SCENE_H__

