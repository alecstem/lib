#include <bits/stdc++.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

using ll = long long;
using ld = long double;
using ull = unsigned long long;

using namespace std;

class Point
{
public:
    int x;
    int y;
    int id;

    Point()
    {
        x = 0;
        y = 0;
        id = 0;
    }
};

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;
HWND myconsole = GetConsoleWindow();
HDC dc = GetDC(myconsole);

int XSIZE = 100;
int YSIZE = 49;
int NUM_POINTS = 20;

vector<char> v = {char(176),char(177),char(178),char(219)};

vector<Point> generatePoints()
{
    vector<Point> points;

    for (int i = 0; i < NUM_POINTS; i++)
    {
        Point temp = Point();

        temp.x = rand()%XSIZE;
        temp.y = rand()%YSIZE;
        temp.id = i;

        points.push_back(temp);
    }

    return points;
}

void draw(vector<Point> points)
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        FillConsoleOutputCharacter(hOutput, char(219), 1, {points[i].x, points[i].y}, &dwWritten);
    }
}

void edist(vector<Point> points)
{
    for (int i = 0; i < XSIZE; i++)
    {
        for (int j = 0; j < YSIZE; j++)
        {
            float shortestDistance = FLT_MAX;
            int shortestPoint;
            for (int k = 0; k < NUM_POINTS; k++)
            {
                float x2 = points[k].x;
                float x1 = i;
                float y2 = points[k].y;
                float y1 = j;

                float dist = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

                if (dist < shortestDistance)
                {
                    shortestDistance = dist;
                    shortestPoint = points[k].id;
                }
            }
            FillConsoleOutputCharacter(hOutput, v[shortestPoint%v.size()], 1, {i, j}, &dwWritten);
        }
    }
}

void walkPoints(vector<Point> &points)
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        int my = rand()%4;
        int mx = rand()%4;
        int sign = rand()%2;

        if (sign==1)
        {
            if (points[i].y+my < YSIZE && points[i].x+mx < XSIZE &&
                points[i].y+my >= 0 && points[i].x+mx >= 0)
            {
                points[i].y+=my;
                points[i].x+=mx;
            }
        } else
        {
            if (points[i].y-my < YSIZE && points[i].x-mx < XSIZE &&
                points[i].y-my >= 0 && points[i].x-mx >= 0)
            {
                points[i].y-=my;
                points[i].x-=mx;
            }
        }

    }
}

int main()
{
    srand(time(NULL));

    vector<Point> points = generatePoints();

    while(true)
    {
        //draw(points);

        edist(points);

        walkPoints(points);
    }
}


