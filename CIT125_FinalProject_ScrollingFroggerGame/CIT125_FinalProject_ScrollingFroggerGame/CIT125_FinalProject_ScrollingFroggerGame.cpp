﻿// CIT125_FinalProject_ScrollingFroggerGame.cpp :

/*
a vertical scrolling frogger-like game called Hungy Flowey!
---
a flower is hungry so it must pass wall obstacles to reach the sun
and feed itself.

f is the player, a flower
o is the goal, the sun
# are wall obstacles that must be avoided

the player can move the flower with left and right arrow keys

there are 3 levels total, each with 50 lines total and each line
is 20 characters long

player score increases by 40 every level you successfully finish

each level will auto scroll at a speed of 1 (aka moves every 1 second)

there is a start screen, menu screen for in between levels/went hit obstacle,
and an end screen after successfully finishing all 3 levels

happy playing / reading!
*/

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
   currentDisplay (5 lines currently displayed in console), status (for threads),
   originalLine (bottom line of currentDisplay without player), and gameEnded (status
   of the level/game)
   - this class is used to get/set these attributes from main() function and
   other program-defined functions I have created
** note that a string* pointer to the currentDisplay array is returned with getDisplay
   since you cannot return an entire array in c++ apparently :'(
*/
class Level {
private:
    // private attributes
    string currentDisplay[5] = {""}; //current 5 line display for the game's current level
    string originalLine = currentDisplay[0]; //bottom line without player

    bool status = true; //true for all threads running, false for all threads stopped

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
void resetVars(int &lineCount, bool &canDisplayLevel); //pass as reference
void displayStartScreen();
void movePlayer();
void displayPlayer(int dir);
void scrollLevel(int totalLines, string currentLevel[]);
void updateLevelView(int totalLines, string currentLevel[], int currentLine);
void displayLevel();
void displayMenu();
void displayEndScreen();

//global object myLvl to access get and set for level attributes
Level myLvl;

//global vars
string title; //title of game
int playerPosition; //position of the player
char symbol; //'f' symbol for player (flower)
char wall; //'#' symbol for wall
char goal; //'o' symbol for goal (sun)
int level; //current level the player is on
bool levelSuccess; //used to check whether level was won or not
int score; //player's score : either 0, 40, 80, or 120 (highest) depending on how many levels completed
int scoreMult; //the player score multiplier (40)
bool endOfLevel = false; //check whether at end of level or not


int main()
{
    //first declare and initialize variables
    //vars for game start
    title = "Hungy Flowey"; //aka hungry flower
    playerPosition = 9; //starting player position -> 9 in range of 0-19, inclusive
    symbol = 'f'; //player symbol is f
    wall = '#'; //wall is pound
    goal = 'o'; //goal is the sun

    //vars for levels
    const int TOTAL_LINES = 50; //num of total lines in a level
    ifstream levelFile; //current level file - to read levels input 
    string currentLevelLines[TOTAL_LINES] = { "" }; //string array for every line in the current level's file
    int lineCount = 0; //count for populating currentLevelLines array
    bool canDisplayLevel = false; //can display level bool
    int totalLevels[3] = { 1, 2, 3 }; //three levels total in game
    level = totalLevels[0]; //start at first level
    levelSuccess = false; //level is not successful until reach goal (sun)
    score = 0; //start score at 0
    scoreMult = 40; //the score increases by 40 each level
    
    //for first play through display start screen
    displayStartScreen();

    //while game has not ended, continue to next level (or to level that has been reset)
    while (!myLvl.getGameEnded()) {
        //first reset vars
        resetVars(lineCount, canDisplayLevel);

        //reset currentLevelLines (array to hold each line in the current level's file)
        for (int i = 0; i < TOTAL_LINES; i++)
            currentLevelLines[i] == "";
        //end for

        //open the correct files for the correct levels (1, 2, 3)
        switch (level) {
            case 1:
                //first level, read in level1
                levelFile.open("Levels/level1.txt", ios::in);
                break;
            case 2:
                //second level, read in level2
                levelFile.open("Levels/level2.txt", ios::in);
                break;
            case 3:
                //third/final level, read in level3
                levelFile.open("Levels/level3.txt", ios::in);
                break;
        } //end switch

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

            //declare & init temp display array to hold initial level display
            string display[5] = { "" };
            int eofNum = TOTAL_LINES - 1; //get last line's index since will be displayed reversed

            for (int i = 0; i < 5; i++) {
                //save first 5 lines of the game to be initially displayed
                display[i] = currentLevelLines[eofNum - i]; //display reverse -> (eofNum - i)
            } //end for
            myLvl.setDisplay(display); //setDisplay function to initial level display

            //okay to display the level now that levelFile
            //has been opened and processed
            canDisplayLevel = true;

            levelFile.close(); //close levelFile
        }
        else {
            cout << "Error. Could not open the level file." << endl; //error
        } //end if

        //if can display level, aka went through processing for levelFile
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
            thread th1(scrollLevel, TOTAL_LINES, currentLevelLines); //thread 1 is scrollLevel function
            thread th2(movePlayer); //thread 2 is movePlayer function

            //wait for first thread to finish
            th1.join();
            //wait for second thread to finish
            th2.join();

            //prompt user to either retry level, continue to next level, or end game
            displayMenu();
        } //end if
    } //end while
    
    //when out of while loop, end the game (aka exit main function)

    return 0;
} //end of main function


