// CIT125_FinalProject_ScrollingFroggerGame.cpp : load in level trial file
// 6 lines at a time, move character with left and right arrows,
// reach sun to enter next level
// Created by Rebecca (Bex) Ruvalcaba on 05/20/2021

#include <iostream> //include the input/output stream
#include <fstream> //include the file stream
#include <string> //include the string variable type
#include <time.h> //include time information
#include <conio.h> //include conio for console input/output, i.e keyboard presses with _getch
#include <thread> //unclude thread so code can be executed concurrently
using namespace std; //using the standard namespace

class Level {
private:
    // Private attributes
    string currentDisplay[5] = {""};
    bool status = true;

public:
    // set current display
    void setDisplay(string display[]) {
        for (int i = 0; i < 5; i++) {
            currentDisplay[i] = display[i];
        }
    }
    // get current display
    string* getDisplay() {
        //return pointer to array
        return currentDisplay;
    }

    // set status
    void setStatus(bool stat) {
        status = stat;
    }
    // get status
    bool getStatus() {
        return status;
    }
};

//function prototypes
void resetVars();
void movePlayer(string playerPos, char symbol, char wall, char goal);
int currentPlayerPos(string player, char symbol);
void moveLevel(int totalLines, string currentLevel[]);
void updateLevelView(int totalLines, string currentLevel[], int currentLine);
void displayLevel();

//global object myLvl
Level myLvl;

int main()
{
    //first declare and initialize variables
    //vars for game start
    string title = "Hungy Flowey"; //aka hungry flower
    string description = "a flower is hungry so you must help it reach the sun";
    string goals[2] = { "move left and right to avoid walls", "reach the sun to move on to the next level" };
    string playerPos = "         f          "; //player is a flower
    char symbol = 'f'; //player symbol is f
    char wall = '#'; //wall is pound
    char goal = 'o'; //goal is the sun

    //vars for level
    const int TOTAL_LINES = 50; //num of lines in a level
    ifstream levelFile; //current level file
    ofstream levelFileToDisplay;
    string currentLevel[TOTAL_LINES] = { "" }; //string array for each line in the current level's file
    int lineCount = 0; //count for populating currentLevel array
    bool canDisplayLevel = false;
    
    //read in level file
    levelFile.open("Levels/levelTrial.txt", ios::in);
    //open new level file to display
    levelFileToDisplay.open("Levels/levelTrialToDisplay.txt", ios::out);

    //if file is open, then read the file into an array
    //else display error
    if (levelFile.is_open()) {
        //while not at the end of file,
        //populate the currentLevel array
        while (!levelFile.eof()) {
            string line = ""; //current line of characters in the level file
            getline(levelFile, line);
            
            //populate currentLevel array
            currentLevel[lineCount] = line;

            lineCount++; //increment line count
        } //end while

        //if levelFileToDisplay is open, then populate with level file reverse
        if (levelFileToDisplay.is_open()) {
            //populate with level file reversed
            int eofNum = TOTAL_LINES - 1; //get last line's index

            for (int i = eofNum; i >= 0; i--) {
                levelFileToDisplay << currentLevel[i] << endl;
            } //end for

            canDisplayLevel = true; //okay to display the level

            levelFileToDisplay.close();
        }
        else {
            cout << "Could not create level file to display." << endl;
        } //end if

        string display[5] = { "" };
        for (int i = 0; i < 5; i++) {
            //save first 5 lines of the game to be displayed first
            //*(display + 1) is the same as display[i] --> used since getDisplay
            //returns a pointer to the start of the array
            display[i] = currentLevel[(TOTAL_LINES - 1) - i];
        } //end for
        myLvl.setDisplay(display);

        /*cout << "Level file is ready to go." << endl;
        cout << "There are " << TOTAL_LINES << " lines in this file." << endl << endl;*/
        levelFile.close();
    }
    else {
        cout << "There was an error opening the level file." << endl;
    } //end if
    

    if (canDisplayLevel) {
        //okay to continue displaying the level

        //initial view of the level
        string* display = myLvl.getDisplay();
        for (int i = 4; i >= 0; i--) {
            cout << *(display + i) << endl;
        } //end for

        //vars for threads
        myLvl.setStatus(true); //true for all threads running, false for all threads stopped
        thread th1(moveLevel, TOTAL_LINES, currentLevel);
        thread th2(movePlayer, playerPos, symbol, wall, goal);


        //wait for first thread to finish
        th1.join();
        //wait for second thread to finish
        th2.join();
    } //end if

    //while level is not over, run current level
    //else either prompt user to : restart or exit -or- go to next level or exit
    //depending if th euser passed the level or not
    return 0;
}

