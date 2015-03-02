#include "GridLayer.h"
#include "GameConfig.h"
#include "utils.h"
#include "LevelClearLayer.h"
#include "HintLayer.h"
#include "LevelData.h"
#include "TutorialLayer.h"
#include "UserData.h"
#include <string>


using namespace std;

using namespace cocos2d;
#define UD (UserData::sharedUserData())

GridLayer::GridLayer()
{
    setTouchEnabled(true);
    glLineWidth(1.0 * CC_CONTENT_SCALE_FACTOR());
    _pos = ccp(grid_Left, grid_Bottom );
    _sideLen = SC_SQUARE_SIDE_LENGTH;
    _state = Invalid;
    _saveState = _state;
    nRow = grid_Row;
    nColumn = grid_Column;
    _movingSquares = 0;
    _craftMoveUp = 0;
    _craftMovable = false;
    initGrid();
    //_levelDataFile = 0;
    //_levelDataHead = 0;
    _tutorial = 0;

    this->_drawingSquares = CCArray::createWithCapacity(4*4);
    this->_drawingSquares->retain();
    this->createActions();
}

GridLayer::~GridLayer()
{
    CC_SAFE_RELEASE(_drawingSquares);
    CC_SAFE_RELEASE(_movingSquares);
    //CC_SAFE_RELEASE(_craftMoveUp);            //to debug: memory issue (inter-refer each other)

    //closeLevelDataFile();
    //releaseLevelDataHead();
}

void GridLayer::onExit()
{
    CCLayer::onExit();
    CC_SAFE_RELEASE(_craftMoveUp);
}

GridLayer* GridLayer::create()
{
    GridLayer* node = new GridLayer();
    if(!node) {
        CC_SAFE_DELETE(node);
        return node;
    }
    node->autorelease();
    return node;
}


//#define grid_color   0x48/255.0f, 0x52/255.0f, 0x61/255.0f, 1.0

//version 1
#define grid_color   0x66/255.0f, 0x75/255.0f, 0x58/255.0f, 1.0

//#define grid_color   0x78/255.0f, 0x44/255.0f, 0x21/255.0f, 1.0 
//#define grid_color   0x8a/255.0f, 0x7b/255.0f, 0x66/255.0f, 1.0
//#define grid_color   0x72/255.0f, 0x82/255.0f, 0x97/255.0f, 1.0

//dark
//#define grid_color   0x1d/255.0f, 0x20/255.0f, 0x28/255.0f, 1.0

//dark purple
//#define grid_color   0x44/255.0f, 0x21/255.0f, 0x78/255.0f, 1.0

//bright blue
//#define grid_color   0x00/255.0f, 0xff/255.0f, 0xff/255.0f, 1.0

void GridLayer::draw()
{
    int r, c;
    CCPoint begin, end;

    //draw grid
    ccDrawColor4F( grid_color );
    for(r=0, begin=_pos, end=_pos+ccp(nColumn*_sideLen, 0); r<=nRow; ++r) {
        //purple rgb 96,11,223
        //if( r == grid_DeadLine ) {
            //ccDrawColor4F( 0xaa/255.0f, 0xcc/255.0f, 0xff/255.0f, 1.0);
            //ccDrawLine(begin, end);
            //ccDrawColor4F( grid_color );
        //}else{
            ccDrawLine(begin, end);
        //}
        begin = begin + ccp(0.0f, _sideLen);
        end = end + ccp(0.0f, _sideLen);
    }

    for(c=0, begin=_pos, end=_pos+ccp(0,nRow*_sideLen); c<=nColumn; ++c) {
        ccDrawLine(begin, end);
        begin = begin + ccp(_sideLen, 0.0f);
        end = end + ccp(_sideLen, 0.0f);
    }

    CCLayer::draw();
}

void GridLayer::initGrid()
{
    //position of grid[0][0]
    CCPoint p00 = _pos + ccp(_sideLen/2, nRow*_sideLen - _sideLen/2);
    for(int r=0; r<nRow; ++r) {
        for(int c=0; c<nColumn; ++c) {
            _grid[r][c].pos = p00 + ccp(c*_sideLen, -r*_sideLen);   //position of each grid square
            _grid[r][c].sq = 0;
            _grid[r][c].r = r;
            _grid[r][c].c = c;
        }
    }
}

