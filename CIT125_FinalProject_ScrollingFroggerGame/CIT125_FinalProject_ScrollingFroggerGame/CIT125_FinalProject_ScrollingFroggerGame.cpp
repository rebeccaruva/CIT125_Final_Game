// CIT125_FinalProject_ScrollingFroggerGame.cpp : load in level files
// 5 lines at a time, move character with left and right arrows to reach sun,
// level auto scrolls at a speed of 1 (aka moves every 1 second) 
// Created by Rebecca (Bex) Ruvalcaba - last edited on 05/23/2021

#include <iostream> //include the input/output stream
#include <fstream> //include the file stream
#include <string> //include the string variable type
#include <time.h> //include time information, used for scrolling the level every second
#include <conio.h> //include conio for console input/output, i.e keyboard presses with _getch
#include <thread> //unclude thread so code can be executed concurrently in diff threads
using namespace std; //using the standard namespace

/*
Level class :
   - used to encapsulate level-related attributes including:
   currentDisplay (lines currently displayed on console, status (for threads),
   originalLine (bottom line of currentDisplay without player), and gameEnded (status
   of the level/game)
   - used to get/set these attributes from main() function and other program-defined functions
   I have created
   - note that a string* pointer to the currentDisplay array is returned with getDisplay
   since you cannot return an entire array in c++
*/
class Level {
private:
    // Private attributes
    string currentDisplay[5] = {""}; //current 5 line display for the game's current level
    bool status = true; //true for all threads running, false for all threads stopped
    string originalLine = currentDisplay[0]; //bottom line without player
    bool gameEnded = false; //check the status of game, ended or not

public:
    // set current display
    void setDisplay(string display[]) {
        for (int i = 0; i < 5; i++) {
            currentDisplay[i] = display[i];
        }
        //update original player line as well
        originalLine = currentDisplay[0];
    }
    // get current display
    string* getDisplay() {
        //return pointer to array
        return currentDisplay;
    }

    // set new line with player
    void setNewLine(string line) {
        //set bottom line of currentDisplay (index 0) to line
        //so player will be displayed
        currentDisplay[0] = line;
    }
    // get original line
    string getOgLine() {
        return originalLine;
    }

    // set status
    void setStatus(bool stat) {
        status = stat;
    }
    // get status
    bool getStatus() {
        return status;
    }

    // set game status
    void setGameEnded(bool gameStat) {
        gameEnded = gameStat;
    }
    // get game status
    bool getGameEnded() {
        return gameEnded;
    }
};

//function prototypes
void movePlayer();
void displayPlayer(int dir);
void scrollLevel(int totalLines, string currentLevel[]);
void updateLevelView(int totalLines, string currentLevel[], int currentLine);
void displayLevel();
void menuDisplay(int level);

//global object myLvl to access get and set for level attributes
Level myLvl;

//global vars
int playerPosition; //position of the player
char symbol; //'f' symbol for player (flower)
char wall; //'#' symbol for wall
char goal; //'o' symbol for goal (sun)
bool levelSuccess; //used to check whether level was won or not


