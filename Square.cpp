#include "cocos2d.h"
#include "Square.h"
#include "GameConfig.h" 
#include "utils.h"

USING_NS_CC;

Square::Square()
{
    _sideLength = SC_SQUARE_SIDE_LENGTH;
    _type = eNone;
    _r = -1;
    _c = -1;
}

Square::~Square()
{
}

Square* Square::create(char *frameName)
{
    Square* sq = new Square();
    if( sq && sq->initWithSpriteFrameName( frameName ) ) {
        sq->autorelease();
        return sq;
    }
    CCLog("%s : %d create square %s failed", __func__, __LINE__, frameName);
    return 0;
}

Square* Square::create(SquareType type)
{
    Square* sq =0;
    switch(type) {
        case eDrawing:
            sq = create("SquareUnkown.png");
            break;
        case eNormalBlue:
            sq = create("SquareBlue.png");
            break;
        case eNormalRed:
            sq = create("SquareRed.png");
            break;
        case eNormalGreen:
            sq = create("SquareGreen.png");
            break;
        case eNormalPurple:
            sq = create("SquarePurple.png");
            break;
        case eNormalYellow:
            sq = create("SquareYellow.png");
            break;
        case eCraft:
            sq = create("SquareBlue.png");
            break;
        default:
            CCLog("undefined square type %d", type);
            break;
    };
    sq->_type = type;
    
    return sq;
}

bool Square::inSquare(CCPoint p)
{
    return PointInSprite(p, *this);
}

void Square::changeType(Square::SquareType newType)
{
    if(newType != _type) {
        switch(newType) {
            case eDrawing:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareUnkown.png"));
                break;
            case eNormalBlue:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareBlue.png"));
                break;
            case eNormalYellow:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareYellow.png"));
                break;
            case eNormalGreen:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareGreen.png"));
                break;
            case eNormalPurple:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquarePurple.png"));
                break;
            case eNormalRed:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareRed.png"));
                break;
            case eCraft:
                this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("SquareBlue.png"));
                break;
            default:
                CCLog("undefined square type %d", newType);
                break;
        };
        _type = newType;
    }

}

