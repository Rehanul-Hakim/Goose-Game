/*
START of gooseEscapeUtil.hpp
*/

//STUDENTS: Be very careful if you decide to change these values

#ifndef GOOSE_UTIL
#define GOOSE_UTIL
/*
With graphics, screens are given an x,y coordinate system with the origin
in the upper left corner.  So it means the coordinate axes are:
---------------->   x direction
|
|
|
|
|
V

y direction 
*/

// Screen layout
const int NUM_SCREEN_X = 80;
const int NUM_SCREEN_Y = 25;
const char SETUP_MESSAGE[] = "window: title='Escape the Goose', size=80x25";
const int MIN_SCREEN_X = 0;
const int MIN_SCREEN_Y = 0;
const int MAX_SCREEN_X = NUM_SCREEN_X - 1;
const int MAX_SCREEN_Y = NUM_SCREEN_Y - 1;

// Play area layout
const int NUM_BOARD_X = 80;  // needs to be <= NUM_SCREEN_X
const int NUM_BOARD_Y = 21;  // needs to be < NUM_SCREEN_Y
const int MIN_BOARD_X = 0;
const int MIN_BOARD_Y = 0;
const int MAX_BOARD_X = NUM_BOARD_X - 1;
const int MAX_BOARD_Y = NUM_BOARD_Y - 1;

// Console message area layout
const int NUM_CONSOLE_X = 79;  // needs to be <= NUM_SCREEN_X
const int NUM_CONSOLE_Y = NUM_SCREEN_Y - NUM_BOARD_Y;
const int MIN_CONSOLE_X = 1;
const int MIN_CONSOLE_Y = MAX_BOARD_Y + 1;
const int MAX_CONSOLE_X = MAX_SCREEN_X;
const int MAX_CONSOLE_Y = MAX_SCREEN_Y;

//Gameplay features
const int STARTING_HEALTH = 3;
const string HP = "<3 ";
const int NUM_OF_TP_POWERUPS = 2; //we will have 2 teleporter powerups
const int NUM_OF_BOOST_POWERUPS = 3; //we will have 3 boost powerups
const int NUM_OF_HEALTH_POWERUPS = 3; //we will have 3 health powerups
const int GOOSE_MOVES_AFTER_BATTLE = 10;
const int BOOST_POWER_MOVES = 20;
#endif
/*
END of gooseEscapeUtil.hpp
*/