int main()
{
    //first declare and initialize variables
    //vars for game start
    string title = "Hungy Flowey"; //aka hungry flower
    string description = "a flower is hungry so you must help it reach the sun";
    string goals[2] = { "move left and right to avoid walls", "reach the sun to move on to the next level" };
    playerPosition = 9; //starting player position -> 9 in range of 0-19, inclusive
    symbol = 'f'; //player symbol is f
    wall = '#'; //wall is pound
    goal = 'o'; //goal is the sun

    //vars for levels
    const int TOTAL_LINES = 50; //num of total lines in a level
    ifstream levelFile; //current level file
    ofstream levelFileToDisplay; //current level file reversed (so can be displayed easier)
    string currentLevelLines[TOTAL_LINES] = { "" }; //string array for every line in the current level's file
    int lineCount = 0; //count for populating currentLevelLines array
    bool canDisplayLevel = false; //can display level bool
    int totalLevels[3] = { 1, 2, 3 }; //three levels total in game
    int level = totalLevels[0]; //start at first level
    levelSuccess = false; //level is not successful until reach goal
    
    //while game has not ended, continue to next level (or to level that has been reset)
    while (!myLvl.getGameEnded()) {
        //open or create the correct files for the correct levels (1, 2, 3)
        switch (level) {
            case 1:
                //first level, read in level1 and create level1ToDisplay
                //read in level file
                levelFile.open("Levels/level1.txt", ios::in);
                //open (new) level file to be displayed
                levelFileToDisplay.open("Levels/level1ToDisplay.txt", ios::out);
                break;
            case 2:
                //second level, read in level2 and create level2ToDisplay
                //read in level file
                levelFile.open("Levels/level2.txt", ios::in);
                //open (new) level file to be displayed
                levelFileToDisplay.open("Levels/level2ToDisplay.txt", ios::out);
                break;
            case 3:
                //third/final level, read in level3 and create level3ToDisplay
                //read in level file
                levelFile.open("Levels/level3.txt", ios::in);
                //open (new) level file to be displayed
                levelFileToDisplay.open("Levels/level3ToDisplay.txt", ios::out);
                break;
        }

        //if levelFile is open, then read the file into an array
        //else display error
        if (levelFile.is_open()) {
            //while not at the end of file,
            //populate the currentLevelLines array by going through each line in file
            while (!levelFile.eof()) {
                string line = ""; //to hold current line of characters in the level file
                getline(levelFile, line); //place current line from levelFile into line string

                //populate currentLevelLines array
                currentLevelLines[lineCount] = line;

                lineCount++; //increment line count
            } //end while

            //if levelFileToDisplay is open, then populate with level file reversed
            if (levelFileToDisplay.is_open()) {
                //populate with level file reversed
                int eofNum = TOTAL_LINES - 1; //get last line's index

                //go through each element of currentLevelLines array backwards to populate levelFileToDisplay
                for (int i = eofNum; i >= 0; i--) {
                    levelFileToDisplay << currentLevelLines[i] << endl;
                } //end for

                //okay to display the level now that levelFile and levelFileToDisplay
                //have been opened and processed
                canDisplayLevel = true;

                levelFileToDisplay.close(); //close levelFileToDisplay
            }
            else {
                cout << "Error: Could not create level file to display." << endl; //error
            } //end if

            //declare & init temp display array to hold initial level display
            string display[5] = { "" };
            int eofNum = TOTAL_LINES - 1; //get last line's index since will be displayed reversed

            for (int i = 0; i < 5; i++) {
                //save first 5 lines of the game to be initially displayed
                display[i] = currentLevelLines[eofNum - i]; //display reverse -> (eofNum - i)
            } //end for
            myLvl.setDisplay(display); //setDisplay function to initial level display

            levelFile.close(); //close levelFile
        }
        else {
            cout << "Error. Could not open the level file." << endl; //error
        } //end if

        //if can display level, aka went through processing for levelFile AND levelFileToDisplay
        if (canDisplayLevel) {
            //display initial view of the level

            //string* is a pointer to the currentDisplay array,
            //since you cannot return arrays via return functions in c++
            //but you can point to the array with string (array type) and *
            string* display = myLvl.getDisplay(); //get pointer to beginning of currentDisplay array

            //display the lines in reverse
            for (int i = 4; i >= 0; i--) {
                //*(display + i) == display[i] since we are getting the info
                //from a pointer as opposed an array ... pointing to (display + i)
                //means we are pointing to the next location in the array
                cout << *(display + i) << endl;
            } //end for

            //vars for threads
            myLvl.setStatus(true); //setting status to true since all threads will begin running
            thread th1(scrollLevel, TOTAL_LINES, currentLevelLines); //thread 1 is moveLevel function
            thread th2(movePlayer); //thread 2 is movePlayer function

            //wait for first thread to finish
            th1.join();
            //wait for second thread to finish
            th2.join();

            //prompt user to either retry level, continue to next level, or end game
            menuDisplay(level);
        } //end if
    } //end while

    //while level is not over, run current level
    //else either prompt user to : restart or exit -or- go to next level or exit
    //depending if th euser passed the level or not
    return 0;
}

//**function definitions**


