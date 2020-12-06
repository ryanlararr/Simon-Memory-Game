﻿
#pragma once

#ifndef NANA_ENABLE_AUDIO
#define NANA_ENABLE_AUDIO
#include <nana/push_ignore_diagnostic>
#include <nana/deploy.hpp>
#endif

//Nana directories
#include <nana/traits.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/audio/player.hpp>
#include <nana/threads/pool.hpp>
#include <nana/gui/timer.hpp>

//C++ directories
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace nana;

bool playerTurn = false; /*Dictates the ability for the player to trigger color button events.*/
vector<button*> buttons; /*Vector containing pointers to the color buttons*/
enum BtnColors {BLUE, YELLOW, RED, GREEN}; /*Values corresponding to each color. Used for sequence matching.*/
vector<int> sequence; /*Vector containing the sequence of color codes generated randomly*/

nana::audio::player playerDo("Do_Piano.wav");
nana::audio::player playerRe("Re_Piano.wav");
nana::audio::player playerMi("Mi_Piano.wav");
nana::audio::player playerFa("Fa_Piano.wav");

void playSound(int);
void lightOn(button&, int);
void lightOff(button&, int);
void debugColor(int);