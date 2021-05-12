/*
START of gooseEscapeGamePlay.cpp
*/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;
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
	
/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
*/

// print the game board function
void printBoard(ifstream & fin, int gameBoard[NUM_BOARD_X][NUM_BOARD_Y])
{
	for(int row = 0; row < NUM_BOARD_X; row++)
	{
		for(int col = 0; col < NUM_BOARD_Y; col++)
		{	
			fin >> gameBoard[row][col];
			
			if(gameBoard[row][col] == SHALL_NOT_PASS)
			{
				terminal_put(row, col, WALL_CHAR);
			}
		}
	}

    /*
    Setting up the freeze powerups and health powerups
    */
    
    int powerup_row = 0;
	int powerup_column = 0;
	
    for(int powerup_index = 0; powerup_index < NUM_OF_BOOST_POWERUPS; powerup_index++) //inserting freeze powerups
    {
        powerup_row = rand() % NUM_BOARD_X;
        powerup_column = rand() % NUM_BOARD_Y;
        while (gameBoard[powerup_row][powerup_column] != EMPTY)
        {
            powerup_row = rand() % NUM_BOARD_X;
            powerup_column = rand() % NUM_BOARD_Y;
        }
        gameBoard[powerup_row][powerup_column] = BOOST_POWERUP;
        terminal_put(powerup_row,powerup_column,BOOST_POWERUP);
    }
    
    for (int powerup_index = 0; powerup_index < NUM_OF_HEALTH_POWERUPS; powerup_index++) //inserting health powerups
    {
        powerup_row = rand() %  NUM_BOARD_X;
        powerup_column = rand() % NUM_BOARD_Y;
        while (gameBoard[powerup_row][powerup_column] != EMPTY)
        {
            powerup_row = rand() %  NUM_BOARD_X;
            powerup_column = rand() % NUM_BOARD_Y;
        }
        gameBoard[powerup_row][powerup_column] = HEALTH_POWERUP;
        terminal_put(powerup_row,powerup_column,HEALTH_POWERUP);
    }
	
    for (int powerup_index = 0; powerup_index < NUM_OF_TP_POWERUPS; powerup_index++) //inserting teleport powerups
    {
        do
        {
            powerup_row = rand() %  NUM_BOARD_X;
            powerup_column = rand() % NUM_BOARD_Y;
        }
        while(gameBoard[powerup_row][powerup_column] != EMPTY);
        gameBoard[powerup_row][powerup_column] = TELEPORTER_POWERUP;
        terminal_put(powerup_row,powerup_column,TELEPORTER_POWERUP);
    }
}

//health 

void updateHealth(Actor const & player)
    {
        string totalHealth = "";
        for(int count = 0; count < player.get_health(); count++)
        {
            totalHealth += HP;
        }
        for(int count = 0; count < NUM_OF_HEALTH_POWERUPS; count++)
        {
        	out.writeLine("");
		}
       	out.writeLine(totalHealth);
    }
    
/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/
bool winCheck(Actor const & player, int win_x, int win_y)
{
	return ((player.get_x() ==  win_x) && (player.get_y() == win_y));
}

bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x()
         && player.get_y() == monster.get_y());
}

void gooseBattle(Actor & player, Actor & monster, int battleNumberPlayer, int battleNumberMonster)
{
    player.hp_decrement();
    if (battleNumberMonster != battleNumberPlayer)
        player.hp_decrement(); //decrease health by 2
    updateHealth(player);
}

