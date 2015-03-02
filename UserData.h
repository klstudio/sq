//
//  UserData.h
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#ifndef __SquareCraft__UserData__
#define __SquareCraft__UserData__

#include<string>
#include<fstream>
#include<stdio.h>
#include "cocos2d.h"
#include "GameConfig.h"
using namespace std;
using namespace cocos2d;
struct UserFile{
    int version;
    int file_size;
    int num_levels_unlocked;
    int userMoves[game_MaxLevels]; //user score
};


class UserData {
    public:
        UserData();
        ~UserData();
        static UserData* sharedUserData(void);
        void writeUserDataFile();
        void setUserMoves(int level, int moves);
        int getNumLevelsUnlocked();
        int isLevelUnlocked(int n);
        void setNumLevelsUnlocked(int n);
        void unlockLevel(int n);

    protected:
        FILE *_userDataFile;
        long _userDataSize;
        char *_userData;
        string _path;

        void loadUserData();
        void createWritableUserDataFile();
        void init();
        void openUserDataFile();
};

#endif

