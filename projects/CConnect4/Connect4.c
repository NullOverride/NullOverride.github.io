#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int connect_4[7][6];    
int cellsRemaining = 42;
int moves = 0;
int insert_board (int column, int value)
{
	int empty = findNextEmptySlotInColumn(column);
	if (empty == -1) {return 0;}
	connect_4[column][empty] = value;
	
	int returnValue = checkWin(value);
	
	
	return returnValue;
}

int checkWin(int value)
{
	// CHECK VERTICAL
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			if (getCell(x, y) != 0 && getCell(x, y) == getCell(x, y+1) && getCell(x, y) == getCell(x, y+2) && getCell(x, y) == getCell(x, y+3) )
			{
				return value;
			}
		}
	}
	
	// CHECK HORIZONTAL
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			if (getCell(x, y) != 0 && getCell(x, y) == getCell(x + 1, y) && getCell(x, y) == getCell(x + 2, y) && getCell(x, y) == getCell(x + 3, y) )
			{
				return value;
			}
		}
	}
	
	//CHECK DIAGONAL
	//Diagonal direction is either -1 or 1, therefore both top-to-bottom and bottom-to-top diagonals are checked.
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			for (int diagonalDirection = -1; diagonalDirection <= 1; diagonalDirection+=2) 
			{
				if (getCell(x, y) != 0 && getCell(x, y) == getCell(x + 1 * diagonalDirection, y+1) && getCell(x, y) == getCell(x + 2 * diagonalDirection , y+2) && getCell(x, y) == getCell(x + 3 * diagonalDirection, y+3) )
				{
					return value;
				}
			}
		}
	}
	if (cellsRemaining == 1 && checkWin(value) != 1 && checkWin(value) != 2) { return -1; } // no winner if board is filled and previous checks fail
	return 0;
}

int next_move()
{
	srand(time(NULL));
	int returnValue = 0;
	int priority[7] = {6,0,5,1,4,2,3}; // last = highest priority
	int privateBoard[7][6];
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			privateBoard[i][j] = connect_4[i][j]; // copy the board
		}
	}
	for(int i = 0; i < 7; i++)
	{
		if (findNextEmptySlotInColumn(i) != -1)
		{
			returnValue = i; // no good move left
		}
	}
	
	for(int l = 0; l < 7; l++)
	{
		if (insert_board(priority[l], 1) == 0 && insert_board(priority[l], 2) != 2 && findNextEmptySlotInColumn(priority[l]) != -1) // next best move based on priority
		{
			returnValue = priority[l];
		}

		for(int i = 0; i < 7; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				connect_4[i][j] = privateBoard[i][j]; // restore board
			}
		}
	}
	
	for(int k = 0; k < 7; k++)
	{
		if (insert_board(k, 2) == 2 && findNextEmptySlotInColumn(k) != -1) // check defensive move
		{
			returnValue = k;
		}

		for(int i = 0; i < 7; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				connect_4[i][j] = privateBoard[i][j]; // restore board
			}
		}
	}
	
	for(int k = 0; k < 7; k++)
	{
		if (insert_board(k, 1) == 1 && findNextEmptySlotInColumn(k) != -1) // check winning move
		{
			returnValue = k;
		}

		for(int i = 0; i < 7; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				connect_4[i][j] = privateBoard[i][j]; // copy the board
			}
		}
	}

	return returnValue; //1-7;
}  

void printBoard()
{
	for(int j = 5; j >= 0; j--)
	{
		for(int i = 0; i < 7; i++)
		{
			printf("%i ", connect_4[i][j]);
		}
		printf("\n");
	}
}
int getCell(int x, int y) 
{
	return (y<0||x<0||y>=6||x>=7) ? 0 : connect_4[x][y];
}

int findNextEmptySlotInColumn(int x)
{
		int y = -1;
		for (int i = 5; i >= 0; i--)
		{
			if (connect_4[x][i] == 0)
			{
				y = i;
			}
		}
		return y;
}

int main(int argc, char** argv)
{
	if (argc != 2 || (atoi(argv[1]) != 1 && atoi(argv[1]) != 2))
	{
		printf("Usage: %s [1 or 2 players]" , argv[0]);
		return 1;
	}
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			connect_4[i][j] = 0;
		}
	}

	if (atoi(argv[1]) == 2)
	{
		int player = 1;
		int col;
		printBoard();	
		printf("Player %i's move: ", player);
		scanf("%d", &col);
		col = col - 1;
		while (col < 0 || col > 6)
		{
			printf("Invalid move - Out of bounds\n");
			scanf("%d", &col);
			col = col - 1;
		}
		int winner;
		int colFlags[6] = { 1, 1, 1, 1, 1, 1};
		while ((winner = insert_board(col, player)) == 0)
		{
			printf("Column chosen = %d \n" , col);
			if (col < 0 || col > 6)
			{
				printf("Invalid move - Out of bounds\n");
			}
			else if (colFlags[col] == 0)
			{
				printf("Column %d is full\n", col + 1);
			}
			else 
			{
				cellsRemaining--;
				player = (player == 1) ? 2 : 1;
			}
			if (findNextEmptySlotInColumn(col) == -1)
			{
				colFlags[col] = 0;
			}
			printBoard();
			printf("Player %i's move: ", player);
			scanf("%d", &col);
			col = col - 1;
		}
		printBoard();
		if (winner == 1 || winner == 2)
		{
			printf("The winner is player %d! \n", winner);
		}
		if (winner == -1)
		{
			printf("Tie game! \n");
		}	
		return 1;
	}
	else if (atoi(argv[1]))
	{
		int player = 1;
		insert_board(3, player);
		printBoard();
		moves++;
		printf("Total moves: %d \n", moves);
		printf("Your move: ");
		int col;
		scanf("%d", &col);
		col = col - 1;
		while (col < 0 || col > 6)
		{
			printf("Invalid move - Out of bounds\n");
			scanf("%d", &col);
			col = col - 1;
		}
		moves+=2;
		int winner;
		int colFlags[6] = { 1, 1, 1, 1, 1, 1};
		while ((winner = insert_board(col, 2)) == 0)
		{
			printf("Total moves: %d \n", moves);
			//printf("Column chosen = %d \n" , col + 1);
			if (col < 0 || col > 6)
			{
				printf("Invalid move - Out of bounds\n");
			}
			else if (colFlags[col] == 0)
			{
				printf("Column %d is full\n", col + 1);
			}
			else 
			{
				moves+=2;
				cellsRemaining-=2;
				if(insert_board(next_move(), 1) == 1)
				{
					winner = 1;
					break;
				}
			}
			if (findNextEmptySlotInColumn(col) == -1)
			{
				colFlags[col] = 0;
			}
			printBoard();
			printf("Your move: ");
			scanf("%d", &col);
			col = col - 1;
		}
		printBoard();
		printf("Total moves: %d \n", moves);
		if (winner == 1)
		{
			printf("The CPU wins!");
		}
		if (winner == 2)
		{
			printf("You win!");
		}
		if (winner == -1)
		{
			printf("Tie game! \n");
		}
		return 1;
	}
	
	return 0;
	
}