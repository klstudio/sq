//
//  LevelData.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__LevelData__
#define __SquareCraft__LevelData__

#include<stdio.h>
#include<string>
#include "cocos2d.h"
#include "GameConfig.h"

USING_NS_CC;
using namespace std;

struct LevelFileHead{
    int file_size;
    int num_levels;
    int level_offset[game_MaxLevels]; //offset from BEGINNING of the FILE
};

struct LevelEntry{
    int g[grid_Row][grid_Column];
    int bestMoves;
};

class LevelData {
    public:
        LevelData();
        ~LevelData();
        bool setLevel(LevelEntry *le, int n);
        bool setBestMoves(int bestMoves, int level);
        bool getLevel(LevelEntry *le, int n);
        static LevelData* sharedLevelData(void);
        void writeLevelDataFile();
        bool getBestMoves(int *bestMoves, int level);
        bool hasHint(int level);

    protected:
        FILE *_levelDataFile;
        long _levelDataSize;
        char *_levelData;
        string _path;

        void initLevelData();
        void initFileHead();

        void createWritableLevelData(string dst, string src);
        void init();
        void openLevelDataFile();
        void openWritableLevelDataFile();
        void openReadOnlyLevelDataFile();
};

#endif
