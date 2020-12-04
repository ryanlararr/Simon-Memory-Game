#ifndef NANA_AUDIO_PLAYER_HPP
#define NANA_AUDIO_PLAYER_HPP
#endif

#include <nana/push_ignore_diagnostic>
#include <nana/deploy.hpp>

#include "simon-game.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/audio/player.hpp>
#include <nana/threads/pool.hpp>

#include <nana/paint/graphics.hpp>

using namespace std;
using namespace nana;

int main()
{
    //Define a form.
    form fm;
    fm.caption("Simon - Ryan Arreola");

    //Defining buttons
    button redBtn{ fm}; redBtn.bgcolor(color(255, 0, 0));
    button greenBtn{ fm}; greenBtn.bgcolor(color(0, 255, 0));
    button blueBtn{ fm}; blueBtn.bgcolor(color(0, 0, 255));
    button yellowBtn{ fm}; yellowBtn.bgcolor(color(175, 200, 0));

    //Layout management
    string row1 = "< <BLUE><GREEN> >";
    string row2 = "< <RED><YELLOW> >";
    fm.div("vert "+row1.append(row2));

    //Assigning buttons
    fm["BLUE"] << blueBtn;
    fm["GREEN"] << greenBtn;
    fm["RED"] << redBtn;
    fm["YELLOW"] << yellowBtn;
    fm.bgcolor(color(0, 0, 0));
    fm.collocate();

    //Show the form
    fm.show();
    
    //Start to event loop process, it blocks until the form is closed.
    exec();
}
