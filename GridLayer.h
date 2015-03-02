#ifndef __Grid_H__
#define __Grid_H__

#include "cocos2d.h"
#include "Square.h"
#include "GameConfig.h"
#include "LevelData.h"
#include "SoundPlay.h"

USING_NS_CC;

struct LevelFileDesc{
    int file_size;
    int num_levels;
    //offset from beginning of file
    int level_offset[1024];     // assume maximum 1024 levels
};

struct GridEntry{
    int r;  // row index
    int c;  // column index
    Square *sq;
    CCPoint pos;    // position for square
};

class TutorialLayer;
// nRow by nColumn  grid
class GridLayer : public CCLayer
{
    public:
        enum State{
            Invalid,
            Drawing,
            Running,    //craft done.
            Pause,      //won't take any input or make any automatical progress
            NoInput     // doing animation/action transit to other state when done
        }_state;
        State _saveState;
        bool _craftMovable;

        enum Gesture{
            None,
            LeftSwipe,
            RightSwipe,
            UpSwipe,
            DownSwipe
        }_gesture;

        int nRow;
        int nColumn;
        CCPoint _pos;   //(left, bottom)
        float _sideLen;

        GridLayer();
        ~GridLayer();

        static GridLayer *create();
        virtual void draw();
        virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
        virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

        virtual void loadLevel(int n);
        virtual void step(float dt);
        virtual void openLevelDataFile();
        virtual void onExit();

        void pause() {
            if(_state != Pause) {
                _saveState = _state;
                _state = Pause;
            }
        }

        int isPaused() {
            return (_state == Pause);
        }

        void resume() {
            if(_state == Pause) {
                _state = _saveState;
            }else{
                CCLog("resume gridlayer when _state is not Pause!");
            }
        }

        //CCArray _puzzleSquares;
        GridEntry _grid[grid_Row][grid_Column];

        //used to detect squares collide to drawing square
        int _sensorRow;
        int _sensorTouched;

        //hint state
        int _hintOn;

    protected:
        void initGrid();    //setup all GridEntries
        CCArray *_drawingSquares;
        Square *_touchBeginSquare;

        int _curLevel;
        TutorialLayer *_tutorial;
        //level data
        FILE *_levelDataFile;
        char* _levelDataHead;
        long _levelDataSize;

        void loadLevelDesc();
        void closeLevelDataFile();
        void releaseLevelDataHead();
        void getLevel(int g[grid_Row][grid_Column], int level);
        void setLevelEntry(LevelEntry *le,
                           GridEntry ge[grid_Row][grid_Column],
                           int bestMoves);

        CCSprite *_btnGo;   // Go button for launch square craft
        bool _btnGoPressed;

        //labels and related animation
        CCLabelTTF *_labelUserMoves;
        //CCLabelTTF * _labelParMoves;
        CCSprite *_capMoves;
        
        int _userMoves;
        int _restMoves;
        void createMovesLabel();
        void updateMovesLabel() {
            char szValue[10]={0};
            //sprintf(szValue, "%i", _userMoves);
            sprintf(szValue, "%i", _restMoves);
            _labelUserMoves->setString(szValue);
        }
        void increaseUserMoves();
        void decreaseRestMoves() {
            _restMoves--;
            _restMoves = (_restMoves < 0) ? 0 : _restMoves;
        }
        void flyToUserMovesLabelDone(CCNode *node);

        int _parMoves;

        CCLabelTTF *_labelLevel;
        void createLevelLabel();


        CCPoint _touchBeginPos;
        CCPoint _touchEndPos;

        CCAction *_craftMoveUp;

        void processGesture();  // move according to gesture
        void createActions();

        //Game play rule
        void eliminateRow(int row);
        bool isRowClear(int r);
        void eliminateRowDone(CCNode* node);
        void moveUpBlockRows(int row);
        void eliminateCompleteRows();
        bool isLevelCleared();
        bool isLevelFailed();
        void completeTouchDown();

        //data and functions for movingsquares/crfat
        CCArray *_movingSquares;
        //bounding box of craft in terms of grid (row, column)
        GridEntry _craftMatrix[max_CraftSize][max_CraftSize];  //local matrix
        int _craftMatrixN;  // NxN matrix

        float _floatingTime;
        bool createMovingSquares(); // based on _drawingSquares
        bool craftCanMove(int r_diff, int c_diff);
        void craftMoveUp();
        void craftMoveLeft();
        void craftMoveRight();
        void craftMoveUpDone();
        void craftInitMatrix();

        //functions for rotation
        void craftRotate();
        bool craftCanRotate(GridEntry tmpMatrix[max_CraftSize][max_CraftSize], int r_offset, int c_offset);
        void craftGetOffset(int &r_offset, int &c_offset);  // offset between _grid and _craftMatrix index
        void craftRotateMatrix(GridEntry m[max_CraftSize][max_CraftSize]);

        void craftCopyMatrix(GridEntry dst[max_CraftSize][max_CraftSize],
                GridEntry src[max_CraftSize][max_CraftSize]);

        void craftClearMatrix() {
            for(int r=0; r<_craftMatrixN; ++r) {
                for(int c=0; c<_craftMatrixN; ++c) {
                    _craftMatrix[r][c].r = r;
                    _craftMatrix[r][c].c = c;
                    _craftMatrix[r][c].sq = 0;
                    _craftMatrix[r][c].pos = ccp(0,0);
                }
            }
        }

        void clearMovingSquares() {
            _movingSquares->removeAllObjects();
        }

        void convertMovingSuqaresToBlock() {
            int count = _movingSquares->count();
            for(int i=0; i<count; ++i) {
                Square *sq = (Square*)_movingSquares->objectAtIndex(i);
                sq->changeType(Square::eNormalYellow);
                _grid[sq->_r][sq->_c].sq = sq;
            }
        }

        int collideWithSensorRow() {
            int count = _movingSquares->count();
            for(int i=0; i<count; ++i) {
                Square *sq = (Square*)_movingSquares->objectAtIndex(i);
                if(sq->_r >= _sensorRow)
                    return 1;
            }
            return 0;
        }

        void updateMovingSquaresRC(int r_diff, int c_diff);
        void showHint(CCObject* pSender);
        void addHintButton();
};

#endif
