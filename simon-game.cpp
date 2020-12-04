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
    API::window_icon(fm.handle(), paint::image("favicon.ico"));

    //Defining buttons
    button redBtn{ fm }; redBtn.bgcolor(color(255, 0, 0));
    redBtn.events().click([&fm, &playerMi] {
        playerMi.play();
        });
    button greenBtn{ fm }; greenBtn.bgcolor(color(0, 255, 0));
    fm["GREEN"] << greenBtn; greenBtn.hide();
    greenBtn.events().click([&fm, &playerRe] {
        playerRe.play();
        });
    button blueBtn{ fm }; blueBtn.bgcolor(color(0, 0, 255));
    blueBtn.events().click([&fm, &playerDo] {
        playerDo.play();
        });
    button yellowBtn{ fm }; yellowBtn.bgcolor(color(175, 200, 0));
    yellowBtn.events().click([&fm, &playerFa] {
        playerFa.play();
        });

    //Layout management
    string row1 = "< <BLUE><GREEN> >";
    string row2 = "< <RED><YELLOW> >";
    fm.div("vert " + row1.append(row2));

    //Assigning buttons
    fm["BLUE"] << blueBtn; blueBtn.hide();
    fm["RED"] << redBtn; redBtn.hide();
    fm["YELLOW"] << yellowBtn; yellowBtn.hide();

    fm.bgcolor(color(0, 0, 0));
    fm.collocate();

    //Show the form
    fm.show();
    nested_form fm2(fm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    fm2.bgcolor(color(25, 25, 25));
    fm2.fgcolor(color(25, 25, 25));

    label lab{ fm2, "<size=15 top bold white max_limited>\n\tStart New \t\t\tGame</><size=10 white center>\n\t~Click me to begin~</>" };
    lab.format(true);
    lab.events().click([&fm2, &lab] {
        string currentStreak = "<size=12 bold white>\n-   Current Streak    -</>";
        string streak = "<size=20 bold white center>\n\t\t..0</>";
        string simonText = "<size=8 bold white>\n\t\-  Simon Game CST8132  -</>";
        lab.caption(currentStreak.append(streak).append(simonText));
        });
    lab.events().mouse_enter([&fm2] {
        fm2.fgcolor(color(75, 75, 5));
        });

    button test{ fm2, "test", true }; test.bgcolor(color(255, 0, 0));
    fm2.div("vert <text>");
    fm2["text"] << lab;
    fm2.collocate();
    fm2.show();

    exec();
}
