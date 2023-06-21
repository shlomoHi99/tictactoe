#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3

//Forward declaretions:
void PrintBoard(int board[BOARD_SIZE][BOARD_SIZE], int withIndex, int withWin, int lastMove);
int IsWinning(int board[BOARD_SIZE][BOARD_SIZE], int sideToCheck);
int AskIfAnotherGame(int didPlayerWon);
int PlayersMove(int board[BOARD_SIZE][BOARD_SIZE], int playerSymbol);
int ComputerMove(int board[BOARD_SIZE][BOARD_SIZE], int computerSymbol, int playerLastMove);
int CheckIfThreatened(int board[BOARD_SIZE][BOARD_SIZE], int computerSymbol, int playerLastMove);
int FindOtherCorner(int corner[2]);
int IsThereAnotherTheSameInLine(int board[BOARD_SIZE][BOARD_SIZE], int symbol, int place);
int IsThirdInLineEmpty(int board[BOARD_SIZE][BOARD_SIZE], int symbol, int place1);
int IsBoardFull(int board[BOARD_SIZE][BOARD_SIZE]);

int gNewGame;

void main()
{
	srand((int)time(NULL));
	int board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	int playerTool = rand() % 2 + 1;
	int sideToPlay = 1;//rand() % 2 + 1;
	int playerLastMove;
	int computerTool;
	int computerMove;
	int winningPlace;
	char symbols[2] = { 'X','O' };

	do {
		
		playerTool %= 2 + 1;
		computerTool = playerTool % 2 + 1;
		sideToPlay %= 2;
		playerLastMove = -1;
		//init game:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				board[i][j] = 0;
			}
		}
		gNewGame = 1;
		system("CLS");
		
		while (!IsWinning(board, (sideToPlay+1) % 2+1) && !IsBoardFull(board)){
			if (sideToPlay % 2 +1 == playerTool)
			{		
				if (gNewGame)
				{		
					printf("Player = %c\tComputer = %c\n", symbols[playerTool - 1], symbols[computerTool - 1]);	
					PrintBoard(board, 1,0,0);
				}
				playerLastMove = PlayersMove(board, playerTool);
				if (!IsBoardFull(board)&& !IsWinning(board, (sideToPlay + 1) % 2 + 1))
				{
					system("CLS");				
					printf("Player = %c\tComputer = %c", symbols[playerTool - 1], symbols[computerTool - 1]);
				}
				board[playerLastMove / 10 % 3][playerLastMove % 10 % 3] = playerTool;
				printf("\nplayers move:");
				winningPlace = IsWinning(board, sideToPlay % 2 + 1);
				PrintBoard(board, 0, winningPlace, playerLastMove);
			}
			else
			{
				if (gNewGame)
				{
					printf("Player = %c\tComputer = %c", symbols[playerTool - 1], symbols[computerTool - 1]);
				}
				computerMove = ComputerMove(board, playerTool, playerLastMove);
				board[computerMove / 10 % 3][computerMove % 10 % 3] = computerTool;
				printf("\ncomputers move:");
				winningPlace = IsWinning(board, sideToPlay % 2 + 1);
				PrintBoard(board, 0, winningPlace, computerMove);
			}
			sideToPlay++;
		}
	} while (AskIfAnotherGame(IsWinning(board, playerTool)));
}