void GridLayer::createMovesLabel()
{
    _capMoves = CCSprite::createWithSpriteFrameName("cap_moves.png");
    _capMoves->setPosition(cap_moves_position);
    this->addChild(_capMoves, z_BackGround);

    char szValue[50] = {0};
    sprintf(szValue, "%i", _restMoves);
    //_labelUserMoves = CCLabelBMFont::create(szValue, "number_font.fnt", 40);
    _labelUserMoves = CCLabelTTF::create(szValue, "Arial Rounded MT Bold", 80);
    CCPoint pos = ccp(label_UserMoves_X, label_UserMoves_Y);
    _labelUserMoves->setPosition(pos);
    _labelUserMoves->setScale(1);
    _labelUserMoves->setColor(ccc3(200, 200, 0));
    this->addChild(_labelUserMoves, z_ForeGround);

    //sprintf(szValue, "Best Moves %i", _parMoves);
    //_labelParMoves = CCLabelTTF::create(szValue, "Arial", 40);
    //_labelParMoves->setPosition(ccp(200, label_BestMoves_Y));
    //this->addChild(_labelParMoves, z_ForeGround);
}

void GridLayer::createLevelLabel()
{
    char szValue[50] = {0};
    sprintf(szValue, "Level %i", _curLevel);
    //_labelLevel = CCLabelTTF::create(szValue, "Arial", 30);
    _labelLevel = CCLabelTTF::create(szValue, "Arial Rounded MT Bold", 40);
    _labelLevel->setPosition(ccp(label_level_X, label_level_Y));
    //#21 25 30 (hex) 
    _labelLevel->setColor(ccc3(0x21, 0x25, 0x30));
    this->addChild(_labelLevel, z_ForeGround);
}

bool GridLayer::isLevelCleared()
{
    int r, c;
    for(r=0; r<grid_Row; ++r) {
        for( c=0; c<grid_Column; ++c) {
            Square *sq = (Square*)_grid[r][c].sq;
            if(sq != 0 && sq->isBlock()) {
                return false;
            }
        }
    }

    return true;;
}

//interaction of moving squarecraft and puzzle squares
void GridLayer::step(float dt)
{
    if(_state == Running) {
        if(_craftMovable == true) {
            _floatingTime += dt;
            if(_floatingTime > sc_FloatingDuration) {
                //need move craft up
                if(craftCanMove(-1, 0)) {
                    craftMoveUp();
                }else{
                    //touched
                    CCLog("Touched");
                    sound_play_squareTouched;
                    _craftMovable = false;
                    convertMovingSuqaresToBlock();
                    //must put sensor row test here
                    _sensorTouched =  collideWithSensorRow();
                    increaseUserMoves();
                    decreaseRestMoves();
                    clearMovingSquares();
                    eliminateCompleteRows();
#if 0
                    if(isLevelCleared()) {
                        //run level clear layer
                        LevelClearLayer *lc_layer = LevelClearLayer::create(_userMoves, 
                                                                            _parMoves, 
                                                                            _curLevel,
                                                                            LevelClearLayer::eSucceed);
                        lc_layer->setPosition(CCPointZero);
                        this->addChild(lc_layer, z_ForeGround2);
                        _state = Invalid;
                        if(PRODUCT_DEV && _userMoves < _parMoves) {
                            _parMoves = _userMoves;
                            //set level of new best score
                            LevelData::sharedLevelData()->setBestMoves(_parMoves, _curLevel);
                            LevelData::sharedLevelData()->writeLevelDataFile();
                        }
                    }else{
                        if(_userMoves == _parMoves) {
                            //Level failure
                            LevelClearLayer *lc_layer = LevelClearLayer::create(_userMoves, 
                                    _parMoves, 
                                    _curLevel,
                                    LevelClearLayer::eOutOfMoves);
                            lc_layer->setPosition(CCPointZero);
                            this->addChild(lc_layer, z_ForeGround2);
                            _state = Invalid;
                        }else{
                            _state = Drawing;
                        }
                    }
#endif
                }
            }
        }
    }
}