//***function definitions***

/*
displayStartScreen function :
   - displays start screen to user and prompts them to enter any character to begin
*/
void displayStartScreen() {
    //declare and init start screen vars
    string description = "a flower is hungry so you must help it reach the sun";
    string goals[2] = { "move left and right with arrow keys to avoid walls", "reach the sun to move on to the next level" };
    char c = ' ';

    //display start screen to user
    cout << "------------" << endl;
    cout << title << endl;
    cout << "------------" << endl << endl;
    cout << description << endl << endl << endl;
    cout << "How to play:" << endl;
    cout << "-- " << goals[0] << endl;
    cout << "-- " << goals[1] << endl << endl;

    //prompt the user to start game
    cout << "Enter any character to begin game: ";
    cin >> c; //once something is entered, exit void function
} //end of displayStartScreen function

/*
resetVars function :
   - resets necessary variables to original state (for levels to begin)
*/
void resetVars(int &lineCount, bool &canDisplayLevel) {
    //resets variables so levels can begin
    playerPosition = 9; //reset starting player position

    lineCount = 0; //reset count for populating currentLevelLines array

    canDisplayLevel = false; //reset : cannot display level yet
    levelSuccess = false; //reset : level is not successful until reach goal
    endOfLevel = false; //reset - not at end of level

    score = 0; //reset player score --> score is calculated at each menu/end screen
    //depending on how many levels have been successfully completed and the score multiplyer (40)

    string resetDisplay[5] = { "" }; //empty string array with 5 elements
    myLvl.setDisplay(resetDisplay); //reset currentDisplay array to empty string array
    myLvl.setStatus(true); //reset status
} //end of resetVars function

