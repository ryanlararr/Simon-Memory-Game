#include "simon-game.h"

int main() {
    nana::audio::player playerDo("Do_Piano.wav");
    nana::audio::player playerRe("Re_Piano.wav");
    nana::audio::player playerMi("Mi_Piano.wav");
    nana::audio::player playerFa("Fa_Piano.wav");
    nana::audio::player playerWrong("Wrong.wav");
    nana::threads::pool pool(1);

    //Define timer for user input
    timer inputTimer(chrono::milliseconds(5000));

    //Declaring indexes to reference location in the sequence
    int displayIdx = 0;
    int inputIdx = 0;

    timer seqDisplayTimer(chrono::milliseconds(500));

    //Defining the base form
    const rectangle& rect = API::make_center(300, 200);
    //Removing the sizable property of the form
    form mainFm(rect, appearance(1, 1, 1, 1, 1, 1, 0));
    mainFm.caption("Simon - Ryan Arreola");
    API::window_icon(mainFm.handle(), paint::image("favicon.ico"));


    //Defining buttons
    button redBtn{ mainFm }; redBtn.bgcolor(color(255, 0, 0));
    button greenBtn{ mainFm }; greenBtn.bgcolor(color(0, 100, 0));
    button blueBtn{ mainFm }; blueBtn.bgcolor(color(0, 0, 255));
    button yellowBtn{ mainFm }; yellowBtn.bgcolor(color(200, 200, 0));

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
    button quitBtn{ labelFm, "Quit",true };
    button restartBtn{ labelFm, "Restart Game",true };

    //Layout management of the forms
    labelFm.div("vert <begin>< <PLAY><QUIT> >");
    labelFm["begin"] << lab;
    labelFm["PLAY"] << restartBtn;
    labelFm["QUIT"] << quitBtn;

    //Hide the widgets until game over is reached
    labelFm.get_place().field_display("PLAY", false);
    labelFm.get_place().field_display("QUIT", false);
    labelFm.get_place().field_display("begin", false);


    labelFm.collocate();
    startFm.collocate();
    startFm.show();

    /******Event Handling******/

    seqDisplayTimer.elapse([&] {
        //If display is at the first index of the sequence there is no previous light to turn off
        if (displayIdx > 0) {
            lightOff(*buttons.at(sequence.at(displayIdx - 1)), sequence.at(displayIdx - 1));
        }
        //As long as the index is within the range of the sequence vector there is a color to be represented
        if (displayIdx < sequence.size()) {
            //Turn on the light of a button in the button vector based on the color code at the current index
            lightOn(*buttons.at(sequence.at(displayIdx)), sequence.at(displayIdx));
            playSound(sequence.at(displayIdx));
            debugColor(sequence.at(displayIdx));
            displayIdx++;
        }
        else {
            cout << "end of sequence" << endl;
            seqDisplayTimer.stop();
            playerTurn = true;
            //Starts timer for user input
            inputTimer.start();
        }
        });

    inputTimer.elapse([&] {
        cout << "Failed to input within 5 seconds\n";

        //Remove player ability to activate button events and end the input timer
        playerTurn = false;
        playerWrong.play();
        inputTimer.stop();

        //Set the caption to display the value of the round
        lab.caption("<white>Sorry, you took over 5 seconds.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
        //Display the hidden widgets in the form
        labelFm.get_place().field_display("begin", true);
        labelFm.get_place().field_display("PLAY", true);
        labelFm.get_place().field_display("QUIT", true);
        labelFm.collocate();
        });

    startBtn.events().click([&] {
        //Hide the initial form and show the form containing end game information
        startFm.hide();
        labelFm.show();

        //Generate random number into the sequence and start the timer
        std::srand((unsigned int)time(NULL));
        sequence.push_back(rand() % 4);
        seqDisplayTimer.start();
        });

    quitBtn.events().click([&] {
        //End the program
        mainFm.close();
        });

    restartBtn.events().click([&] {
        //Reset the sequence properties and index trackers
        displayIdx = 0;
        inputIdx = 0;
        sequence.clear();

        //Hide the widgets
        labelFm.get_place().field_display("PLAY", false);
        labelFm.get_place().field_display("QUIT", false);
        labelFm.get_place().field_display("begin", false);

        //Add first random button to the sequence and start the display generation
        std::srand((unsigned int)time(NULL));
        sequence.push_back(rand() % 4);
        seqDisplayTimer.start();
        });

    yellowBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == YELLOW) {
                cout << "Correct Press\n";
                playerFa.play();
                if (inputIdx == sequence.size() - 1) {
                    cout << "Input sequence successfully\n";
                    //Reset the indexes
                    inputIdx = 0;
                    displayIdx = 0;
                    //Stop the timer since it's done
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
                cout << "Incorrect Press";
                inputTimer.stop();
                playerWrong.play();
                lab.caption("<white>Sorry, Wrong Color.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
                labelFm.get_place().field_display("begin", true);
                labelFm.get_place().field_display("PLAY", true);
                labelFm.get_place().field_display("QUIT", true);
                labelFm.collocate();
            }
        }
        });

    blueBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == BLUE) {
                cout << "Correct Press\n";
                playerDo.play();
                if (inputIdx == sequence.size() - 1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
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
                cout << "Incorrect Press";
                inputTimer.stop();
                playerWrong.play();
                lab.caption("<white>Sorry, Wrong Color.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
                labelFm.get_place().field_display("begin", true);
                labelFm.get_place().field_display("PLAY", true);
                labelFm.get_place().field_display("QUIT", true);
                labelFm.collocate();
            }
        }
        });

    greenBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == GREEN) {
                cout << "Correct Press\n";
                playerRe.play();
                if (inputIdx == sequence.size() - 1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
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
                cout << "Incorrect Press";
                inputTimer.stop();
                playerWrong.play();
                lab.caption("<white>Sorry, Wrong Color.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
                labelFm.get_place().field_display("begin", true);
                labelFm.get_place().field_display("PLAY", true);
                labelFm.get_place().field_display("QUIT", true);
                labelFm.collocate();
            }
        }
        });

    redBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == RED) {
                cout << "Correct Press\n";
                playerMi.play();
                if (inputIdx == sequence.size() - 1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
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
                cout << "Incorrect Press";
                inputTimer.stop();
                playerWrong.play();
                lab.caption("<white>Sorry, Wrong Color.\nYou made it to round " + to_string(sequence.size() - 1) + ".</>");
                labelFm.get_place().field_display("begin", true);
                labelFm.get_place().field_display("PLAY", true);
                labelFm.get_place().field_display("QUIT", true);
                labelFm.collocate();
            }
        }
        });

    //Execute Nana
    nana::exec();
}

void playSound(int colorCode) {
    nana::audio::player playerDo("Do_Piano.wav");
    nana::audio::player playerRe("Re_Piano.wav");
    nana::audio::player playerMi("Mi_Piano.wav");
    nana::audio::player playerFa("Fa_Piano.wav");
    nana::audio::player playerWrong("Wrong.wav");
    nana::threads::pool pool(1);

    switch (colorCode) {
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

void lightOn(button& btn, int colorCode) {
    switch (colorCode) {
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

void lightOff(button& btn, int colorCode) {
    switch (colorCode) {
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

void debugColor(int colorCode) {
    switch (colorCode) {
    case BLUE: cout << "blue - ";
        break;
    case RED:
        cout << "red - ";
        break;
    case YELLOW:
        cout << "yellow - ";
        break;
    case GREEN:
        cout << "green - ";
        break;
    default:
        break;
    }
}