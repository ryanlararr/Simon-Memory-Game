#include "simon-game.h"

using namespace std;
using namespace nana;

int main() {
    //Randomizing rand
    srand((unsigned int)time(NULL));

    //Definining audio files
    nana::audio::player playerDo("Do_Piano.wav");
    nana::audio::player playerRe("Re_Piano.wav");
    nana::audio::player playerMi("Mi_Piano.wav");
    nana::audio::player playerFa("Fa_Piano.wav");
    nana::audio::player playerWrong("Wrong.wav");

    //Define timer for user input
    //User input must match the sequence vector
    timer inputTimer(chrono::milliseconds(5000));
    inputTimer.elapse([&] {
        //If gotten to this point then the user has failed to input in the appropriate time frame.
        cout << "Failed to input within 5 seconds";
        playerTurn = false;
        playerWrong.play();
        inputTimer.stop();
        });

    int displayIdx = 0;
    int inputIdx = 0;

    timer seqDisplayTimer(chrono::milliseconds(500));
    seqDisplayTimer.elapse([&] {
        if (displayIdx > 0) {
            lightOff(*buttons.at(sequence.at(displayIdx - 1.0)), sequence.at(displayIdx - 1.0));
        }
        if (displayIdx < sequence.size()) {
            lightOn(*buttons.at(sequence.at(displayIdx)), sequence.at(displayIdx));
            playSound(sequence.at(displayIdx));
            switch (sequence.at(displayIdx)) {
            case BLUE:
                cout << "blue, ";
                break;
            case RED:
                cout << "red, ";
                break;
            case YELLOW:
                cout << "yellow, ";
                break;
            case GREEN:
                cout << "green, ";
                break;
            default:
                break;
            }
            displayIdx++;
        }
        else {
            cout << "End sequence" << endl;
            seqDisplayTimer.stop();
            playerTurn = true;
            //Starts timer for user input
            inputTimer.start();
        }
        });

    //Defining the base form
    const rectangle& rect = API::make_center(300, 200);
    //Removing the sizable property of the form
    form mainFm(rect, appearance(1, 0, 1, 1, 1, 1, 0));
    mainFm.caption("Simon - Ryan Arreola");
    API::window_icon(mainFm.handle(), paint::image("favicon.ico"));


    //Buttons will check whether it is at the end of the sequence and if the input itself matched the sequence.
    //If the button press did not match the sequence then the error sound plays

    
    //Defining buttons
    button redBtn{ mainFm }; redBtn.bgcolor(color(255, 0, 0));
    redBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == RED) {
                playerMi.play();
                if (inputIdx == sequence.size()-1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
                    inputTimer.stop();
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
            }
        }
        });
    button greenBtn{ mainFm }; greenBtn.bgcolor(color(0, 100, 0));
    greenBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == GREEN) {
                cout << "Correct Press";
                playerRe.play();
                if (inputIdx == sequence.size()-1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
                    inputTimer.stop();
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
            }
        }
        });
    button blueBtn{ mainFm }; blueBtn.bgcolor(color(0, 0, 255));
    blueBtn.events().click([&] {        
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == BLUE) {
                cout << "Correct Press\n";
                playerDo.play();
                if (inputIdx == sequence.size()-1) {
                    cout << "Input sequence successfully\n";
                    inputIdx = 0;
                    displayIdx = 0;
                    inputTimer.stop();
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
            }
        }
        });
    button yellowBtn{ mainFm }; yellowBtn.bgcolor(color(200, 200, 0));
    yellowBtn.events().click([&] {
        if (playerTurn) {
            inputTimer.stop();
            inputTimer.start();
            if (sequence.at(inputIdx) == YELLOW) {
                cout << "Correct Press\n";
                playerFa.play();
                if (inputIdx == sequence.size()-1) {
                    cout << "Input sequence successfully\n";
                    //Reset the indexes
                    inputIdx = 0;
                    displayIdx = 0;
                    //Stop the timer since it's done
                    inputTimer.stop();
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
            }
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
        sequence.push_back(rand() % 4);
        seqDisplayTimer.start();
        });

    labelFm.collocate();
    startFm.collocate();
    startFm.show();

    exec();
}
void playSound(int colorCode) {
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