/*
movePlayer function :
   - allows the user to click left and right arrows to move
   the player on the screen.
   - movePlayer calls on displayPlayer function
*/
void movePlayer() {
    //keys for moving player left and right
    const int MOVE_LEFT = 75; //left arrow key
    const int MOVE_RIGHT = 77; //right arrow key

    int dir = 0; //direction the player is moving

    int c = 0; //used to find which key was pressed
    int checkKeys = true;

    //while keys still need to be checked (aka not terminating the thread yet)
    //check if left or right arrow were pressed
    while (checkKeys) {
        c = 0; //reset key to 0

        //getch() will read a character input from keyboard
        //gives us back the keycode (hence why c is an integer)
        //check whether left or right key were pressed
        switch (c = _getch()) {
            case MOVE_LEFT:
                //move player left :
                //if player position is greater than 0, player can move left
                if (playerPosition > 0) {
                    dir = -1; //direction is to the left
                    displayPlayer(dir);
                }
                else {
                    dir = 0; //direction cannot move
                    displayPlayer(dir); //display character as previous position
                }
                break;
            case MOVE_RIGHT:
                //move player right :
                //if player position is less than line length-1 aka 19, player can move right
                if (playerPosition < 19) {
                    dir = 1; //direction is to the right
                    displayPlayer(dir);
                }
                else {
                    dir = 0; //direction cannot move
                    displayPlayer(dir); //display character as previous position
                }
                break;
        } //end switch

        //if status is false, terminate thread
        if (!myLvl.getStatus()) {
            checkKeys = false; //check keys will stop while loop and ultimately exit function
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
   - else the player will display and game will continue as normal
*/
void displayPlayer(int dir) {
    //display player on the bottom line of the current level display

    string playerLine = myLvl.getOgLine();; //get bottom line of display (player to be added)
    string ogLine = myLvl.getOgLine(); //get the bottom line of the display without player

    string s(1, symbol); //cast symbol (size 1) from char to string using string constructor

    //place player on screen
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
        playerPosition += 1;//position moved to the right

        // update the bottom line displayed to include the player
        playerLine.replace(playerPosition, 1, s); //s is symbol as a string
        myLvl.setNewLine(playerLine);
    }
    else {
        //dir is 0 and direction cannot move
        //update the bottom line to include the player position as is
        playerLine.replace(playerPosition, 1, s); //s is symbol as a string
        myLvl.setNewLine(playerLine);
    } //end if

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
        myLvl.setStatus(false); //end all threads
        return; //break out of function
    }
    else {
        levelSuccess = false; //level was unsuccessful
        if (endOfLevel) {
            //didn't win level so end it
            myLvl.setStatus(false); //end all threads
            return; //break out of function
        }
    }//end if

    //redisplay the level with new bottom line (includes the player)
    //don't display until after check for wall or goal
    displayLevel();
} //end of displayPlayer function


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

    int currentLine = 0; //count of currentLine (0-49)
    bool keepMoving = true; //keep scrolling as long as true

    //while the level should be moved, scroll at levelSpeed
    //else end function
    while (keepMoving) {
        //if current time minus start is equal to levelSpeed (1 second)
        //update the level view, increment the current line, and
        //update starting time to reset the 1 second count
        if (time(0) - start == levelSpeed) {
            //1 second has passed so move level by a line
            updateLevelView(totalLines, currentLevelLines, currentLine);
            currentLine++; //update the currentLine count

            //add current levelSpeed to starting time
            //so the 1 second count can restart
            start = start + levelSpeed;
        } //end if

        //check if still need to move along the level
        //if not (aka if currentLine is greater than or equal to the
        //last element - 5 of lines in the level file), end this thread
        if (currentLine >= totalLines - 5) {
            endOfLevel = true;
            myLvl.setStatus(false);
        }//end if

        //if status is false, thread needs to be terminated
        if (!myLvl.getStatus()) {
            //prompt the user to press the right arrow so the first thread can 
            //get the memo that it is time to terminate
            //\r will make sure next cout will jump to the beginning of the current line
            cout << endl << "Press right arrow to continue." << '\r';
            keepMoving = false; //end while loop and function
        } //end if
    } //end while
} //end of moveLevel function


/*
updateLevelView function :
   - updates the current level view by moving the view "down" by
   1 line
   - it will get and set the currentDisplay array attribute from the level class
   to be displayed in displayLevel function
   - calls on displayLevel to display the view updates and displayPlayer to
   assure the player shows on the bottom line of level view
*/
void updateLevelView(int totalLines, string currentLevelLines[], int currentLine) {
    //update current level view

    //only run if not end of level
    if (!endOfLevel) {
        int eofNum = totalLines - 1; // last index in currentLevelLines array

        //do not move elements if at eof
        if (currentLine <= eofNum - 5) {
            //first move level view down 1 by moving all elements forward 1 index
            //and adding the next line to the final element
            string tempDisplay[5] = { "" }; //temp display to hold previous display
            string updateD[5] = { "" }; //updated display to be set

            //fill temp display with current display elements
            string* display = myLvl.getDisplay();
            for (int i = 0; i < 5; i++) {
                tempDisplay[i] = *(display + i); //*(display + i) == display[i] (but is for pointer)
            } //end for

            //move the elements forward 1 index
            for (int i = 0; i < 5; i++) {
                //if not at final element, move the elements forward 1
                //else add the new line to the last element in the array
                if (i != 4) {
                    //move elements forward one
                    updateD[i] = tempDisplay[i + 1];
                }
                else {
                    int lineNum = currentLine + 5; //new line number to add -> currentLine + 5
                    //so it is the next line not yet in the currentDisplay array

                    //add new line to final element in display array
                    updateD[i] = currentLevelLines[eofNum - lineNum];
                } //end if
            } //end for

            //update the current display array
            myLvl.setDisplay(updateD);
        }//end if

        //make sure player is placed on screen where previously was
        //on the bottom of level view
        displayPlayer(0); //0 being no direction to move to

        //display the updates made above
        displayLevel();
    }
} //end of updateLevelView function