void messageAfterBattle (int battleNumberMonster, int battleNumberPlayer, Actor & player)
{
    if (battleNumberMonster != battleNumberPlayer)
    {
        cout << "Oh no! You lost 2 health because you guessed the wrong number." << endl;
        cout << "You have " << player.get_health() << "hp  left." << endl;
    }
    else
    {
        cout << "Phew! You lost only 1 health because you guessed the correct number." << endl;
        cout << "You have " << player.get_health() << "hp  left." << endl;
    }
}
void moveGooseAfterBattle(Actor & monster, Actor & player, int gameBoard[NUM_BOARD_X][NUM_BOARD_Y])
{
	int tempX = 0, tempY = 0;
	tempX = player.get_x();
	tempY = player.get_y();
    int spacesMovedAfterBattle = GOOSE_MOVES_AFTER_BATTLE;
    int newLocationRow = 0, newLocationColumn = 0;
    gameBoard[monster.get_x()][monster.get_y()] = EMPTY;
    /*
    Following iterations are to check if the goose is near the edges of the
    board. To ensure the new goose location is not outside the board.
    */

	do   
    {
    if (monster.get_x() > spacesMovedAfterBattle && monster.get_x() < (NUM_BOARD_X - spacesMovedAfterBattle)) //get new x-coordinate
        newLocationRow = rand() % (NUM_BOARD_X - monster.get_x()) + (2 * spacesMovedAfterBattle);
    else if (monster.get_x() <= spacesMovedAfterBattle)
        newLocationRow = rand() % monster.get_x();
    else
        newLocationRow = rand() % (NUM_BOARD_X - monster.get_x()) + monster.get_x();
    
    if (monster.get_y() > spacesMovedAfterBattle && monster.get_y() < (NUM_BOARD_Y - spacesMovedAfterBattle)) //get new y-coordinate
        newLocationColumn = rand() % (NUM_BOARD_Y - monster.get_y()) + (2 * spacesMovedAfterBattle);
    else if (monster.get_y() <= spacesMovedAfterBattle)
        newLocationColumn = rand() % monster.get_y();
    else
        newLocationColumn = rand() % (NUM_BOARD_Y - monster.get_y()) + monster.get_y();
    }
    while (gameBoard[newLocationRow][newLocationColumn] != EMPTY);
	
	gameBoard[newLocationRow][newLocationColumn] = MONSTER_CHAR;
    monster.update_location(newLocationRow - monster.get_x(), newLocationColumn - monster.get_y());
    
}
/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/

void movePlayer(int key, Actor & player, int gameBoard[NUM_BOARD_X][NUM_BOARD_Y])
{
    gameBoard[player.get_x()][player.get_y()] = EMPTY;
    int yMove = 0, xMove = 0;
    int yChange = 0, xChange = 0;
   
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
    
    yChange = yMove + player.get_y();
    xChange = xMove + player.get_x();

    if (gameBoard[xChange][yChange] == HEALTH_POWERUP) //player has picked up the health powerup
    {
        gameBoard[player.get_x()][player.get_y()] = EMPTY;
        player.hp_increment();
        updateHealth(player);
    }
    
    if (gameBoard[xChange][yChange] == TELEPORTER_POWERUP)
    {
    	gameBoard[player.get_x()][player.get_y()] = EMPTY;
    	int newLocationX = 0, newLocationY = 0;
	    do
	    {
	         newLocationX = rand() % NUM_BOARD_X;
	         newLocationY = rand() % NUM_BOARD_Y; 
	    }
	    while(gameBoard[newLocationX][newLocationY] != EMPTY);
	    player.update_location(newLocationX - player.get_x(), newLocationY - player.get_y());
	    gameBoard[player.get_x()][player.get_y()] = PLAYER_CHAR;
	    cout << "WOOOOOOSH! The teleporter pulls you in and you appear in a new place." << endl;
	}
    
    if(player.can_move(xMove, yMove) && gameBoard[xChange][yChange] != SHALL_NOT_PASS)
    {
        player.update_location(xMove, yMove);
    }
    gameBoard[player.get_x()][player.get_y()] = PLAYER_CHAR;
}

bool checkBoostPowerup(int key, Actor & player, int gameBoard[NUM_BOARD_X][NUM_BOARD_Y])
{
    gameBoard[player.get_x()][player.get_y()] = EMPTY;
    int yMove = 0, xMove = 0;
    int yChange = 0, xChange = 0;
   
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
    
    yChange = yMove + player.get_y();
    xChange = xMove + player.get_x();
    
    if(gameBoard[xChange][yChange] == BOOST_POWERUP)
    {
    	gameBoard[player.get_x()][player.get_y()] = EMPTY;
		return true;
    }
	else
	{
		return false;  
	}
}	
/*
 What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/

void gooseChase(Actor & monster, Actor & player, int gameBoard[NUM_BOARD_X][NUM_BOARD_Y])
{
    gameBoard[monster.get_x()][monster.get_y()] = EMPTY;
	int xMove = 0, yMove = 0;
    if (player.get_x() < monster.get_x())
    {
    	xMove = -1;
	}
    else if (player.get_x() > monster.get_x())
    {
    	xMove = 1;
	}   
    if (player.get_y() < monster.get_y())
    {
    	yMove = -1;
	}
    else if (player.get_y() > monster.get_y())
    {
    	yMove = 1;
	}
	monster.update_location(xMove, yMove);
    gameBoard[monster.get_x()][monster.get_y()] = MONSTER_CHAR;
}
/*
END of gooseEscapeGamePlay.cpp
*/
