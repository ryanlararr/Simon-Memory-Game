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

    //Defining the base form
    const rectangle& rect = API::make_center(300, 200);
    //Removing the sizable property of the form
    form fm(rect, appearance(1, 1, 1, 1, 1, 1, 0));
    fm.caption("Simon - Ryan Arreola");
    API::window_icon(fm.handle(), paint::image("favicon.ico"));

    //Defining buttons
    button redBtn{ fm }; redBtn.bgcolor(color(255, 0, 0));
    redBtn.events().click([&fm, &playerMi, &redBtn] {
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
    button yellowBtn{ fm }; yellowBtn.bgcolor(color(200, 255, 0));
    yellowBtn.events().click([&fm, &playerFa] {
        playerFa.play();
        });

    //Layout management
    string row1 = "< <BLUE><YELLOW> >";
    string row2 = "< <RED><GREEN> >";
    fm.div("vert " + row1.append(row2));

    //Assigning buttons
    fm["BLUE"] << blueBtn; blueBtn.hide();
    fm["RED"] << redBtn; redBtn.hide();
    fm["YELLOW"] << yellowBtn; yellowBtn.hide();

    fm.bgcolor(color(0, 0, 0));
    fm.collocate();

    //Show the form
    fm.show();

    //Form for the start button
    nested_form fm2(fm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    fm2.bgcolor(color(255, 255, 255));
    fm2.fgcolor(color(0, 0, 0));
    //Start button
    button test{ fm2, "Start Game",true }; test.bgcolor(color(0, 0, 0, 100));
    test.fgcolor(color(255, 255, 255));
    fm2.div("vert <start>");
    fm2["start"] << test;

    //Form for the ingame tracking
    nested_form fm3(fm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    fm3.bgcolor(color(25, 25, 25));
    fm3.fgcolor(color(0, 0, 0));
    //Test label
    label lab{ fm3 };
    string currentStreak = "<size=12 bold white>\n-    Current Level    -</>";
    string streak = "<size=20 bold white center>\n\t\t..0</>";
    string simonText = "<size=8 bold white>\n\t\-  Simon Game CST8132  -</>";
    lab.caption(currentStreak.append(streak).append(simonText));
    lab.format(true);

    fm3.div("vert <begin>");
    fm3["begin"] << lab;

    test.events().click([&fm3, &fm2, &test] {
        fm2.hide();
        fm3.show();
        });

    fm3.collocate();
    fm2.collocate();
    fm2.show();

    exec();
}



