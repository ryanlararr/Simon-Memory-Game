#include "simon-game.h"

using namespace std;
using namespace nana;

int main() {

    //Definining audio files
    nana::audio::player playerDo("Do_Piano.wav");
    nana::audio::player playerRe("Re_Piano.wav");
    nana::audio::player playerMi("Mi_Piano.wav");
    nana::audio::player playerFa("Fa_Piano.wav");

    int i = 0;
    int colorCode;
    timer seqDisplayTimer(chrono::milliseconds(2000));
    seqDisplayTimer.elapse([&] {
        sequence.push_back(rand() % 4);
        if (i < sequence.size()) {
            colorCode = sequence.at(i);

            if (i > 0 && sequence.at(i-1.0) != sequence.at(i)) { lightOff(*buttons.at(sequence.at(i - 1.0)), sequence.at(i-1.0)); }

            if (i < sequence.size()) {
                lightOn(*buttons.at(sequence.at(i)), sequence.at(i));
                playSound(colorCode);
                cout << "Playing ";
                switch (colorCode) {
                case BLUE:
                    cout << "blue\n";
                    break;
                case RED:
                    cout << "red\n";
                    break;
                case YELLOW:
                    cout << "yellow\n";
                    break;
                case GREEN:
                    cout << "green\n";
                    break;
                default:
                    break;
                }
            }
            else {
                seqDisplayTimer.stop();
                cout << "End sequence" << endl;
            }
        }
        i++;
        });

    //Defining the base form
    const rectangle& rect = API::make_center(300, 200);
    //Removing the sizable property of the form
    form mainFm(rect, appearance(1, 1, 1, 1, 1, 1, 0));
    mainFm.caption("Simon - Ryan Arreola");
    API::window_icon(mainFm.handle(), paint::image("favicon.ico"));

    //Defining buttons
    button redBtn{ mainFm }; redBtn.bgcolor(color(255, 0, 0));
    redBtn.events().click([&] {
        if (playerTurn) {
            playerMi.play();
        }
        });
    button greenBtn{ mainFm }; greenBtn.bgcolor(color(0, 100, 0));
    greenBtn.events().click([&] {
        if (playerTurn) {
            playerRe.play();
        }
        });
    button blueBtn{ mainFm }; blueBtn.bgcolor(color(0, 0, 255));
    blueBtn.events().click([&] {
        if (playerTurn) {
            playerDo.play();
        }
        });
    button yellowBtn{ mainFm }; yellowBtn.bgcolor(color(200, 200, 0));
    yellowBtn.events().click([&] {
        if (playerTurn) {
            playerFa.play();
        }
        });

    //Adding button addresses to the button vector
    buttons.insert(buttons.end(), {
        &blueBtn,
        &yellowBtn,
        &redBtn,
        &greenBtn });

    //Layout management
    string row1 = "< <BLUE><YELLOW> >";
    string row2 = "< <RED><GREEN> >";
    mainFm.div("vert " + row1.append(row2));

    //Assigning buttons
    mainFm["BLUE"] << blueBtn;
    mainFm["RED"] << redBtn;
    mainFm["YELLOW"] << yellowBtn;
    mainFm["GREEN"] << greenBtn;

    mainFm.bgcolor(color(0, 0, 0));
    mainFm.collocate();

    //Show the form
    mainFm.show();

    //Form for the start button
    nested_form startFm(mainFm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    startFm.bgcolor(color(255, 255, 255));
    startFm.fgcolor(color(0, 0, 0));
    //Start button
    button startBtn{ startFm, "Start Game",true }; startBtn.bgcolor(color(0, 0, 0, 100));
    startBtn.fgcolor(color(255, 255, 255));
    startFm.div("vert <start>");
    startFm["start"] << startBtn;

    //Form for the ingame tracking
    nested_form labelFm(mainFm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    labelFm.bgcolor(color(25, 25, 25));
    labelFm.fgcolor(color(0, 0, 0));

    //Game label
    label lab{ labelFm };
    lab.format(true);

    labelFm.div("vert <begin>");
    labelFm["begin"] << lab;

    startBtn.events().click([&] {
        startFm.hide();
        labelFm.show();
        seqDisplayTimer.start();
        });

    labelFm.collocate();
    startFm.collocate();
    startFm.show();

    exec();
}

/*
* Sets the current streak on the game label.
*/
void setStreak(int streak, label& lab) {
    lab.caption(
        labelHeader.append(
            streak1d).append(
                to_string(streak)).append(
                    simonText)
    );
}

void playSound(int colorId) {
    switch (colorId) {
    case BLUE:
        playerDo.play();
        break;
    case YELLOW:
        playerFa.play();
        break;
    case RED:
        playerMi.play();
        break;
    case GREEN:
        playerRe.play();
        break;
    default:
        cout << "Error. Exiting.";
        exit(EXIT_FAILURE);
    }
}


void lightOn(button& btn, int colorId) {
    switch (colorId) {
    case BLUE:
        btn.bgcolor(color(128, 128, 255));
        break;
    case YELLOW:
        btn.bgcolor(color(255, 255, 128));
        break;
    case RED:
        btn.bgcolor(color(255, 128, 128));
        break;
    case GREEN:
        btn.bgcolor(color(128, 255, 128));
        break;
    default:
        cout << "Error. Exiting.";
        exit(EXIT_FAILURE);
    }
}

void lightOff(button& btn, int colorId) {
    switch (colorId) {
    case BLUE:
        btn.bgcolor(color(0, 0, 255));
        break;
    case YELLOW:
        btn.bgcolor(color(200, 200, 0));
        break;
    case RED:
        btn.bgcolor(color(255, 0, 0));
        break;
    case GREEN:
        btn.bgcolor(color(0, 100, 0));
        break;
    default:
        cout << "Error. Exiting.";
        exit(EXIT_FAILURE);
    }
}