void GridLayer::completeTouchDown()
{
    if(isLevelCleared()) {
        //run level clear layer
        LevelClearLayer *lc_layer = LevelClearLayer::create(_userMoves, 
                _parMoves, 
                _curLevel,
                LevelClearLayer::eSucceed);
        lc_layer->setPosition(CCPointZero);
        this->addChild(lc_layer, z_ForeGround2);
        _state = Invalid;
        if(PRODUCT_DEV && _userMoves < _parMoves) {
            _parMoves = _userMoves;
            //set level of new best score
            LevelData::sharedLevelData()->setBestMoves(_parMoves, _curLevel);
            LevelData::sharedLevelData()->writeLevelDataFile();
        }
        UD->unlockLevel(_curLevel+1);
        //sound
        sound_play_levelSolved;
    }else{
        if(_userMoves == _parMoves || _sensorTouched ) {
            //Level failure
            LevelClearLayer *lc_layer = LevelClearLayer::create(_userMoves, 
                    _parMoves, 
                    _curLevel,
                    LevelClearLayer::eOutOfMoves);
            lc_layer->setPosition(CCPointZero);
            this->addChild(lc_layer, z_ForeGround2);
            _state = Invalid;
            sound_play_levelFailed;
        }else{
            _state = Drawing;
        }
    }
}

void GridLayer::increaseUserMoves()
{
    //increase user moves
    ++_userMoves;
    CCSprite *touchedStar = CCSprite::createWithSpriteFrameName("SquareBlue.png");
    Square* sq = (Square*)_movingSquares->objectAtIndex(0);
    //To Do: find a lowest square
    touchedStar->setPosition(sq->getPosition());
    this->addChild(touchedStar);
    CCAction *flyToLabel = CCSequence::create(
                            CCScaleTo::create(0.1, 0.3),
                            CCMoveTo::create(0.3, _labelUserMoves->getPosition()),
                            CCCallFuncN::create(this, callfuncN_selector(GridLayer::flyToUserMovesLabelDone)),
                            NULL );

    touchedStar->runAction((CCAction*)flyToLabel->copy()->autorelease());
}

void GridLayer::flyToUserMovesLabelDone(CCNode *node)
{
    node->removeFromParent();
    updateMovesLabel();
    completeTouchDown();
}

void GridLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint tap = touch->getLocation();

    if (touch) {
        if(_state == Drawing) {
            int count = _drawingSquares->count();
            for(int i=0; i<count; ++i) {
                Square *sq = (Square*)_drawingSquares->objectAtIndex(i);
                if(sq->inSquare(tap)) {
                    _touchBeginSquare = sq;
                    if( sq->_type == Square::eDrawing) {
                        CCLog("touched drawing square");
                        sq->changeType(Square::eCraft);
                    }else if(sq->isCraft()) {
                        sq->changeType(Square::eDrawing);
                    }

                    if(_tutorial) {
                        //_tutorial->hide(1);
                        //_tutorial->show(2);
                    }
                }
            }

            if( PointInSprite(tap, *_btnGo) ) {
                _btnGoPressed = true;
                CCActionInterval* btnGoActive = CCSequence::create( CCScaleTo::create(0.2, 1.4),
                        CCScaleTo::create(0.2, 0.6),
                        0);
                CCRepeatForever *rep = CCRepeatForever::create(btnGoActive);
                _btnGo->runAction(rep);
            }
        }else if(_state == Running) {
            _touchBeginPos = tap;
        }
    }

}

void GridLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint tap = touch->getLocation();
    if(_state == Drawing && touch) {
        CCPoint tap = touch->getLocation();
        int count = _drawingSquares->count();
        for(int i=0; i<count; ++i) {
            Square *sq = (Square*)_drawingSquares->objectAtIndex(i);
            if(_touchBeginSquare && _touchBeginSquare->_type != Square::eDrawing) {
                if(sq->inSquare(tap)) {
                    if( sq->_type == Square::eDrawing) {
                        CCLog("touched drawing square");
                        sq->changeType(Square::eCraft);
                    } // normal square doesn't change in touch move
                }
            }
        }
    }else if(_state == Running && touch) {
        _gesture = None;
        _touchEndPos = tap;
        float dx = _touchEndPos.x - _touchBeginPos.x;
        float dy = _touchEndPos.y - _touchBeginPos.y;
        if(fabs(dx) < fabs(dy)) {
            if( dy > gesture_MinDistance ) {
                _gesture = UpSwipe;
            }
        }else if(fabs(dx) > fabs(dy)) {
            if( dx > 1.5*gesture_MinDistance ) {
                _gesture = RightSwipe;
            }else if( dx < -1.5*gesture_MinDistance) {
                _gesture = LeftSwipe;
            }
        }
        processGesture();
    }

    // GO button
    if(_btnGoPressed) {
        if( !PointInSprite(tap, *_btnGo) ) {
            _btnGoPressed = false;
            _btnGo->stopAllActions();
        }
    }
}


void GridLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint tap = touch->getLocation();

    if(_state == Running) {
        _gesture = None;
        _touchEndPos = tap;
        float dx = _touchEndPos.x - _touchBeginPos.x;
        float dy = _touchEndPos.y - _touchBeginPos.y;
        if(fabs(dx) > fabs(dy)) {
            if( dx > gesture_MinDistance ) {
                //_gesture = RightSwipe;
            }else if( dx < -gesture_MinDistance) {
                //_gesture = LeftSwipe;
            }
        }else {
            if( dy > gesture_MinDistance ) {
                //_gesture = UpSwipe;
            }else if( dy < -1.5*gesture_MinDistance) {
                _gesture = DownSwipe;
            }
        }
        processGesture();
    }else if(_state == Drawing) {
        //To Do: handle the case when there's zero normal square
        if(_btnGoPressed) {
            if( PointInSprite(tap, *_btnGo) && _state == Drawing ) {
                if( createMovingSquares() ) {
                    //done with crafting
                    _state = Running;
                    //add crafted squares to _movingSquares
                    //clear crafted squares to eDrawing type
                    craftInitMatrix();
                    //move it up right a way
                    if(_tutorial) {
                        _tutorial->hide(2);
                        _tutorial->show(3);
                    }
                    craftMoveUp();
                }
            }
            _btnGoPressed = false;
            _btnGo->stopAllActions();
            _btnGo->setScale(1);
        }

        _touchBeginSquare = 0;
    }
}

void GridLayer::processGesture()
{
    if(_state == Running ) {
        if(_gesture == LeftSwipe) {
            //moving left
            if(craftCanMove(0, -1)) {
                craftMoveLeft();
                _touchBeginPos = _touchEndPos;
            }
        }else if(_gesture == RightSwipe) {
            //moving right
            if(craftCanMove(0, 1)) {
                craftMoveRight();
                _touchBeginPos = _touchEndPos;
            }
        }else if(_gesture == DownSwipe) {
            //rotation
            craftRotate();
        }else if(_gesture == UpSwipe) {
           _floatingTime += sc_FloatingDuration;
        }
    }
}

bool GridLayer::createMovingSquares()
{
    bool ret = false;
    if(!_movingSquares) {
        _movingSquares = CCArray::createWithCapacity(1);
        _movingSquares->retain();
    }else{
        CCLog("_movingSquares already created!");
        _movingSquares->removeAllObjects();
    }

    int count = _drawingSquares->count();
    for(int i=0; i<count; ++i) {
        Square *drawing_sq = (Square*)_drawingSquares->objectAtIndex(i);
        if(drawing_sq->isCraft()) {
            //clone one and add to _movingSquares
            Square *moving_sq = Square::create( drawing_sq->_type );
            _movingSquares->addObject(moving_sq);
            moving_sq->setPosition(drawing_sq->getPosition());
            moving_sq->setRC(drawing_sq->_r, drawing_sq->_c);
            this->addChild(moving_sq, z_MiddleGround);

            //change the drawing square back to drawing state
            drawing_sq->changeType(Square::eDrawing);
            ret = true;
        }
    }

    if(ret) {
        _craftMovable = true;
    }
    return ret;
}

void GridLayer::createActions()
{
    
    _craftMoveUp = CCSequence::create( CCMoveBy::create(0.02, ccp(0, square_SideLen)),
            CCCallFuncN::create(this, callfuncN_selector(GridLayer::craftMoveUpDone)),
        0);
    _craftMoveUp->retain();
}

void GridLayer::craftMoveUp()
{
    if(_state != Running && _craftMovable != true) {
        CCLog("trying to move up craft in invalid state");
        return;
    }
    _craftMovable = false;
    
    int count = _movingSquares->count();
    for(int i=0; i<count; ++i) {
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        sq->runAction((CCFiniteTimeAction*)_craftMoveUp->copy()->autorelease());
    }
}

