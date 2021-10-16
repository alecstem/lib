#include <bits/stdc++.h>

using namespace std;

struct Ant
{
	int x,y,d;
	Ant(int x, int y, int d) : x(x), y(y), d(d) {}

};
// Directions (N, E, S, W)
vector<vector<int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void updateAnt(Ant &ant, vector<vector<int>> &board)
{
	// Apply rules
	if (board[ant.x][ant.y]==1)
		board[ant.x][ant.y] = 0, ant.d = (ant.d-1%4+4)%4;
	else
		board[ant.x][ant.y] = 1, ant.d = (ant.d+1%4+4)%4;
	// Move ant
	ant.x+=dirs[ant.d][0], ant.y+=dirs[ant.d][1];
}

int main()
{
	// Init board
	//
	// 0 = white, 1 = black
	int boardX = 75, boardY = 100;
	vector<vector<int>> board(boardX, vector<int>(boardY, 0));

	// Create ant
	Ant ant(boardX/2, boardY/2, 0);

	// While ant is in bounds...
	while (ant.x >= 0 && ant.x < boardX && ant.y >= 0 && ant.y < boardY)
	{
		updateAnt(ant, board);
	}

	// Print board
	for (auto &e: board)
	{
		for (auto &x: e)
			if (x==0) cout << " ";
			else cout << "#";
		cout << "\n";
	}

	return 0;
}
