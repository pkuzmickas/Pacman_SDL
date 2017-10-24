#include "../Headers/Board.h"
#include <iostream>
using namespace std;

/*
	0 - EMPTY
	1 - WALL
	2 - SUPERCANDY
	3 - CANDY
	4 - PACMAN
*/


Board::~Board() {
	for (int i = 0; i < COLUMNS; i++) {
		for (int j = 0; j < ROWS; j++) {
			if(candyBoard[i][j]!=NULL) delete candyBoard[i][j];
			if (board[i][j] != NULL) delete board[i][j];
		}
	}
	//if(candyBoard!=NULL)
	//delete candyBoard;

	//delete[] board;
}

Board::Board() {
	int setupMatrix[ROWS][COLUMNS] = {  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 2, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 2, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
										{ 1, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 1 },
										{ 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
										{ 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
										{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
										{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	};

	for (int i = 0; i < COLUMNS; i++) {
		for (int j = 0; j < ROWS; j++) {
			board[i][j] = new int;
			candyBoard[i][j] = new bool;
			*board[i][j] = setupMatrix[j][i];
			 if (*board[i][j] == 0 && (i<7 && j!=14 || j<9 || i>20 && j!=14 || j>19)) {
				*candyBoard[i][j] = true;
				MaxCandy++;
				CandyNumber++;
			}
			else {
				*candyBoard[i][j] = false;
			}
		}
	}
	*candyBoard[6][14] = true;
	*candyBoard[21][14] = true;

	//*board[STARTCOL][STARTROW] = 4;

}


void Board::printBoard() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			cout << *board[j][i] << " ";
		}
		cout << endl;
	}
}

void Board::eatCandy(int col, int row) { 
	if (*candyBoard[col][row]) {
		*candyBoard[col][row] = false;
		CandyNumber--;
	}
	
}

void Board::findAPossibleMove(std::string direction, int &startCol, int &startRow)
{
	if (direction == "left") {

		for (int i = startCol - 1; i >= 0; i--)
		{
			if (*board[i][startRow] == 1) {
				startCol = i + 1;
				return;
			}
			if (i == 0 && *board[i][startRow] == 0) {
				startCol = i;
			}
		}
	}
	if (direction == "right") {
		for (int i = startCol + 1; i < COLUMNS; i++)
		{
			if (*board[i][startRow] == 1) {
				startCol = i - 1;
				return;
			}
			if (i == COLUMNS - 1 && *board[i][startRow] == 0) {
				startCol = i;
			}
		}

	}
	if (direction == "up") {
		for (int i = startRow - 1; i >= 0; i--)
		{
			if (*board[startCol][i] == 1) {
				startRow = i + 1;
				return;
			}
		}
	}
	if (direction == "down") {
		for (int i = startRow + 1; i < ROWS; i++)
		{
			if (*board[startCol][i] == 1) {
				startRow = i - 1;
				return;
			}
		}
	}

	
}

void Board::findAPossibleTurn(std::string directionMoving, std::string directionToTurn, int &startCol, int &startRow) {

	if (directionMoving == "left") {
		if (directionToTurn == "up") {
			for (int i = startCol; i >= 0; i--) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow - 1] == 0) {
					startCol = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "down") {
			for (int i = startCol; i >= 0; i--) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow + 1] == 0) {
					startCol = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "any") {
			for (int i = startCol-1; i >= 0; i--) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow + 1] == 0 || *board[i][startRow - 1] == 0) {
					startCol = i;
					return;
				}
			}
			startCol = -1;
		}
	}
	else if (directionMoving == "right") {
		if (directionToTurn == "up") {
			for (int i = startCol; i < COLUMNS; i++) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow - 1] == 0) {
					startCol = i;

					return;
				}
			}

			return;
		}
		if (directionToTurn == "down") {
			for (int i = startCol; i < COLUMNS; i++) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow + 1] == 0) {
					startCol = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "any") {
			for (int i = startCol+1; i < COLUMNS; i++) {
				if (*board[i][startRow] == 1) return;
				if (*board[i][startRow + 1] == 0 || *board[i][startRow - 1] == 0) {
					startCol = i;
					return;
				}
			}
			startCol = COLUMNS;
		}
	}
	else if (directionMoving == "up") {
		if (directionToTurn == "left") {
			for (int i = startRow; i >= 0; i--) {
				if (*board[startCol][i] == 1) return;
				if (*board[startCol-1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "right") {
			for (int i = startRow; i >= 0; i--) {
				if (*board[startCol][i] == 1) return;
				if (*board[startCol + 1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "any") {
			for (int i = startRow-1; i >= 0; i--) {
				if (*board[startCol][i] == 1) return;
				if (*board[startCol + 1][i] == 0 || *board[startCol-1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
	}
	else if (directionMoving == "down") {
		if (directionToTurn == "left") {
			
			for (int i = startRow; i < ROWS; i++) {
				if (*board[startCol][i] == 1) {
					return;
				}
				if (*board[startCol-1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "right") {
			for (int i = startRow; i < ROWS; i++) {
				if (*board[startCol][i] == 1) return;
				if (*board[startCol + 1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
		if (directionToTurn == "any") {
			for (int i = startRow+1; i < ROWS; i++) {
				if (*board[startCol][i] == 1) return;
				if (*board[startCol + 1][i] == 0 || *board[startCol - 1][i] == 0) {
					startRow = i;
					return;
				}
			}
			return;
		}
	}
}