void GridLayer::craftMoveLeft()
{
    if(_state != Running && _craftMovable != true) {
        CCLog("trying to move left craft in invalid state");
        return;
    }
    int count = _movingSquares->count();
    for(int i=0; i<count; ++i) {
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        sq->setPosition( sq->getPosition() + ccp(-square_SideLen, 0));
    }
    updateMovingSquaresRC(0,-1); //for each
}

void GridLayer::craftMoveRight()
{
    if(_state != Running && _craftMovable != true) {
        CCLog("trying to move right craft in invalid state");
        return;
    }
    int count = _movingSquares->count();
    for(int i=0; i<count; ++i) {
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        sq->setPosition( sq->getPosition() + ccp(square_SideLen, 0));
    }
    updateMovingSquaresRC(0,1); //for each
}

//this will be called for each square in the array
void GridLayer::craftMoveUpDone()
{
    if(_craftMovable == false) {
        _craftMovable = true;
        updateMovingSquaresRC(-1,0); //for each
        //clear _floating time
        _floatingTime = 0;
        //update grid entry
    }
}

bool GridLayer::craftCanMove(int r_diff, int c_diff)
{
    // for each square in movingSquares array to check if moving up to empty grid
    // if not empty need to check if that grid is held by a moving squares
    // To Do: use a different type for craft square?
    int count = _movingSquares->count();
    for(int i=0; i<count; ++i) {
        int r_new, c_new;
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        r_new = sq->_r + r_diff;
        c_new = sq->_c + c_diff;
        //out of boundary
        if(r_new < 0 || r_new >= grid_Row || c_new < 0 || c_new >= grid_Column) {
            return false;
        }
        Square *grid_sq = _grid[r_new][c_new].sq;
        if(grid_sq != 0 && grid_sq->isBlock()) {
            return false;
        }
    }
    return true;
}


bool GridLayer::isRowClear(int r)
{
    for(int c=0; c<grid_Column; ++c) {
        if(_grid[r][c].sq != 0)
            return false;
    }
    return true;
}

void GridLayer::eliminateRowDone(CCNode *node)
{
    Square *sq = (Square*)node;
    sq->removeFromParent();
}

void GridLayer::eliminateRow(int r)
{
    CCLog("eliminate row %d", r);
    CCAction *removeRow = CCSequence::create(
                            CCRotateBy::create(0.1, 45),
                            CCScaleTo::create(0.1, 1.5),
                            CCScaleTo::create(0.2, 0.05),
                            CCCallFuncN::create(this, callfuncN_selector(GridLayer::eliminateRowDone)),
                            NULL
                            );

    //remove Squares in the row
    for(int c=0; c<grid_Column; ++c) {
        Square *sq = (Square*)_grid[r][c].sq;
        //sq->removeFromParent();
        sq->runAction((CCAction*)removeRow->copy()->autorelease());
        _grid[sq->_r][sq->_c].sq = 0;
    }
}

//move up rows from row to grid_Row
void GridLayer::moveUpBlockRows(int row)
{
    int r, c;

    //CCCallFuncN::create(this, callfuncN_selector(GridLayer::BlockRowMoveUpDone)),
    CCAction *moveUp = CCSequence::create(
            CCDelayTime::create(0.41),
            CCMoveBy::create(0.3, ccp(0, square_SideLen)),
            0);
    for(r=row; r<grid_Row; ++r) {
        for(c=0; c<grid_Column; ++c) {
            Square *sq = (Square*)_grid[r][c].sq;
            if(sq && sq->isBlock()) {
                _grid[r-1][c].sq = sq;
                _grid[r][c].sq = 0;
                //sq->setPosition(_grid[r-1][c].pos);
                sq->runAction((CCFiniteTimeAction*)moveUp->copy()->autorelease());
                sq->_r = r-1;
                sq->_c = c;
            }
        }
    }
}

void GridLayer::eliminateCompleteRows()
{
    int r, c;
    for(r=0; r<grid_Row; ) {
        c=0;
        while(c<grid_Column) {
            Square *sq = (Square*)_grid[r][c].sq;
            if(sq == 0 || !sq->isBlock())
                break;
            ++c;
        }

        if(c == grid_Column) {
            eliminateRow(r);
            moveUpBlockRows(r+1);
            continue;
        }
        ++r;
    }
}

