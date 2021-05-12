/*
START OF SOURCE FILE
*/
#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <time.h> //used for random number seeding
#include <fstream>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//Set up the console.
Console out;

int main()
{
	srand(time(0)); //used for random number seeding
	//Set up the window.
    terminal_open();
  	terminal_set(SETUP_MESSAGE);
	
	ifstream fin("map.txt"); //Assume file opens correctly
/*
    The code below provides a skeleton of the game play.  You will need to
    write code for setting up the game board, and playing the game itself.
    You can modify the code given as needed.
    
    Call the functions that you have written in the game play file, and that
    you have added to the Actor class.
*/
	
    // Declare the array that will hold the game board "map"
  	int gameBoard [NUM_BOARD_X][NUM_BOARD_Y] = {0};    
/*
    Initiallize locations in the game board to have game features.  What if you
    have man things to add to the game board?  Should you use a loop?  Does it
    make sense to store this information in a file?  Should this code be a
    function as well?
*/
	
    // Call the function to print the game board
    printBoard(fin, gameBoard);
    
    //Print player and monster randomly
  	int actor_posX = 0, actor_posY = 0;
  	do
        {
            actor_posX = rand() % NUM_BOARD_X;
            actor_posY = rand() % NUM_BOARD_Y;
        }
        while(gameBoard[actor_posX][actor_posY] != EMPTY);
        Actor player(PLAYER_CHAR, actor_posX,actor_posY);
        
  	do
        {
            actor_posX = rand() % NUM_BOARD_X;
            actor_posY = rand() % NUM_BOARD_Y;
        }
        while(gameBoard[actor_posX][actor_posY] != EMPTY);
        Actor monster(MONSTER_CHAR, actor_posX,actor_posY);
    
    //Print win char randomly
    int win_X = 0, win_Y = 0;
	do
	{
		win_X = rand() % NUM_BOARD_X;
        win_Y = rand() % NUM_BOARD_Y;
	}
	while(gameBoard[win_X][win_Y] != EMPTY);
	
	gameBoard[win_X][win_Y] = WIN_CHAR;
	terminal_put(win_X,win_Y, WIN_CHAR);
	// Printing the instructions
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you'll have to battle it out'!");
	out.writeLine("Be careful! The goose smashes through walls to get to you!");
/*
    This is the main game loop.  It continues to let the player give input
    as long as they do not press escape or close, they are not captured by
    the goose, and they didn't reach the win tile
*/
/*
    All key presses start with "TK_" then the character.  So "TK_A" is the "a"
    key being pressed.
*/
	int gooseBattleNum = 0, playerBattleNum = 0;
	int numOfBoostMoves = 0;
    bool printHealth = true; //updated to false when key pressed in loop
    int keyEntered = TK_A;  // can be any valid value that is not ESCAPE or CLOSE
    while (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && !winCheck(player, win_X, win_Y) && player.get_health() > 0)
	{
		terminal_put(win_X,win_Y, WIN_CHAR);
	    // get player key press
	    keyEntered = terminal_read();
        
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
            if(checkBoostPowerup(keyEntered, player, gameBoard))
            {
                while ((!winCheck(player, win_X, win_Y)) && (numOfBoostMoves < BOOST_POWER_MOVES))
                {
                    movePlayer(keyEntered, player, gameBoard);
                    numOfBoostMoves++;
                }
                numOfBoostMoves = 0;
            }
            else
            {
                // move the player, you can modify this function
    	        movePlayer(keyEntered, player, gameBoard);

                // call the goose's chase function
                gooseChase(monster, player, gameBoard);
            }
            // call other functions to do stuff?		    
        }
        if (captured(player,monster))
        {
            gooseBattleNum = rand() % 5 + 1;
            //cout << "PSSSST, its " << gooseBattleNum << endl;
            cout << "Guess the correct number between 1 to 5 :O" << endl;
            cin >> playerBattleNum;
            gooseBattle(player, monster, playerBattleNum, gooseBattleNum);
            messageAfterBattle(gooseBattleNum, playerBattleNum, player);
            moveGooseAfterBattle(monster, player, gameBoard);
        }
        if(printHealth == true) //Prints health to console after key is pressed
        {
        	terminal_refresh();
            updateHealth(player);
        }
        printHealth = false;
  	}

    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        out.writeLine("Game has ended");
    
        // output why:  did the goose get us, or did we get to the win location?
        if (keyEntered == TK_ESCAPE)
            cout << "You have decided to closed game." << endl;
        else
            if(player.get_health() <= 0)
                cout << "Oh no! The goose has captured you! You lose :(" << endl;
            else
                cout << "Prepare for extraction, UWaterloo Police is on its way! :)" << endl;
            
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	//game is done, close it  
	fin.close();
    terminal_close();
}
/*
END OF SOURCE FILE
*/
