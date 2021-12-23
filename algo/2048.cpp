#include <bits/stdc++.h>
#define ll long long
#define ull unsigned long long

using namespace std;

const int STARTAMOUNT = 2;
const int ROWS = 4;
const int COLS = 4;

vector<vector<int>> gameBoard(ROWS, vector<int>(COLS));
vector<vector<int>> moves = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void startGame(vector<vector<int>> &gameBoard)
{
	// Start random cells up to startAmount with 2 or 4 (10% chance).
	for (int i = 0; i < STARTAMOUNT; ++i)
	{
		int r = rand()%ROWS;	
		int c = rand()%COLS;
		int check = rand()%100;
		int val = (check<=10) ? 4 : 2;
		gameBoard[r][c] = val;
	}
}
void spawnCell(vector<vector<int>> &gameBoard)
{
	// Forcibly spawns new number in empty space, this is a bad
	// way of handling this but too lazy to make it better atm.
	int r = rand()%ROWS;
	int c = rand()%COLS;
	while (gameBoard[r][c])
	{
		r = rand()%ROWS;	
		c = rand()%COLS;
	}
	int check = rand()%100;
	int val = (check<=10) ? 4 : 2;
	gameBoard[r][c] = val;
}
void displayBoard(vector<vector<int>> gameBoard)
{
	// Simple display board function.
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			cout << gameBoard[i][j] << "\t";
		}
		cout << "\n";
	}
}
bool moveBoard(vector<vector<int>> &gameBoard, char choice)
{
	vector<vector<int>> tempBoard = gameBoard;
	unordered_map<char, int> m;
	bool ok = false;
	m['w']=0, m['a']=1, m['s']=2, m['d']=3;
	// Move everything to end, matrix traversal depends on direction.
	if (choice=='d'||choice=='s')
	{
		for (int i = ROWS-1; i >= 0; --i)
		{
			for (int j = COLS-1; j >= 0; --j)
			{
				if (gameBoard[i][j])
				{
					int x = moves[m[choice]][0];
					int y = moves[m[choice]][1];
					int r = i, c = j;
					int el = gameBoard[i][j];
					int bestR=-1, bestC=-1;
					while (r>=0&&r<ROWS&&c>=0&&c<COLS)	
					{
						if (tempBoard[r][c]==0) bestR=r, bestC=c;
						r+=x, c+=y;	
					}
					if ((bestR!=-1)||(bestC!=-1))
						tempBoard[i][j] = 0, tempBoard[bestR][bestC]=el, ok = true;
				}
			}
		}

	}
	else
	{
		for (int i = 0; i < ROWS; ++i)
		{
			for (int j = 0; j < COLS; ++j)
			{
				if (gameBoard[i][j])
				{
					int x = moves[m[choice]][0];
					int y = moves[m[choice]][1];
					int r = i, c = j;
					int el = gameBoard[i][j];
					int bestR=-1, bestC=-1;
					while (r>=0&&r<ROWS&&c>=0&&c<COLS)	
					{
						if (tempBoard[r][c]==0) bestR=r, bestC=c;
						r+=x, c+=y;	
					}
					if ((bestR!=-1)||(bestC!=-1))
						tempBoard[i][j] = 0, tempBoard[bestR][bestC]=el, ok = true;						    	  
				}
			}
		}
	}
	gameBoard = tempBoard;
	return ok;
}
bool mergeRight(vector<vector<int>> &gameBoard)
{
	for (int i = ROWS-1; i >= 0; i--)
	{
		for (int j = COLS-1; j >= 1; j--)
		{
			if ((gameBoard[i][j]!=0)&&gameBoard[i][j]==gameBoard[i][j-1])
			{
				gameBoard[i][j]+=gameBoard[i][j];
				gameBoard[i][j-1]=0;
				moveBoard(gameBoard, 'd');
			}
		}
	}
}

bool mergeLeft(vector<vector<int>> &gameBoard)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 1; j < COLS; j++)
		{
			if ((gameBoard[i][j]!=0)&&gameBoard[i][j]==gameBoard[i][j-1])
			{
				gameBoard[i][j]+=gameBoard[i][j];
				gameBoard[i][j-1]=0;
				moveBoard(gameBoard, 'a');
			}
		}
	}
}
bool mergeUp(vector<vector<int>> &gameBoard)
{
	for (int i = 0; i < ROWS-1; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if ((gameBoard[i][j]!=0)&&gameBoard[i][j]==gameBoard[i+1][j])
			{
				gameBoard[i][j]+=gameBoard[i][j];
				gameBoard[i+1][j]=0;
				moveBoard(gameBoard, 'w');
			}
		}
	}
}
bool mergeDown(vector<vector<int>> &gameBoard)
{
	for (int i = ROWS-1; i >= 1; i--)
	{
		for (int j = COLS-1; j >= 0; j--)
		{
			if ((gameBoard[i][j]!=0)&&gameBoard[i][j]==gameBoard[i-1][j])
			{
				gameBoard[i][j]+=gameBoard[i][j];
				gameBoard[i-1][j]=0;
				moveBoard(gameBoard, 's');
			}
		}
	}
}

void mergeControl(vector<vector<int>> &gameBoard, char choice)
{
	// Handles merging of cells based on direction, could definitely make this neater...
	switch (choice)
	{
		case 'w':
			mergeUp(gameBoard);
			break;
		case 'a':
			mergeLeft(gameBoard);
			break;
		case 's':
			mergeDown(gameBoard);
			break;
		case 'd':
			mergeRight(gameBoard);
			break;
		default:
			break;
	}
}

int main()
{
	srand(time(NULL));
	system("cls");
	startGame(gameBoard);
	displayBoard(gameBoard);
	char choice;
	bool ok = false;
	while (true)
	{
		ok = false;
		cin >> choice;
		if (moveBoard(gameBoard, choice))
			ok = true;
		mergeControl(gameBoard, choice);
		if (ok)
			spawnCell(gameBoard);
		system("cls");
		displayBoard(gameBoard);
	}
	return 0;
}