/*
displayLevel function :
   - clears screen and displays the current level view of 5 lines
   returned from the level class' getDisplay() function
** for some reason clear doesn't always run if user is pressing 
   left and right arrows too quickly, not a bug (aka doesn't stop gameplay)
   but definitely a glitch that shows double level for a moment **
*/
void displayLevel() {
    //display the current level view

    //clear screen
    system("CLS");

    //display level view
    string* display = myLvl.getDisplay(); //get pointer to currentDisplay array
    //go through the array reversed and display the level in console
    for (int i = 4; i >= 0; i--)
        cout << *(display + i) << endl;
    //end for
} //end of displayLevel function

/*
displayMenu function :
   - display menu of options for user depending on the levelSuccess (if user
   won the level or not)
   - C (+ won) will continue to next level or final screen,
   R (+ not won) will reset the level, and X will end the
   code (gameEnded bool set to true to end program)
   - will prompt user to enter a choice until a valid choice is entered
*/
void displayMenu() {
    //display the menu of options to user
    char choice = ' '; //user choice
    bool validChoice = false; //checks whether it was a valid choice or not

    //update player's score
    if (levelSuccess) {
        //if level was won, update score at current level
        score = scoreMult * level;
    }
    else {
        //else player lost so update score at level-1
        score = scoreMult * (level - 1); //ex. if level is 1, then 40*(1-1)=0 score
    } //end if

    //display player's score
    cout << "Score: ";
    cout << score;

    //if score is greater than 0 add an exclamation point afterwards
    //if score is 0 no exclamation point, sorry :(
    if (score > 0) {
        cout << "!                           " << endl;
    }
    else {
        cout << "                            " << endl;
    }//end if

    //if level was a success put a celebratory line,
    //else give a line of encouragement
    if (levelSuccess) {
        cout << "Woo! Go flower!" << endl << endl;
    }
    else {
        cout << "Keep trying you got his, flower!" << endl << endl;
    }

    //while choice is not valid, display menu to user
    while (!validChoice) {
        cout << "Level Menu" << endl;
        cout << "----------" << endl;
        //if level was successful (won) then prompt for C to continue
        //else prompt R to retry level
        if (levelSuccess)
            cout << "Enter C to continue" << endl;
        else
            cout << "Enter R to retry level" << endl;
        //end if
        cout << "Enter X to end game" << endl; //always give the users the option to end program

        //display prompt to user and user input choice
        cout << endl << "Please enter a choice: ";
        cin >> choice;
        choice = toupper(choice); //make sure choice is capitalized

        //check which choice was made
        if (choice == 'C' && levelSuccess) {
            //if C and won, continue to next level, aka increment level
            //but first check if have a level to go to
            if (level >= 3) { //3 is final level's index
                myLvl.setGameEnded(false); //game is not over yet
                displayEndScreen(); //display the ending screen
            }
            else
                level++; //increment level to play the next level
            //end if
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else if (choice == 'X') {
            //if X end the game
            myLvl.setGameEnded(true);
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else if (choice == 'R' && !levelSuccess) {
            //if R and not won, retry the level, aka do not change level
            //but make sure gameEnded is false
            myLvl.setGameEnded(false);
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else {
            //invalid character entered, display error to user
            cout << endl << "Invalid choice entered, please enter another choice." << endl << endl;
        } //end if
    } //end while
} //end of displayMenu function

/*
displayEndScreen function :
   - displays end screen to user and prompts them to enter R to restart or
   X to exit
*/
void displayEndScreen() {
    //declare and init end screen vars
    string finalDesc = "you helped the flower reach its photosynthesis goals";
    char c = ' ';
    bool validChoice = false;

    //clear screen
    system("CLS");

    //display end screen to user
    cout << "------------" << endl;
    cout << title << endl;
    cout << "------------" << endl << endl;
    cout << "thank you for playing!" << endl;
    cout << finalDesc << endl;
    cout << endl << endl;
    cout << "Final Score: " << score << "!" << endl << endl;

    //continue asking for choice until a valid choice is made (either x or r)
    while (!validChoice) {
        //prompt the user to end game or restart
        cout << "Enter X to exit or R to restart: ";
        cin >> c;
        c = toupper(c);

        //check what user entered
        if (c == 'X') {
            //if X end game
            myLvl.setGameEnded(true);
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else if (c == 'R') {
            //if R, reset game and level
            myLvl.setGameEnded(false); //not ended yet
            level = 1; //go back to first level
            validChoice == true; //a valid choice was entered
            return; //exit function
        }
        else {
            //error - not a valid character
            cout << "Please enter a valid character." << endl;
        } //end if
    } //end while
} //end of displayEndScreen function