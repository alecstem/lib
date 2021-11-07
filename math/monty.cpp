#include <bits/stdc++.h>
#include <Windows.h>
#define ll long long
#define ull unsigned long long

using namespace std;

struct Doors
{
	unordered_map<int, char> m;
	int carNumber;
	vector<int> goatNumbers;
	Doors()
	{
		carNumber = rand()%3;
		m[0] = 'S', m[1] = 'S', m[2] = 'S';
		m[carNumber] = 'C';
		for (int i = 0; i < 3; ++i)
		{
			if (i!=carNumber)
				goatNumbers.push_back(i);
		}
	}
};

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;
HWND myconsole = GetConsoleWindow();
HDC dc = GetDC(myconsole);

bool simulateGame(Doors doors, bool isSwitch)
{
	// Simulate choices for contestant and host
	int hostChoice, contestantChoice = rand()%3; 	

	if (contestantChoice==doors.goatNumbers[0]) hostChoice = doors.goatNumbers[1];
	else if (contestantChoice==doors.goatNumbers[1]) hostChoice = doors.goatNumbers[0];
	else hostChoice = (rand() > RAND_MAX/2) ? doors.goatNumbers[0] : doors.goatNumbers[1];	

	if (isSwitch)
	{
		// If we're switching, then we must choose the other option that ISN'T the host's choice.
		for (int i = 0; i < 3; ++i)
		       if (i!=contestantChoice&&i!=hostChoice)
		       {
			       contestantChoice=i;
			       break;
		       }	       	
	}	
	// Return if contestant picked the car door or not.
	
	// Debug: check choices
	/*
	 cout << "Host choice: " << hostChoice << "\nContestant choice: " << contestantChoice
	 << "\nCar: " << doors.carNumber << endl;
	*/		
	return (doors.m[contestantChoice]=='C') ? true : false;
}

int main()
{
	srand(time(NULL));
	double numberOfSamples = 1, winningGamesNoSwitching = 0, winningGamesSwitching = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
	while (numberOfSamples<500)
	{
		for (int i = 0; i < numberOfSamples; ++i)
		{
			Doors doors = Doors();
			if (simulateGame(doors, false))
				winningGamesNoSwitching++;	
			Doors doors2 = Doors();
			if (simulateGame(doors2, true))
				winningGamesSwitching++;
		}
		double noSwitchRate = winningGamesNoSwitching/numberOfSamples;
		double switchRate = winningGamesSwitching/numberOfSamples;
		cout << "Samples: " << numberOfSamples << "\n";
		cout << "Success rate without switching: " << noSwitchRate*100.0 << "\nSuccess rate with switching: " <<
			switchRate*100.0 << "\n\n";
		winningGamesNoSwitching = 0, winningGamesSwitching = 0;
		numberOfSamples++;
		Sleep(100);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
	}
	return 0;
}