void GridLayer::updateMovingSquaresRC(int r_diff, int c_diff) {
    int count = _movingSquares->count();
    bool updateGridEntry = true;

    /*
       for(int i=0; i<count; ++i) {
       Square *sq = (Square*)_movingSquares->objectAtIndex(i);
       if(sq->_r + r_diff > grid_DeadLine) {
       updateGridEntry = false;
       break;
       }
       }
       */

    if(updateGridEntry) {
        //clear current sq in grid entry
        for(int i=0; i<count; ++i) {
            Square *sq = (Square*)_movingSquares->objectAtIndex(i);
            _grid[sq->_r][sq->_c].sq = 0;
        }
    }


    for(int i=0; i<count; ++i) {
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        sq->setRC(sq->_r+r_diff, sq->_c+c_diff);

        if(updateGridEntry) {
            if( _grid[sq->_r][sq->_c].sq != 0) {
                CCLog("_grid[%d][%d].sq is not NULL", sq->_r, sq->_c);
            }
            _grid[sq->_r][sq->_c].sq = sq;
        }
    }
}

// for rotation
void GridLayer::craftInitMatrix()
{
    int count = _movingSquares->count();
    int c_min = grid_Column, c_max = 0;
    int r_min = grid_Row, r_max = 0;

    for(int i=0; i<count; ++i) {
        Square *s = (Square*)_movingSquares->objectAtIndex(i);
        c_min = (s->_c < c_min) ? s->_c : c_min;
        c_max = (s->_c > c_max) ? s->_c : c_max;
        r_min = (s->_r < r_min) ? s->_r : r_min;
        r_max = (s->_r > r_max) ? s->_r : r_max;
    }

    int r_diff = r_max - r_min + 1;
    int c_diff = c_max - c_min + 1;

    _craftMatrixN = (r_diff > c_diff) ? r_diff : c_diff;
    // (0,0) entry <----> (r_min, c_min) in global
    craftClearMatrix();
    for(int i=0; i<count; ++i) {
        Square *s = (Square*)_movingSquares->objectAtIndex(i);
        _craftMatrix[s->_r - r_min][s->_c - c_min].sq = s;
    }
}

//do rotation if it's valid
//the golbal _grid entry will be updated
//the _craftMatrix will be updated
//position of _movingSquares will be updated
void GridLayer::craftRotate()
{
    if(_state != Running || _craftMovable == false)
        return;

    GridEntry tmpMatrix[max_CraftSize][max_CraftSize];
    craftCopyMatrix(tmpMatrix, _craftMatrix);
    craftRotateMatrix(tmpMatrix);

    //check validity of rotated craft
    int r_offset, c_offset;
    craftGetOffset(r_offset, c_offset);
    CCLog("r_offset %d ,c_offset %d", r_offset, c_offset);
    if(!craftCanRotate(tmpMatrix, r_offset, c_offset)) {
        CCLog("cannot rotate");
        return;
    }

    CCLog("do rotation");
    craftCopyMatrix(_craftMatrix, tmpMatrix);

    //update _grid and _movingSquares
    int count = _movingSquares->count();
    //clear _grid for craft
    for(int i=0; i<count; ++i) {
        Square *sq = (Square*)_movingSquares->objectAtIndex(i);
        _grid[sq->_r][sq->_c].sq = 0;
    }

    for(int r=0; r<_craftMatrixN; ++r) {
        for(int c=0; c<_craftMatrixN; ++c) {
            int gr = r + r_offset;
            int gc = c + c_offset;
            if(_craftMatrix[r][c].sq) {
                Square *s = _craftMatrix[r][c].sq;
                _grid[gr][gc].sq = s;
                s->_r = gr;
                s->_c = gc;
                s->setPosition(_grid[gr][gc].pos);
            }
        }
    }
}

 // offset between _grid and _craftMatrix index
