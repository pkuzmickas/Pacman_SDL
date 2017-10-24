#pragma once
#include "Definitions.h"
#include <string>
class Board {
private:
	int* board[COLUMNS][ROWS];
	bool* candyBoard[COLUMNS][ROWS];
	int CandyNumber = 0, MaxCandy = 0;
public:
	void setBoard(int col, int row, int val) { *board[col][row] = val; }
	void eatCandy(int col, int row);
	int candyEaten() { return MaxCandy - CandyNumber; }
	int getTotalCandy() { return MaxCandy; }
	int getValue(int col, int row) { return *board[col][row]; }
	int isCandy(int col, int row) { return *candyBoard[col][row]; }
	Board();
	~Board();
	void printBoard();
	void findAPossibleMove(std::string direction, int &startCol, int &startRow);
	void Board::findAPossibleTurn(std::string directionMoving, std::string directionToTurn, int &startCol, int &startRow);
};