//**function definitions**
void resetVars() {

} //end of resetVars function

void movePlayer(string playerPos, char symbol, char wall, char goal) {
    //keys for moving player left and right
    const int MOVE_LEFT = 75;
    const int MOVE_RIGHT = 77;
    const int END = 27;

    string s(1, symbol); //cast symbol (size 1) from char to string using string constructor
    string w(1, wall);
    string g(1, goal);
    int pos = 0; //to be used as the player's current index

    int c = 0;
    int checkKeys = true;
    while (checkKeys) {
        c = 0;

        switch (c = _getch()) {
            case MOVE_LEFT:
                //move player left :
                //if player position is greater than 0, player can move left
                pos = currentPlayerPos(playerPos, symbol); //get current player position
                if (pos > 0) {
                    //replace current player with a space
                    //and replace the space to the left with the player
                    playerPos.replace(pos, 1, " ");
                    playerPos.replace(pos - 1, 1, s); //s is symbol as a string
                    cout << playerPos << endl;
                }
                break;
            case MOVE_RIGHT:
                //move player right :
                //if player position is less than length-1, player can move right
                pos = currentPlayerPos(playerPos, symbol); //get current player position
                if (pos < playerPos.length()-1) {
                    //replace current player with a space
                    //and replace the space to the right with the player
                    playerPos.replace(pos, 1, " ");
                    playerPos.replace(pos + 1, 1, s); //s is symbol as a string
                    cout << playerPos << endl;
                }
                break;
            case END:
                //update thread status
                myLvl.setStatus(false);
                break;
        } //end switch

        //if status is false, terminate thread
        if (!myLvl.getStatus()) {
            checkKeys = false;
        } //end if
    } //end while
} //end of movePlayer function

int currentPlayerPos(string player, char symbol) {
    //search for player symbol, when it is found return it's index
    //if not found just return 0
    for (int i = 0; i < player.length(); i++) {
        if (player[i] == symbol) {
            return i;
        } //end if
    } //end for
    return 0;
} //end of currentPlayerPos function

void moveLevel(int totalLines, string currentLevel[]) {
    int levelSpeed = 1; //speed of level
    time_t start;
    start = time(0); //time starts at 0

    int currentLine = 0; //count of currentLine (0-50)
    bool keepMoving = true;

    while (keepMoving) {
        if (time(0) - start == levelSpeed) {
            //move level by a line
            updateLevelView(totalLines, currentLevel, currentLine);
            currentLine++; //update the currentLine count

            //add current levelSpeed to starting time
            start = start + levelSpeed;
        } //end if

        //check if still need to move along the level
        //if not, end this thread
        if (currentLine >= totalLines-1)
            myLvl.setStatus(false);
        //end if

        //if status is false, terminate thread
        if (!myLvl.getStatus())
            keepMoving = false;
        //end if
    } //end while
} //end of moveLevel function

void updateLevelView(int totalLines, string currentLevel[], int currentLine) {
    //update current level view

    //first move level view down 1 by moving all elements back 1 index
    //and adding the next line to the final element
    string tempDisplay[5] = { "" }; //temp display to hold previous display
    string updateD[5] = { "" }; //updated display to be set
    
    //fill temp display with current display elements
    string* display = myLvl.getDisplay();
    for (int i = 0; i < 5; i++) {
        tempDisplay[i] = *(display + i);
    } //end for

    //move the elements back 1 index
    for (int i = 0; i < 5; i++) {
        //if not at final element, move the elements back 1
        //else add the new line to the last element in the array
        if (i != 4) {
            updateD[i] = tempDisplay[i + 1];
        }
        else {
            int lineNum = currentLine + 5; //line num to add
            updateD[i] = currentLevel[(totalLines - 1) - lineNum];
        } //end if
    } //end for
    
    //update the current display
    myLvl.setDisplay(updateD);

    displayLevel();
} //end of updateLevelView function

void displayLevel() {
    //display the current level view

    //clear screen
    system("CLS");

    //display level view
    string* display = myLvl.getDisplay();
    for (int i = 4; i >= 0; i--)
        cout << *(display + i) << endl;
    //end for
} //end of displayLevel function
