#include "simon-game.h"

int main() {

    //Defining the base form
    const rectangle& rect = API::make_center(300, 200);
    form mainFm(rect, appearance(1, 1, 1, 1, 1, 1, 0));
    mainFm.caption("Simon - Ryan Arreola");
    API::window_icon(mainFm.handle(), paint::image("resources/favicon.ico"));

    //Storing the audio players for use in event handling
    audioPlayers.insert(audioPlayers.end(), { &playerDo, &playerRe, &playerMi, &playerFa, &playerWrong });

    //Defining buttons
    button blueBtn{ mainFm }; blueBtn.bgcolor(color(0, 0, 255));
    button yellowBtn{ mainFm }; yellowBtn.bgcolor(color(200, 200, 0));
    button redBtn{ mainFm }; redBtn.bgcolor(color(255, 0, 0));
    button greenBtn{ mainFm }; greenBtn.bgcolor(color(0, 100, 0));
    
    //Adding button addresses to the button vector
    buttons.insert(buttons.end(), { &blueBtn, &yellowBtn, &redBtn, &greenBtn });

    //Layout management
    mainFm.div("vert < <BLUE><YELLOW> >< <RED><GREEN> >");

    //Assigning buttons
    mainFm["BLUE"] << blueBtn;
    mainFm["RED"] << redBtn;
    mainFm["YELLOW"] << yellowBtn;
    mainFm["GREEN"] << greenBtn;

    mainFm.collocate();
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
    nested_form gameFm(mainFm, rectangle(75, 50, 150, 100), appearance(0, 0, 0, 1, 0, 1, 0));
    gameFm.bgcolor(color(25, 25, 25));
    gameFm.fgcolor(color(0, 0, 0));

    //Game label
    label gameLabel{ gameFm };
    gameLabel.format(true);
    button quitBtn{ gameFm, "Quit",true };
    button restartBtn{ gameFm, "Restart Game",true };

    //Layout management of the forms
    gameFm.div("vert <HEAD>< <PLAY><QUIT> >");
    gameFm["HEAD"] << gameLabel;
    gameFm["PLAY"] << restartBtn;
    gameFm["QUIT"] << quitBtn;

    //Hide the widgets until game over is reached
    viewMenu(gameFm, false);

    gameFm.collocate();
    startFm.collocate();
    startFm.show();

    /************ Event Handling ************/

    //Setting the button events for each button
    for each (button* btn in buttons) { setBtnEvent(btn, colorIdx++, gameLabel, gameFm); }

    seqDisplayTimer.elapse([&] {
        //If display is at the first index of the sequence there is no previous light to turn off
        if (displayIdx > 0) {
            lightOff(*buttons.at(sequence.at(displayIdx - 1)), sequence.at(displayIdx - 1));
        }
        //As long as the index is within the range of the sequence vector there is a color to be represented
        if (displayIdx < sequence.size()) {
            //Turn on the light of a button in the button vector based on the color code at the current index
            lightOn(*buttons.at(sequence.at(displayIdx)), sequence.at(displayIdx));
            displayIdx++;
        }
        else {
            seqDisplayTimer.stop();
            playerTurn = true;
            //Starts timer for user input
            inputTimer.start();
        }
        });

    //If the user does not input within 5 seconds they lose the ability to trigger button events and the error sound plays.
    inputTimer.elapse([&] {
        playerTurn = false;
        audioPlayers[WRONG]->play();
        inputTimer.stop();

        gameLabel.caption("<bold white>Sorry, you took over 5 seconds.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
        viewMenu(gameFm, true);
        gameFm.collocate();
        });

    startBtn.events().click([&] {
        //Hide the initial form and show the form containing end game information
        startFm.hide();
        gameFm.show();

        //Generate random number into the sequence and start the timer
        std::srand((unsigned int)time(NULL));
        sequence.push_back(rand() % 4);
        playerTurn = false;
        seqDisplayTimer.start();
        });

    restartBtn.events().click([&] {
        //Reset the sequence properties and index trackers
        displayIdx = inputIdx = 0;
        sequence.clear();

        viewMenu(gameFm, false);
        playerTurn = false;

        //Add first random button to the sequence and start the display generation
        std::srand((unsigned int)time(NULL));
        sequence.push_back(rand() % 4);
        seqDisplayTimer.start();
        });

    //End the program
    quitBtn.events().click([&] {
        mainFm.close();
        });

    //Execute Nana
    nana::exec();
}

void setBtnEvent(button *templBtn, int colorCode, label& gameLabel, nested_form& gameFm) {
    templBtn->events().click([colorCode, &gameLabel, &gameFm] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == colorCode) {
                audioPlayers[colorCode]->play();
                if (inputIdx == sequence.size() - 1) {
                    playerTurn = false;
                    displayIdx = inputIdx = 0;
                    inputTimer.stop();
                    std::srand((unsigned int)time(NULL));
                    sequence.push_back(rand() % 4);
                    seqDisplayTimer.start();
                }
                else {
                    inputTimer.start();
                    inputIdx++;
                }
            }
            else {
                inputTimer.stop();
                audioPlayers[WRONG]->play();
                gameLabel.caption("<bold white>Sorry, Wrong Color.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
                viewMenu(gameFm, true);
                playerTurn = false;
                gameFm.collocate();
            }
        }
        });
}

void lightOn(button& btn, int colorCode) {
    switch (colorCode) {
    case BLUE:
        btn.bgcolor(color(128, 128, 255));
        audioPlayers[BLUE]->play();
        break;
    case YELLOW:
        btn.bgcolor(color(255, 255, 128));
        audioPlayers[YELLOW]->play();
        break;
    case RED:
        btn.bgcolor(color(255, 128, 128));
        audioPlayers[RED]->play();
        break;
    case GREEN:
        btn.bgcolor(color(128, 255, 128));
        audioPlayers[GREEN]->play();
        break;
    }
}

void lightOff(button& btn, int colorCode) {
    switch (colorCode) {
    case BLUE: btn.bgcolor(color(0, 0, 255));
        break;
    case YELLOW: btn.bgcolor(color(200, 200, 0));
        break;
    case RED: btn.bgcolor(color(255, 0, 0));
        break;
    case GREEN: btn.bgcolor(color(0, 100, 0));
        break;
    }
}

void viewMenu(nested_form& gameFm, bool view) {
    gameFm.get_place().field_display("PLAY", view);
    gameFm.get_place().field_display("QUIT", view);
    gameFm.get_place().field_display("HEAD", view);
}