void GridLayer::craftGetOffset(int &r_offset, int &c_offset)
{
    Square *sq = (Square*)_movingSquares->objectAtIndex(0);
    int r, c;
    for(r=0; r<_craftMatrixN; ++r) {
        for(c=0; c<_craftMatrixN; ++c) {
            if(_craftMatrix[r][c].sq == sq) {
                r_offset = sq->_r - r;
                c_offset = sq->_c - c;
                return;
            }
        }
    }

    CCLog("Failed to get offset (_grid & _craftMatrix)");
    r_offset = 0;
    c_offset = 0;
}

//determine if rotate is valid based on offset between _grid and local matrix
bool GridLayer::craftCanRotate(GridEntry tmpMatrix[max_CraftSize][max_CraftSize], int r_offset, int c_offset)
{
    int r, c;
    for(r=0; r<_craftMatrixN; ++r) {
        for(c=0; c<_craftMatrixN; ++c) {
            if(tmpMatrix[r][c].sq) {
                int gr = r+r_offset;
                int gc = c+c_offset;
                if(gr>=0 && gr<grid_Row && gc>=0 && gc<grid_Column) {
                    Square *s = (Square*)_grid[gr][gc].sq;
                    if(s != 0 && s->isBlock() ) {
                        //target grid already occupied by a block
                        return false;
                    }
                }else{
                    //bounding matix out of grid
                    return false;
                }
            }
        }
    }

    return true;
}

void GridLayer::craftRotateMatrix(GridEntry a[max_CraftSize][max_CraftSize])
{
    // n x m matrix
    int n, m;
    //transpose
    for(n=0; n<_craftMatrixN; ++n)
        for(m=n+1; m<_craftMatrixN; ++m) {
            //swap [n][m] with [m][n]
            Square *tmps = a[n][m].sq;
            a[n][m].sq = a[m][n].sq;
            a[m][n].sq = tmps;
        }

    //reverse each row
    for(n=0; n<_craftMatrixN; ++n) {
        int l=0, r=_craftMatrixN-1;
        while(l<r) {
            Square *tmps = a[n][l].sq;
            a[n][l].sq = a[n][r].sq;
            a[n][r].sq = tmps;
            ++l;
            --r;
        }
    }
}

void GridLayer::craftCopyMatrix(GridEntry dst[max_CraftSize][max_CraftSize],
        GridEntry src[max_CraftSize][max_CraftSize])
{
    for(int r=0; r<_craftMatrixN; ++r) {
        for(int c=0; c<_craftMatrixN; ++c) {
            dst[r][c] = src[r][c];
        }
    }
}

void GridLayer::releaseLevelDataHead()
{
    if(_levelDataHead) {
        free(_levelDataHead);
        _levelDataHead = 0;
    }
}

void GridLayer::closeLevelDataFile()
{
    if(_levelDataFile) {
        fclose(_levelDataFile);
        _levelDataFile = 0;
    }
}

//load file head
void GridLayer::loadLevelDesc()
{
    fseek(_levelDataFile, 0, SEEK_END);
    long fsize=ftell(_levelDataFile);
    fseek(_levelDataFile, 0, SEEK_SET);

    int rdSize = sizeof(LevelFileDesc);
    _levelDataHead = (char*)malloc(rdSize); //should have been zeroed
    fread(_levelDataHead, rdSize, 1, _levelDataFile);
    _levelDataSize = fsize;
}

void GridLayer::openLevelDataFile()
{
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels.dat");
    if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
        CCLog("levels.dat didn't get packaged");
        //try to load writable level data file
        string levelPath_w = CCFileUtils::sharedFileUtils()->getWritablePath().append("levels_w.dat");
        if(CCFileUtils::sharedFileUtils()->isFileExist(levelPath_w)){
            path = levelPath_w;
        }
    }

    _levelDataFile = fopen(path.c_str(), "r");
}

void GridLayer::getLevel(int g[grid_Row][grid_Column], int level)
{
    if(!_levelDataHead) {
        loadLevelDesc();
    }

    if(_levelDataSize < sizeof(LevelFileDesc)) {
        CCLog("new level data file");
        return;
    }
    LevelFileDesc *lfd = (LevelFileDesc*)_levelDataHead;
    if(level > lfd->num_levels) {
        CCLog("level %d is greater than total level %d", level, lfd->num_levels);
        return;
    }
    int offset = lfd->level_offset[level];
    fseek(_levelDataFile, offset, SEEK_SET);
    int len = sizeof(int)*grid_Row*grid_Column;
    fread(g, 1, len, _levelDataFile);
}

