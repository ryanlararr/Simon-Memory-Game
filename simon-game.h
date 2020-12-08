
#pragma once

#ifndef NANA_ENABLE_AUDIO_PLAYER_HPP
#define NANA_ENABLE_AUDIO_PLAYER_HPP
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
vector<audio::player*> audioPlayers; /*Vector containing audio players*/
enum BtnColors { BLUE, YELLOW, RED, GREEN, WRONG }; /*Values corresponding to each color. Used for sequence matching.*/
vector<int> sequence; /*Vector containing the sequence of color codes generated randomly*/

//Audio player declarations
audio::player playerDo("resources/Do_Piano.wav");
audio::player playerRe("resources/Re_Piano.wav");
audio::player playerMi("resources/Mi_Piano.wav");
audio::player playerFa("resources/Fa_Piano.wav");
audio::player playerWrong("resources/Wrong.wav");
nana::threads::pool pool(1);

//Define timer for user input
timer inputTimer(chrono::milliseconds(5000)); /*Timer for user input. Set for 5 seconds.*/
timer seqDisplayTimer(chrono::milliseconds(500)); /*Timer for light display. Set for 5ms*/

int displayIdx = 0; /*Tracks index of the display sequence*/
int inputIdx = 0; /*Tracks index of user input sequence*/
char colorIdx = 0; /*Sets the colors for the button events 0-3*/

void lightOn(button&, int);
void lightOff(button&, int);
void setBtnEvent(button* templBtn, int colorCode, label& gameLabel, nested_form& gameFm);
void viewMenu(nested_form& gameFm, bool view);