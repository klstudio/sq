//
//  GameConfig.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-11-25.
//
//

#ifndef SquareCraft_GameConfig_h
#define SquareCraft_GameConfig_h

#define design_Size_Width   (720)
#define design_Size_Height  (1280)
#define center_position ccp(design_Size_Width/2, design_Size_Height/2)

//enable editor for none-release version
#define PRODUCT_DEV (0)
#define PRODUCT_RELEASE (1)

#define Lite_Version   1
//#define Lite_Version 0

//level data
#ifdef Lite_Version
#define TOTAL_LEVELS 8
#define LEVELS_PER_SCREEN 8
#define level_rows 2
#define level_cols 4
#else
#define TOTAL_LEVELS 60
#define LEVELS_PER_SCREEN 20
#define level_rows 5
#define level_cols 4
#endif

//gesture
#define gesture_MinDistance (30)

//grid area
#define square_SideLen (60.0)
#define GRID_NUM_COLUMN (10)
#define GRID_NUM_ROW (10)
#define grid_Left (60.0)
//#define grid_Bottom (180.0)
#define grid_Bottom (5.0)
#define grid_Row (18)
#define grid_Column (10)
#define grid_DeadLine (7)

//square craft area
#define SC_SQUARE_SIDE_LENGTH (60.0)        //square side length need to be even number
#define SC_DRAWINGPANEL_X (3*60+30)
#define SC_DRAWINGPANEL_Y (100)

#define v_MovingSquaresInit (ccp(0, SC_SQUARE_SIDE_LENGTH))

#define sc_FloatingDuration (0.8)

#define max_CraftSize    (10)

//UI
#define cap_moves_position  ccp(grid_Left+grid_Column*0.5*square_SideLen+90, \
                                design_Size_Height-146/2-3)
//#define label_UserMoves_X (grid_Left+grid_Column*square_SideLen-64)
#define label_UserMoves_X (grid_Left+grid_Column*0.5*square_SideLen+90)
#define label_UserMoves_Y (design_Size_Height-146/2-28)
#define label_BestMoves_X (60 + 64)
#define label_BestMoves_Y (grid_Bottom+grid_Row*square_SideLen+96)

#define label_level_X (cap_moves_position.x - 230)
#define label_level_Y (cap_moves_position.y + 30)

//game levels
#define game_MaxLevels (1024)

//Z order
enum {
    z_BackGround2,
    z_BackGround,   
    z_MiddleGround,
    z_ForeGround,
    z_ForeGround2,
};

#endif

