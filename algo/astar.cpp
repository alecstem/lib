#include <bits/stdc++.h>
#include <Windows.h>

using namespace std;

// Screen size constants
const int ROWS = 20;
const int COLS = 20;

// Ending node coordinates
int TARGETR = 0;
int TARGETC = 0;

// Starting node coordinates
int STARTR = 0;
int STARTC = 6;

struct Node
{
	// f(n) = g(n)+h(n)
	float f,g,h;
	short s,e;
	int r,c,parentR,parentC;
	Node(float f, float g, float h,
	     short s, short e) : f(f), g(g), h(h), s(s), e(e) {}
};

float euclidian(Node node, int endr, int endc)
{
	// Heuristic used for h(n) will be euclidian distance
	float val = 0.0f;
	val = sqrt(((endr-node.r)*(endr-node.r))+((endc-node.c)*(endc-node.c)));
	return val;
}

void gotoxy(short x, short y)
{
	// Goto function for console graphics output
	static HANDLE h = NULL;
  	if(!h)
    		h = GetStdHandle(STD_OUTPUT_HANDLE);
  	COORD c = { x, y };
  	SetConsoleCursorPosition(h,c);
}

void drawMap(vector<vector<Node>> screen, vector<Node> finalPath, vector<vector<bool>> obs)
{
	// Hacked together display function.
	system("cls");
	for (int i = 0; i < screen.size(); ++i)
	{
		for (int j = 0; j < screen[i].size(); ++j)
		{
			if (obs[i][j])
				cout << char(219);
			else if (screen[i][j].s==1)
				cout << "S";
			else if (screen[i][j].e==1)
				cout << "E";
			else
				cout << " ";
		}
		cout << endl;
	}
	for (auto &e: finalPath)
		gotoxy(e.c, e.r), cout << char(176);
}

void drawMapNoPath(vector<vector<Node>> screen, vector<vector<bool>> obs)
{
	// Hacked together display function.
	system("cls");
	for (int i = 0; i < screen.size(); ++i)
	{
		for (int j = 0; j < screen[i].size(); ++j)
		{
			if (obs[i][j])
				cout << char(219);
			else if (screen[i][j].s==1)
				cout << "S";
			else if (screen[i][j].e==1)
				cout << "E";
			else
				cout << " ";
		}
		cout << endl;
	}
}

void generateObstacles(vector<vector<Node>> screen, vector<vector<bool>> &obs)
{
	int randomNumber;
	for (int i = 0; i < screen.size(); ++i)
	{
		for (int j = 0; j < screen[i].size(); ++j)
		{
			randomNumber = rand()%10;
			if (randomNumber < 4) obs[i][j]=1;
		}
	}

}

int main()
{
	srand(time(NULL));

	bool pathFound = false;

	// Nodes will all have max values to start out with.
	Node node(FLT_MAX,FLT_MAX,FLT_MAX, 0, 0);

	// 2d screen vector, direction vector (with diagonal)
	vector<vector<Node>> screen(ROWS, vector<Node>(COLS, node));
	//vector<vector<int>> dirs = {{1,0}, {0,1}, {-1,0},
	//			    {0,-1}, {1,1}, {-1,1},
	//			    {-1,-1}, {1, -1}};
	// Uncomment for non-diagonal
	vector<vector<int>> dirs = {{1,0},{0,1},{-1,0},{0,-1}};

	// This tells which nodes are start/end nodes.

	// Overloading compare operator for heap to make it compare with f(n)
	auto comp = []( Node  &a, Node &b ) { return a.f > b.f; };

	// Heap = openSet, normal vectors used for closedSet and obstacles.
	priority_queue<Node, vector<Node>, decltype(comp)> open(comp);
	vector<vector<bool>> closed(ROWS, vector<bool>(COLS, 0));
	vector<vector<bool>> obs(ROWS, vector<bool>(COLS, 0));

	// Initiate all nodes with their proper X and Y values.
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
		{
			screen[i][j].r=i;
			screen[i][j].c=j;
		}

	generateObstacles(screen, obs);
	drawMapNoPath(screen, obs);
	gotoxy(0, 22);
	cout << "Enter start node:\n";
	cin >> STARTR >> STARTC;
	cout << "Enter end node:\n";
	cin >> TARGETR >> TARGETC;


	screen[STARTR][STARTC].s=1;
	screen[TARGETR][TARGETC].e=1;

	// Initiate start node values to 0, parent to itself, push into heap
	screen[STARTR][STARTC].f=0.0f;
	screen[STARTR][STARTC].g=0.0f;
	screen[STARTR][STARTC].h=0.0f;
	screen[STARTR][STARTC].parentR=STARTR;
	screen[STARTR][STARTC].parentC=STARTC;
	open.push(screen[STARTR][STARTC]);

	while (!open.empty())
	{
		// Get top node of heap, add it to closed set.
		Node current = open.top();
		open.pop();
		closed[current.r][current.c]=1;

		// If current node is the end node, we can break
		if (current.r==TARGETR&&current.c==TARGETC)
		{
			pathFound = true;
			break;
		}

		// For each neighbor of the current node...
		for (auto &e: dirs)
		{
			int x = current.r+e[0], y = current.c+e[1];
			// If neighbor is traversable,
			// not an obstacle, and not in closed set...
			if (x>=0&&x<ROWS&&y>=0&&y<COLS&&!obs[x][y]&&!closed[x][y])
			{
				// g(n) = edge weight so far to reach current node
				float newG = current.g+(sqrt(
					     (current.r-x)*(current.r-x))
					     +(current.c-y)*(current.c-y));
				// Call heurstic function to generate h(n)
				float newH = euclidian(screen[x][y], TARGETR, TARGETC);

				// If h(n)=0, A* downgrades to Dijkstra's
				// float newH = 0.0f;

				// f(n) = g(n) + h(n)
				float newF = newG + newH;

				// If first visit or neighbor's previous f(n) is greater..
				if (screen[x][y].f==FLT_MAX||
				    screen[x][y].f > newF)
				{
					// Update all values to new values, change parent
					screen[x][y].f=newF;
					screen[x][y].g=newG;
					screen[x][y].h=newH;
					screen[x][y].parentR = current.r;
					screen[x][y].parentC = current.c;
					// Enqueue each neighbor into heap
					open.push(screen[x][y]);
				}
			}
		}
	}

	if (pathFound)
	{
		// Rebuild path
		stack<Node> path;
		vector<Node> finalPath;
		int rr = TARGETR, cc = TARGETC;
		// While the parent of the current node is not equal to the starting node...
		while (!(screen[rr][cc].parentR==STARTR&&screen[rr][cc].parentC==STARTC))
		{
			// Push node into stack, then push the parent node into the stack...
			path.push(screen[rr][cc]);
			int tempR = screen[rr][cc].parentR;
			int tempC = screen[rr][cc].parentC;
			rr = tempR;
			cc = tempC;
		}
		path.push(screen[rr][cc]);
		// While the stack is not empty, pop each node off and into a vector.
		// Using stack's LIFO principle, this inverts the path from E->S (due to
		// retracing paradigm in previous step) to S->E
		while (!path.empty())
		{
			Node top = path.top();
			path.pop();
			finalPath.push_back(top);
		}
		drawMap(screen, finalPath, obs);
		gotoxy(TARGETC, TARGETR);
		cout << "E";
	} else
	{
		cout << "Path not found!";
	}


	Sleep(1000000);

	return 0;
}
