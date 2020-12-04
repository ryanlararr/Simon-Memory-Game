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
    fm["BLUE"] << blueBtn; blueBtn.hide();
    fm["GREEN"] << greenBtn; greenBtn.hide();
    fm["RED"] << redBtn; redBtn.hide();
    fm["YELLOW"] << yellowBtn; yellowBtn.hide();
    fm.bgcolor(color(0, 0, 0));
    fm.collocate();

    
    nested_form fm2(fm, rectangle(75, 50, 150, 100) ,appearance(0, 0, 0, 1, 0, 1, 0));
    fm2.bgcolor(color(25, 25, 25));
    fm2.fgcolor(color(25, 25, 25));

    label lab{ fm2, "<size=15 top bold white max_limited>\n\tStart New \t\t\tGame</><size=10 white center>\n\t~Click me to begin~</>" };
    lab.format(true);
    lab.events().click([&fm] {
        fm.close();
        });
    lab.events().mouse_enter([&fm2] {
        fm2.fgcolor(color(75, 75, 5));
        });
    
    button test{ fm2, "test", true}; test.bgcolor(color(255, 0, 0));
    fm2.div("vert <text>");
    fm2["text"] << lab;
    
    fm2.collocate();

    //Show the form
    fm.show();
    fm2.show();
    
    //Start to event loop process, it blocks until the form is closed.
    exec();
}