/*
movePlayer function :
   - allows the user to click left and right arrows to move
   the player on the screen.
   - If Escape button is clicked all threads will terminate.
   - movePlayer calls on currentPlayerPos and displayPlayer 
   functions.
*/
void movePlayer() {
    //keys for moving player left and right
    const int MOVE_LEFT = 75;
    const int MOVE_RIGHT = 77;
    const int END = 27;

    int dir = 0; //direction the player is moving

    int c = 0;
    int checkKeys = true;
    while (checkKeys) {
        c = 0;

        switch (c = _getch()) {
            case MOVE_LEFT:
                //move player left :
                //if player position is greater than 0, player can move left
                //pos = currentPlayerPos(playerPos, symbol); //get current player position
                if (playerPosition > 0) {
                    dir = -1; //direction is to the left
                    displayPlayer(dir);
                }
                else {
                    dir = 0; //direction cannot move
                    displayPlayer(dir);
                }
                break;
            case MOVE_RIGHT:
                //move player right :
                //if player position is less than line length-1 aka 19, player can move right
                //pos = currentPlayerPos(playerPos, symbol); //get current player position
                if (playerPosition < 19) {
                    dir = 1; //direction is to the right
                    displayPlayer(dir);
                }
                else {
                    dir = 0; //direction cannot move
                    displayPlayer(dir);
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


/*
displayPlayer function :
   - displays player on the bottom line of the level
   - checks whether the player is hitting a wall, if so the game will
   go to the menu screen
   - checks whether the player is hitting the goal, if so the successful
   level will go to the menu screen with continue option (if more levels to play)
   - if a blank space the player will be displayed as normal
*/
void displayPlayer(int dir) {
    //display player on the bottom line of the current level display

    string playerLine = myLvl.getOgLine();; //get bottom line of display (player to be added)
    string ogLine = myLvl.getOgLine(); //get the bottom line of the display without player

    string s(1, symbol); //cast symbol (size 1) from char to string using string constructor

    //check if player is hitting wall
    if (ogLine[playerPosition] == wall) {
        //hitting wall, end game negatively
        levelSuccess = false; //level was unsuccessful
        myLvl.setStatus(false); //end all threads
        return; //break out of function
    } //end if

    //check if player is hitting goal
    if (ogLine[playerPosition] == goal) {
        //hit goal, end game positively
        levelSuccess = true; //level was successful
        myLvl.setStatus(false);
        return; //break out of function
    } //end if

    //place player on screen if blank space! (not wall or goal)
    if (dir == -1) {
        //move left
        //update player position
        playerPosition -= 1; //position moved to the left
        
        // update the bottom line displayed to include the player
        playerLine.replace(playerPosition, 1, s); //s is symbol as a string
        myLvl.setNewLine(playerLine);
    }
    else if (dir == 1) {
        //move right
        //update player position
        playerPosition += 1;

        // update the bottom line displayed to include the player
        playerLine.replace(playerPosition, 1, s); //s is symbol as a string
        myLvl.setNewLine(playerLine);
    }
    else {
        //dir is 0 and direction cannot move
        //update the bottom line to include the player position as is
        playerLine.replace(playerPosition, 1, s); //s is symbol as a string
        myLvl.setNewLine(playerLine);
    }

    //redisplay the level with new bottom line (includes the player)
    displayLevel();
}


/*
scrollLevel function :
   - uses the time type to scroll the level at the level speed
   (each second it will move along)
   - once the level reaches the last line it will terminate the 
   thread and prompt the user to click an arrow key so the first
   thread can also be terminated
   - scrollLevel calls on updateLevelView to "scroll" through the
   level
*/
void scrollLevel(int totalLines, string currentLevelLines[]) {
    int levelSpeed = 1; //speed of level (aka 1 second)
    time_t start; //time type called start
    start = time(0); //time starts at current time

    int currentLine = 0; //count of currentLine (0-50)
    bool keepMoving = true; //keep scrolling as long as true

    //while the level should be moved, scroll at levelSpeed
    //else end function
    while (keepMoving) {
        //if current time minus start is equal to levelSpeed (1 second)
        //update the level view, increment the current line, and
        //update starting time to reset second count
        if (time(0) - start == levelSpeed) {
            //move level by a line
            updateLevelView(totalLines, currentLevelLines, currentLine);
            currentLine++; //update the currentLine count

            //add current levelSpeed to starting time
            //so second count can restart
            start = start + levelSpeed;
        } //end if

        //check if still need to move along the level
        //if not (aka if currentLine is greater than or equal to the
        //last index of lines in the level file), end this thread
        if (currentLine >= totalLines-1)
            myLvl.setStatus(false);
        //end if

        //if status is false, thread needs to be terminated
        if (!myLvl.getStatus()) {
            //user needs to press right arrow to continue and
            //make sure first thread (keyboard press check) gets the memo
            //that it should terminate as well
            cout << endl << "Please press right arrow key to continue." << endl << endl;
            keepMoving = false; //end while loop and function
        } //end if
    } //end while
} //end of moveLevel function

void updateLevelView(int totalLines, string currentLevelLines[], int currentLine) {
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
            updateD[i] = currentLevelLines[(totalLines - 1) - lineNum];
        } //end if
    } //end for

    //update the current display
    myLvl.setDisplay(updateD);
    //make sure player is placed on screen where previously was
    displayPlayer(0);

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

void menuDisplay(int level) {
    //display the menu of options to user
    char choice = ' ';
    bool validChoice = false;

    //display prompt to user
    cout << "Please enter a choice:" << endl << endl;

    //while choice is not valid, display menu to user
    while (!validChoice) {
        cout << "Level Menu" << endl;
        cout << "----------" << endl << endl;
        cout << "R to retry" << endl;
        cout << "X to end game" << endl;
        //if level was successful (won) then prompt for C to continue
        if (levelSuccess)
            cout << "C to continue" << endl;
        //end if
        cout << endl;
        cin >> choice;
        choice = toupper(choice); //make sure choice is capitalized

        //check which choice was made
        if (choice == 'C' && levelSuccess) {
            //continue to next level, aka increment level
            //but first check if have a level to go to
            if (level >= 2) //2 is final level's index
                myLvl.setGameEnded(true); //end the game since no more levels to play
            else
                level++; //increment level to play the next level
            //end if
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else if (choice == 'X') {
            //end the game
            myLvl.setGameEnded(true);
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else if (choice == 'R') {
            //retry the level, aka do not change level
            //but make sure gameEnded is false
            myLvl.setGameEnded(false);
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else {
            //invalid character entered, display error to user
            cout << endl << "Invalid choice entered, please enter another choice." << endl << endl;
        }
    } //end while
}