void GridLayer::setLevelEntry(LevelEntry *le, 
                           GridEntry ge[grid_Row][grid_Column],
                           int bestMoves)
{
    for(int r=0; r<grid_Row; ++r) {
        for(int c=0; c<grid_Column; ++c) {
            Square *s = ge[r][c].sq;
            if(s) {
                le->g[r][c] = (int)s->_type;
            }else{
                le->g[r][c] = 0;
            }
        }
    }

    le->bestMoves = bestMoves;

}

//load level
void GridLayer::loadLevel(int n)
{
    LevelEntry le;
    memset(&le, 0, sizeof(LevelEntry));

    //try to load it from
    if(!LevelData::sharedLevelData()->getLevel(&le, n)) {
        CCLog("load level %d fail", n);
        return;
    }
#if 0
        if(n == 1) {
            int g[18][10] = {
                {1, 1, 3, 3, 3, 3, 0, 0, 4, 4},
                {1, 0, 0, 0, 0, 3, 0, 4, 4, 4},
                {1, 1, 0, 0, 2, 2, 0, 4, 4, 4},
                {1, 1, 0, 0, 2, 2, 0, 0, 1, 1},
                {1, 1, 0, 0, 2, 2, 1, 1, 1, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0}
            };
            memcpy(levelGrid, g, sizeof(g));
            _parMoves = 3;
        }else if(n == 2) {
            int g[18][10] = {
                {1, 1, 3, 3, 3, 3, 0, 0, 4, 4},
                {1, 0, 0, 0, 0, 3, 0, 4, 4, 4},
                {0, 1, 0, 0, 2, 2, 0, 4, 4, 4},
                {1, 1, 0, 0, 2, 2, 0, 4, 1, 1},
                {0, 1, 2, 0, 2, 2, 1, 1, 1, 1},
                {0, 0, 0, 0, 2, 0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0},
                {0, 0, 0,99,99,99, 99, 0, 0, 0}
            };
            memcpy(levelGrid, g, sizeof(g));
            _parMoves = 3;
        }
#endif
    
    _parMoves = le.bestMoves;
    _sensorRow = grid_Row - 3;

    for(int i=0; i<nRow; ++i) {
        for(int j=0; j<nColumn; ++j) {
            if(le.g[i][j] != 0) {
                Square *s = Square::create((Square::SquareType)le.g[i][j]);
                s->setPosition(_grid[i][j].pos);
                this->addChild(s, z_BackGround);
                s->setRC(i, j);

                if(s->isBlock()) {
                    _grid[i][j].sq = s;
                }

                //add drawing squares
                if(s->_type == Square::eDrawing) {
                    _drawingSquares->addObject(s);
                    if(i < _sensorRow) 
                        _sensorRow = i;
                }
            }
        }
    }
    
    _sensorTouched = false;

    // some init for each level
    _userMoves = 0;
    _restMoves = _parMoves;
    createMovesLabel();

    //create buttons
    _btnGo = CCSprite::createWithSpriteFrameName("button_go.png");
    _btnGo->setVisible(true);
    _btnGo->setPosition(_grid[16][8].pos);
    this->addChild(_btnGo, z_BackGround);
    _btnGoPressed = false;

    _touchBeginSquare = 0;
    _state = Drawing;
    _curLevel = n;
    createLevelLabel();
    if( LevelData::sharedLevelData()->hasHint(_curLevel) ) {
        addHintButton();
    }

    //tutorial layer
    if(n == 1) {
        _tutorial = TutorialLayer::create(1, this);
        this->addChild(_tutorial, z_ForeGround2);
        _tutorial->show(1);
    }
}

void GridLayer::addHintButton()
{
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_hint.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_hint.png");

    CCMenuItemSprite * hintItem = CCMenuItemSprite::create(
                               menuItemOff,
                               menuItemOn,
                               this,
                               menu_selector(GridLayer::showHint));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	hintItem->setPosition(ccp(label_level_X, label_level_Y - 80));
    //_labelLevel->setPosition(ccp(label_level_X, label_level_Y));
    CCMenu *pMenu = CCMenu::create(hintItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu, z_BackGround);

}

void GridLayer::showHint(CCObject* pSender)
{

}
