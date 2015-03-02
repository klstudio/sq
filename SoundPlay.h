//
//  SoundPlay.h
//  SquareCraft
//
//  Created by Jian Zhang on 2015-02-05.
//
//

#ifndef __SquareCraft__SoundPlay__
#define __SquareCraft__SoundPlay__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define sound_play_button  SimpleAudioEngine::sharedEngine()->playEffect("button.wav");
#define sound_play_levelSolved  SimpleAudioEngine::sharedEngine()->playEffect("levelSolved.wav");
#define sound_play_levelFailed  SimpleAudioEngine::sharedEngine()->playEffect("levelFailed.wav");
#define sound_play_squareTouched  SimpleAudioEngine::sharedEngine()->playEffect("squareTouched.wav");

#endif /* defined(__SquareCraft__SoundPlay__) */