void PrintBoard(int board[BOARD_SIZE][BOARD_SIZE], int withIndex, int withWin, int lastMove)
{
	char symbols[3] = {' ', 'X', 'O'};
	char delimiters[5] = { ' ','-','|','\\', '/' };
	int i = 0, j = 0;

	if (withIndex)
	{
		printf("\n\t\tcol ->\n\n");
		printf("\t row    1  |2  |3  \n");
		printf("\t  |     %2c |%2c |%2c \n", symbols[board[i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t  v     ___|___|___\n");
		printf("\t\t2  |   |   \n");
		printf("\t\t%2c |%2c |%2c \n", symbols[board[++i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t\t___|___|___\n");
		printf("\t\t3  |   |   \n");
		printf("\t\t%2c |%2c |%2c \n", symbols[board[++i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t\t   |   |   \n\n");
	}
	else if(!withWin)
	{
		printf("\n\n\t   |   |   \n");
		printf("\t%2c |%2c |%2c \n", symbols[board[i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t___|___|___\n");
		printf("\t   |   |   \n");
		printf("\t%2c |%2c |%2c \n", symbols[board[++i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t___|___|___\n");
		printf("\t   |   |   \n");
		printf("\t%2c |%2c |%2c \n", symbols[board[++i][j]], symbols[board[i][j + 1]], symbols[board[i][j + 2]]);
		printf("\t   |   |   \n");
	}
	else 
	{
		int rows[3] = { 0 }, cols[3] = {0}, diagnal1 = 0, diagnal2 = 0;
		

		if (withWin == 2)
		{
			diagnal1 = 3;
		}
		else if (withWin == -2)
		{
			diagnal2 = 4;
		}
		else if (withWin == 1)
		{
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				if (i  == lastMove / 10)
				{
					rows[i] = 1;
				}
			}
		}
		else if (withWin == -1)
		{
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				if (i== lastMove % 10)
				{
					cols[i] = 2;
				}
			}
		}
		printf("\n\n\t%c%c | %c | %c%c\n", delimiters[diagnal1], delimiters[cols[0]], delimiters[cols[1]], delimiters[cols[2]], delimiters[diagnal2]);
		printf("\t%c%c%c|%c%c%c|%c%c%c\n", delimiters[rows[0]], symbols[board[i][j]], delimiters[rows[0]], delimiters[rows[0]]
			, symbols[board[i][j + 1]], delimiters[rows[0]], delimiters[rows[0]], symbols[board[i][j + 2]], delimiters[rows[0]]);
		printf("\t___|___|___\n");
		printf("\t %c |%c%c%c| %c \n", delimiters[cols[0]], delimiters[diagnal1], delimiters[cols[1]], delimiters[diagnal2], delimiters[cols[2]]);
		printf("\t%c%c%c|%c%c%c|%c%c%c\n", delimiters[rows[1]], symbols[board[++i][j]], delimiters[rows[1]], delimiters[rows[1]]
			, symbols[board[i][j + 1]], delimiters[rows[1]], delimiters[rows[1]], symbols[board[i][j + 2]], delimiters[rows[1]]);
		printf("\t___|___|___\n");
		printf("\t %c%c| %c |%c%c \n", delimiters[cols[0]], delimiters[diagnal2], delimiters[cols[1]], delimiters[diagnal1], delimiters[cols[2]]);
		printf("\t%c%c%c|%c%c%c|%c%c%c\n", delimiters[rows[2]], symbols[board[++i][j]], delimiters[rows[2]], delimiters[rows[2]]
			, symbols[board[i][j + 1]], delimiters[rows[2]], delimiters[rows[2]], symbols[board[i][j + 2]], delimiters[rows[2]]);
		printf("\t%c%c | %c | %c%c\n", delimiters[diagnal2], delimiters[cols[0]], delimiters[cols[1]], delimiters[cols[2]], delimiters[diagnal1]);
	}
}

int IsWinning(int board[BOARD_SIZE][BOARD_SIZE], int sideToCheck)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if ((board[i][0] == board[i][1] && board[i][1] == board[i][2]) && board[i][1] == sideToCheck )
		{
			return 1;

		}
		if ((board[0][i] == board[1][i] && board[1][i] == board[2][i]) && board[1][i] == (sideToCheck ))
		{
			return -1;
		}
	}
	if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) && board[1][1] == sideToCheck)
	{
		return 2;
	}
	if ((board[0][2] == board[1][1] && board[1][1] == board[2][0]) && board[1][1] == sideToCheck)
	{
		return -2;
	}
	return 0;
}

int IsBoardFull(int board[BOARD_SIZE][BOARD_SIZE])
{
	int numOfFullPlaces = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] != 0)
			{
				numOfFullPlaces++;
			}
		}
	}
	return (numOfFullPlaces == 9);
}

int AskIfAnotherGame(int didPlayerWon)
{
	int anotherGame = 0;

	printf("\n**************************************************\n\n");
	if (didPlayerWon)
	{
		printf("\tYay you won!");
	}
	else
	{
		printf("\tBetter luck next time");
	}
	printf("\n\n************************************************\n\n");
	printf("If you want to play again type 735, if not type enything else\n");
	scanf("%d", &anotherGame);
	return (anotherGame == 735);
}

int PlayersMove(int board[BOARD_SIZE][BOARD_SIZE], int playerSymbol)
{
	int temp, row, col;


	printf("\nNow is your turn to take a move.\n");
	printf("Enter the row number (between 1 to 3), and then the col number (between 1 to 3):\n");
	printf("If you would like to see the board with index type 9.\n");
	scanf("%d",&temp);
	if (temp == 9)
	{
		PrintBoard(board, 1,0,0);
		printf("\nEnter the row number (between 1 to 3), and the col number (between 1 to 3):\n");
		scanf("%d%d", &row, &col);
	}
	else
	{
		row = temp;
		scanf("%d", &col);
	}
	if (row < 1 || row > 3 || col < 1 || col > 3)
	{
		printf("The number you entered is invalid!\nLet's try again.\n");
		_flushall();
		return PlayersMove(board, playerSymbol);
	}
	if (board[row - 1][col - 1] == 0)
	{
		return (row - 1) * 10 + (col - 1);
	}
	else
	{
		printf("Oops this spot is taken");
		PlayersMove(board, playerSymbol);
	}
	return -1;
}

int ComputerMove(int board[BOARD_SIZE][BOARD_SIZE], int computerSymbol, int playerLastMove)

{
	static int computerLastMove;
	srand((int)time(NULL));
	int playerSymbol, currentCheck, temp;
	int sides[4][2] = { {0,1},{1,0},{1,2},{2,1} };
	int corners[4][2] = { {0,0},{2,2},{0,2},{2,0} };
	int row, col;
	int grade[4] = { 0 };
	int max = 0;
	int maxIndex = -1;

	if (gNewGame)
	{
		computerLastMove = -1;
		gNewGame = 0;
	}
	if (board[1][1] == 0)
	{
		computerLastMove = 1 * 10 + 1;
		return computerLastMove;
	}
	if (computerLastMove < 0)
	{
		temp = rand() % 4;
		computerLastMove = corners[temp][0] * 10 + corners[temp][1];
		return computerLastMove;
	}
	playerSymbol = computerSymbol % 2 + 1;
	currentCheck = CheckIfThreatened(board, playerSymbol, computerLastMove);
	if (currentCheck >= 0)
	{
		computerLastMove = currentCheck;
		return computerLastMove;
	}
	currentCheck = CheckIfThreatened(board, computerSymbol, playerLastMove);
	if (currentCheck >= 0)
	{
		computerLastMove = currentCheck;
		return computerLastMove;
	}
	for (int i = 0; i < 4; i++)
	{
		if (board[corners[i][0]][corners[i][1]] == 0)
		{
			//checks if the corner across the board is free
			temp = FindOtherCorner(corners[i]);
			row = temp / 10 % 3;
			col = temp % 10 % 3;
			if (board[row][col] == 0)
			{
				grade[i]++;
			}
			// checks if there is the same symbol in that row or column and if the rest of that row or column is free
			if (IsThereAnotherTheSameInLine(board, computerSymbol, board[corners[i][0]][corners[i][1]]) &&
				IsThirdInLineEmpty(board, computerSymbol, board[corners[i][0]][corners[i][1]]))
			{
				grade[i]++;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (grade[i] > max)
		{
			max = grade[i];
			maxIndex = i;
		}
	}
	if (max > 0)
	{
		computerLastMove = corners[maxIndex][0] * 10 + corners[maxIndex][1];
		return computerLastMove;
	}
	for (int i = 0, tempCheck, tempSecondCheck; i < 4; i++)
	{
		tempCheck = IsThereAnotherTheSameInLine(board, computerSymbol, board[sides[i][0]][sides[i][1]]);
		tempSecondCheck = IsThirdInLineEmpty(board, computerSymbol, board[sides[i][0]][sides[i][1]]);
		if (board[sides[i][0]][sides[i][1]] == 0 && 
			((tempCheck == 1 && tempSecondCheck == 1) || (tempCheck == -1 && tempSecondCheck == -1)))
		{
			computerLastMove = sides[i][0] * 10 + sides[i][1];
			return computerLastMove;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				computerLastMove = i * 10 + j;
				return computerLastMove;
			}
		}
	}
	return -1;
}

int CheckIfThreatened(int board[BOARD_SIZE][BOARD_SIZE], int computerSymbol, int playerLastMove)

{
	unsigned row = (playerLastMove/10)%3;
	unsigned col = (playerLastMove%10)%3;

	//תנאים לאלכסון הראשון
	if (row == col)
	{
		if (row != 2 && (board[row][col] == board[row + 1][col + 1]))
		{
			if (row == 1 && board[row - 1][col - 1] == 0)
			{
				return (row - 1) * 10 + (col - 1);
			}
			if (row == 0 && board[row + 2][col + 2] == 0)
			{
				return (row + 2) * 10 + (col + 2);
			}
		}
		if (row != 0 && (board[row][col] == board[row - 1][col - 1]))
		{
			if (row == 1 && board[row + 1][col + 1] == 0)
			{
				return (row + 1) * 10 + (col + 1);
			}
			if (row == 2 && board[0][0] == 0)
			{
				return (0) * 10 + (0);
			}
		}
		//שני תנאים לאלכסון הראשון במצב שהמאיים זה המשבצת האמצעית
		if (col == 2 && (board[row][col] == board[row - 2][col - 2]) && board[row - 1][col - 1] == 0)
		{
			return (1) * 10 + (1);
		}
		if (col == 0 && (board[row][col] == board[row + 2][col + 2]) && board[row + 1][col + 1] == 0)
		{
			return (1) * 10 + (1);
		}
	}
	//תנאים למאונך
	if (row != 2 && (board[row][col] == board[row + 1][col]))
	{
		if (row == 1 && board[row - 1][col] == 0)
		{
			return (row - 1) * 10 + (col);
		}
		if (row == 0 && board[row + 2][col] == 0)
		{
			return (row + 2) * 10 + (col);
		}
	}
	if (row != 0 && (board[row][col] == board[row - 1][col]))
	{
		if (row == 1 && board[row + 1][col] == 0)
		{
			return (row + 1) * 10 + (col);
		}
		if (row == 2 && board[0][col] == 0)
		{
			return (row - 2) * 10 + (col);
		}
	}
	if (row == 0 && (board[row][col] == board[2][col]) && board[1][col] == 0)
	{
		return (row + 1) * 10 + (col);
	}
	if (row == 2 && (board[row][col] == board[0][col]) && board[1][col] == 0)
	{
		return (1) * 10 + (col);
	}
	//תנאים למאוזן
	if (col != 2 && (board[row][col] == board[row][col + 1]))
	{
		if (col == 1 && board[row][col - 1] == 0)
		{
			return (row) * 10 + (col - 1);
		}
		if (col == 0 && board[row][col + 2] == 0)
		{
			return (row) * 10 + (col + 2);
		}
	}
	if (col != 0 && (board[row][col] == board[row][col - 1]))
	{
		if (col == 1 && board[row][col + 1] == 0)
		{
			return (row) * 10 + (col + 1);
		}
		if (col == 2 && board[row][col - 2] == 0)
		{
			return (row) * 10 + (col - 2);
		}
	}
	if (col == 0 && (board[row][col] == board[row][2]) && board[row][1] == 0)
	{
		return (row) * 10 + (1);
	}
	if (col == 2 && (board[row][col] == board[row][0]) && board[row][1] == 0)
	{
		return (row) * 10 + (1);
	}
	//תנאים לאלכסון השני
	if (playerLastMove == 2)
	{
		if ((board[0][2] == board[1][1]) && board[2][0] == 0)
		{
			return (2) * 10 ;
		}
		if ((board[0][2] == board[2][0]) && board[1][1] == 0)
		{
			return (1) * 10 + (1);
		}
	}
	if (playerLastMove == 11)
	{
		if ((board[1][1] == board[2][0]) && board[0][2] == 0)
		{
			return (0) * 10 + (2);
		}
		if ((board[1][1] == board[0][2]) && board[2][0] == 0)
		{
			return (2) * 10 + (0);
		}
	}
	if (playerLastMove == 20)
	{
		if ((board[2][0] == board[1][1]) && board[0][2] == 0)
		{
			return (0) * 10 + (2);
		}
		if ((board[2][0] == board[0][2]) && board[1][1] == 0)
		{
			return (1) * 10 + (1);
		}
	}

	
	return -1;
}

int IsThereAnotherTheSameInLine(int board[BOARD_SIZE][BOARD_SIZE], int symbol, int place)
{
	for (int i = 0; i < 3; i++)
	{
		//אם יש בשורה שבודקים הוא יחזיר 1
		if (board[place / 10 % 3][i] == symbol)
		{
			return 1;
		}
		//אם יש בטור שבודקים הוא יחזיר -1
		if (board[i][place % 10 % 3] == symbol)
		{
			return -1;
		}
	}
	return 0;
}

int FindOtherCorner(int corner[2])
{
	int co = corner[0] * 10 + corner[1];
	if (co == 0)
	{
		return 22;
	}
	if (co == 22)
	{
		return 0;
	}if (co == 2)
	{
		return 20;
	}
	if (co == 20)
	{
		return 2;
	}
	return -1;
}

int IsThirdInLineEmpty(int board[BOARD_SIZE][BOARD_SIZE], int symbol, int place1)
{
	int rowCheck = 0;
	int colCheck = 0;

	//בודק האם אין בשורה של השחקן השני ואם כן מחזיר 1
	for (int i = 0; i < 3; i++)
	{
		if(board[place1 / 10][i] == symbol || board[place1 / 10][i] == 0)
		{
			rowCheck++;
		}
	}
	if (rowCheck == 3)
	{
		return 1;
	}
	//-בודק האם אין בטור של השחקן השני ואם כן מחזיר 1
	for (int i = 0; i < 3; i++)
	{
		if (board[i][place1 % 10] == symbol || board[i][place1 % 10] == 0)
		{
			colCheck++;
		}
	}
	if (colCheck == 3)
	{
		return -1;
	}
	return 0;
}
