#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cocos2d.h"

USING_NS_CC;


class Square : public CCSprite
{
    public:
        enum SquareType{
            eNone = 0,
            eNormalYellow,
            eNormalGreen,
            eNormalPurple,
            eNormalRed,
            eNormalBlue,
            eMoving,
            eInvisible,
            eExplode1,
            eExpansion,
            eDrawing = 99,
            eCraft,      // craft square
            eCraftGoThrough1,
        }_type;

        Square();
        ~Square();

        static Square* create(char *frameName);
        static Square* create(SquareType type);
        float _sideLength;
        bool _touchable;
        //use m_obPosition for position

        bool inSquare(CCPoint p);
        void changeType(SquareType type);

        bool isNormal() {
            return (_type >= eNormalYellow && _type <= eNormalBlue);
        }

        bool isCraft() {
            return (_type >= eCraft && _type <= eCraftGoThrough1);
        }
    
        // block is squares at the puzzle stage
        bool isBlock() {
            return isNormal();
        }

        void setVelocity(CCPoint &v);
        void getVelocity(CCPoint &v);

        //coordinates of four sides
        float left() {
            //m_obContentSize.width/height
            //m_obPosition
            return (m_obPosition.x - m_obContentSize.width/2.0);
        }

        float right() {
            return (m_obPosition.x + m_obContentSize.width/2.0);
        }

        float bottom() {
            return (m_obPosition.y - m_obContentSize.height/2.0);
        }
        float top() {
            return (m_obPosition.y + m_obContentSize.height/2.0);
        }

        //data for GridLayer
        int _r;  //row index
        int _c;  //column index
        void setRC(int r, int c) {
            _r = r;
            _c = c;
        }
};


#endif  //__SQUARE_H__

