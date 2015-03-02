//
//  UserData.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//
#include "UserData.h"

using namespace std;
static UserData *s_SharedUserData = 0;

UserData::UserData()
{
    _userData = 0;
    _userDataSize = 0;
    _userDataFile = 0;
}

UserData::~UserData()
{
    if(_userData) {
        writeUserDataFile();
        free(_userData);
        _userData = 0;
    }

    if(_userDataFile) {
        fclose(_userDataFile);
        _userDataFile = 0;
    }
}

UserData* UserData::sharedUserData(void)
{
    if(!s_SharedUserData) {
        s_SharedUserData = new UserData;
        s_SharedUserData->init();
    }

    return s_SharedUserData;
}

void UserData::init()
{
    openUserDataFile();
    loadUserData();
}

void UserData::writeUserDataFile()
{
    int rt;
    rt = fseek(_userDataFile, 0, SEEK_SET);
    rt=fwrite(_userData, sizeof(char), _userDataSize, _userDataFile);
    rt=fflush(_userDataFile);
}

void UserData::createWritableUserDataFile()
{
    _userDataFile = fopen(_path.c_str(), "wb");
    _userData = (char*)malloc(sizeof(UserFile));
    _userDataSize = sizeof(UserFile);

    memset(_userData, 0, _userDataSize);
    UserFile *uf = (UserFile*)_userData;
    uf->version = 1;
    uf->file_size = sizeof(UserFile);
    uf->num_levels_unlocked = 1;
    writeUserDataFile();

    fclose(_userDataFile);
    _userDataFile = 0;
    free(_userData);
    _userData = 0;
    _userDataSize = 0;
}

void UserData::openUserDataFile()
{
    _path = CCFileUtils::sharedFileUtils()->getWritablePath().append("user.dat");

    if(!CCFileUtils::sharedFileUtils()->isFileExist(_path)){
        //new user file
        createWritableUserDataFile();
    }
    _userDataFile = fopen(_path.c_str(), "r+");
}

void UserData::loadUserData()
{
    if(_userData) {
        free(_userData);
    }

    fseek(_userDataFile, 0, SEEK_END);
    _userDataSize = ftell(_userDataFile);
    fseek(_userDataFile, 0, SEEK_SET);

    _userData = (char*)malloc(_userDataSize);
    fread(_userData, _userDataSize, 1, _userDataFile);
}

void UserData::setUserMoves(int level, int moves)
{
    UserFile *uf = (UserFile*)_userData;
    if(level > uf->num_levels_unlocked+1 )
        return;

    if(level == uf->num_levels_unlocked + 1) {
        uf->num_levels_unlocked++;
    }

    uf->userMoves[level] = moves;
    writeUserDataFile();
}

void UserData::setNumLevelsUnlocked(int n)
{
    UserFile *uf = (UserFile*)_userData;
    uf->num_levels_unlocked = n;
    writeUserDataFile();
}

int UserData::getNumLevelsUnlocked()
{
    UserFile *uf = (UserFile*)_userData;
    return uf->num_levels_unlocked;
}

int UserData::isLevelUnlocked(int n)
{
    UserFile *uf = (UserFile*)_userData;
    if(n <= uf->num_levels_unlocked) {
        return 1;
    }else{
        return 0;
    }
}

void UserData::unlockLevel(int n)
{
    UserFile *uf = (UserFile*)_userData;
    if(n == uf->num_levels_unlocked+1 ) {
        uf->num_levels_unlocked++;
        if(uf->num_levels_unlocked > TOTAL_LEVELS ) {
           uf->num_levels_unlocked = TOTAL_LEVELS;
        }
    }
    writeUserDataFile();
}







