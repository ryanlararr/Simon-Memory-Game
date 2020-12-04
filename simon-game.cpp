#ifndef NANA_ENABLE_AUDIO
#define NANA_ENABLE_AUDIO
#include <nana/push_ignore_diagnostic>
#include <nana/deploy.hpp>
#endif

#include "simon-game.h"

#include <nana/traits.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/audio/player.hpp>
#include <nana/threads/pool.hpp>

using namespace std;
using namespace nana;

int main()
{
    //Definining audio files
    nana::audio::player playerDo("Do_Piano.wav");
    nana::audio::player playerRe("Re_Piano.wav");
    nana::audio::player playerMi("Mi_Piano.wav");
    nana::audio::player playerFa("Fa_Piano.wav");

    //Define a form.
    form fm;
    fm.caption("Simon - Ryan Arreola");

    //Defining buttons
    button redBtn{ fm}; redBtn.bgcolor(color(255, 0, 0));
    redBtn.events().click([&fm, &playerMi] {
        playerMi.play();
        });
    button greenBtn{ fm}; greenBtn.bgcolor(color(0, 255, 0));
    fm["GREEN"] << greenBtn; greenBtn.hide();
    greenBtn.events().click([&fm, &playerRe] {
        playerRe.play();
        });
    button blueBtn{ fm}; blueBtn.bgcolor(color(0, 0, 255));
    blueBtn.events().click([&fm, &playerDo] {
        playerDo.play();
        });
    button yellowBtn{ fm}; yellowBtn.bgcolor(color(175, 200, 0));
    yellowBtn.events().click([&fm, &playerFa] {
        playerFa.play();
        });

    //Layout management
    string row1 = "< <BLUE><GREEN> >";
    string row2 = "< <RED><YELLOW> >";
    fm.div("vert "+row1.append(row2));

    //Assigning buttons
    fm["BLUE"] << blueBtn; blueBtn.hide();
    fm["RED"] << redBtn; redBtn.hide();    
    fm["YELLOW"] << yellowBtn; yellowBtn.hide();
    
    fm.bgcolor(color(0, 0, 0));
    fm.collocate();

    //Show the form
    fm.show();
    
    exec();
}
