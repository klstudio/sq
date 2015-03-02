//
//  LevelData.cpp
//  SquareCraft
//
//  Created by Jian Zhang on 2014-12-07.
//
//

#include<string>
#include<fstream>

#include "LevelData.h"

using namespace std;
static LevelData *s_SharedLevelData = 0;

LevelData::LevelData()
{
    _levelDataFile = 0;
    _levelDataSize = 0;
    _levelData = 0;
}

LevelData::~LevelData()
{
    if(_levelData) {
        writeLevelDataFile();
        free(_levelData);
        _levelData = 0;
    }

    if(_levelDataFile) {
        fclose(_levelDataFile);
        _levelDataFile = 0;
    }
}

bool LevelData::hasHint(int level)
{
    if( level == 3 || level == 7
        || level == 8 ) {
        return true;
    }

    return false;
}

LevelData* LevelData::sharedLevelData(void)
{
    if(!s_SharedLevelData) {
        s_SharedLevelData = new LevelData();
        s_SharedLevelData->init();
    }

    return s_SharedLevelData;
}

void LevelData::openLevelDataFile()
{
    if(PRODUCT_DEV) {
        openWritableLevelDataFile();
    }else{
        openReadOnlyLevelDataFile();
    }
}

void LevelData::init()
{
    openLevelDataFile();
}

void LevelData::createWritableLevelData(string dstfn, string srcfn)
{
    std::ifstream  src(srcfn.c_str(), std::ios::binary);
    std::ofstream  dst(dstfn.c_str(),   std::ios::binary);
    dst << src.rdbuf();
}

void LevelData::openWritableLevelDataFile()
{

    string levelPath_w = CCFileUtils::sharedFileUtils()->getWritablePath().append("levels_w.dat");

    if(!CCFileUtils::sharedFileUtils()->isFileExist(levelPath_w)){
        // need copy a levels.dat and use that
        string path_src = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels.dat");
        if(!CCFileUtils::sharedFileUtils()->isFileExist(path_src)){
            CCLog("levels.dat didn't get packaged");
        }
        createWritableLevelData(levelPath_w, path_src);
    }

    _levelDataFile = fopen(levelPath_w.c_str(), "r+");
    _path = levelPath_w;
}

void LevelData::openReadOnlyLevelDataFile()
{
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels.dat");
    if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
        CCLog("levels.dat didn't get packaged");
    }

    _levelDataFile = fopen(path.c_str(), "r");
    _path = path;
}

bool LevelData::getLevel(LevelEntry *le, int level)
{
    LevelFileHead *lfh=0; 
    if(!_levelData) {
        //load/create all level data
        initLevelData();
    }

    lfh = (LevelFileHead*)_levelData;
    if(level <= lfh->num_levels) {
        int offset = lfh->level_offset[level];
        //copy level data in ram
        memcpy(le, _levelData+offset, sizeof(LevelEntry));
    }else {
        CCLog("level %d is greater than total level %d", level, lfh->num_levels);
        return false;
    }

    return true;
}

//update best moves only
bool LevelData::setBestMoves(int bestMoves, int level)
{
    if(!_levelData) {
        initLevelData();
    }

    LevelFileHead *lfh = (LevelFileHead*)_levelData;
    if(level > lfh->num_levels) {
        CCLog("level not exits yet");
        return false;
    }
    LevelEntry *le = (LevelEntry*)(_levelData + lfh->level_offset[level]);
    le->bestMoves = bestMoves;
}

bool LevelData::getBestMoves(int *bestMoves, int level)
{
    //to do:
}

bool LevelData::setLevel(LevelEntry *le, int level)
{
    if(!_levelData) {
        initLevelData();
    }

    LevelFileHead *lfh = (LevelFileHead*)_levelData;
    if(level > lfh->num_levels+1) {
        CCLog("can only add levels increasingly");
        return false;
    }

    int len = sizeof(LevelEntry);
    if(level == lfh->num_levels+1) {
        //add new level
        lfh->num_levels++;
        lfh->level_offset[level] = _levelDataSize;
        _levelDataSize += len;
        lfh->file_size += len;
    }
    char *dst = (char*)_levelData + lfh->level_offset[level];
    memcpy(dst, (char*)le, len);
}

void LevelData::initLevelData()
{
    if(_levelData) {
        free(_levelData);
    }

    //check file size
    fseek(_levelDataFile, 0, SEEK_END);
    _levelDataSize = ftell(_levelDataFile);
    fseek(_levelDataFile, 0, SEEK_SET);

    //load all data in unless this is getting too big
    long maxDataSize = game_MaxLevels * sizeof(LevelEntry) + sizeof(LevelFileHead);
    _levelData = (char*)malloc(maxDataSize);
    fseek(_levelDataFile, 0, SEEK_SET);
    fread(_levelData, _levelDataSize, 1, _levelDataFile);

    if(_levelDataSize < sizeof(LevelFileHead)) {
        //new file or corrupted file
        CCLog("new level data file");
        //create file head and flush in file
        initFileHead();
    }
}

void LevelData::initFileHead()
{
    LevelFileHead *lfh = (LevelFileHead*)_levelData;
    memset( (char*)lfh, 0, sizeof(LevelFileHead));

    lfh->num_levels = 0;
    _levelDataSize = sizeof(LevelFileHead);
    lfh->file_size = _levelDataSize;
}

void LevelData::writeLevelDataFile()
{
    int rt;
    rt = fseek(_levelDataFile, 0, SEEK_SET);
    CCLog("fseek return %d",rt);
    rt=fwrite(_levelData, sizeof(char), _levelDataSize, _levelDataFile);
    CCLog("fwrite return %d",rt);
    rt=fflush(_levelDataFile);
    CCLog("fflush return %d",rt);

    //may need close/reopen
    //fclose(_levelDataFile);
}



