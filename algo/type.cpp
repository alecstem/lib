#include <bits/stdc++.h>
#include <conio.h>
#include <chrono>
using namespace std;

struct Scores
{
	int gwpm, wpm, cpm;
	Scores(int gwpm, int wpm, int cpm) : gwpm(gwpm), wpm(wpm), cpm(cpm) {}
};

struct Config
{
	std::unordered_map<std::string, int> configMap;
	Config()
	{
		// This constructor is used for defaults
		// in case the config file is damaged
		configMap["words_per_test"]=10;
		configMap["words_per_line"]=5;
		configMap["sudden_death"]=0;
	}	
};

ostream& operator<<(ostream &os, const Scores &scores)
{
	os << "\n\nWPM: " << scores.gwpm
	<< " " << "CPM: " << scores.cpm << "\n\n";
	return os;
}

void test(vector<string> word_list2, Scores &scores, string s, Config config)
{
	char ch;
	bool indented = false, hasTyped = false;
	int i = 0, wordSize = 0, letterCount = 0, wordPos = 0, extra = 0;
	auto start = chrono::steady_clock::now();

	wordSize = word_list2[0].length();

   	while(true)
    	{
        	if(kbhit())
        	{
			if (!hasTyped)
			{
				start = chrono::steady_clock::now();
				hasTyped = true;
			}
			ch = getch();
		    	if (ch==s[i])
		    	{
				i++;
			    	letterCount++;
			    	cout << ch;
			   	if (letterCount==wordSize)
			    	{
					letterCount = 0;
				    	wordSize = word_list2[++wordPos].length()+1;
			    	}
				// Controls line length
			    	if (wordPos!=0&&wordPos%config.
				    configMap["words_per_line"]==0
				    &&!indented) cout << "\n", indented=true;
				else if (wordPos%config.
					 configMap["words_per_line"]!=0) indented = false;
		   	} else if (ch!=8&&ch!=s[i])
			{
				if (config.configMap["sudden_death"])
				{
					cout << "\nYou typed an incorrect key!\n";
					break;
				}
				extra++;
			}
		    	if (ch==8&&letterCount>0)
		    	{
				i--;
			    	letterCount--;
			    	cout << "\b \b";
		   	}
		    if (i==s.length()) break;
        	}
    	}
	// Stop timer, calculate resultant scores (if successful)
	auto end = chrono::steady_clock::now();

	if (i==s.length())
	{
		float timeSeconds =
		(chrono::duration_cast<chrono::seconds>(end-start).count());
		float gwpm = (((float)i)*(60.0f/(float)timeSeconds)/5.0f);
		float wpm = (((float)i+extra)*(60.0f/(float)timeSeconds)/5.0f);
		float cpm = round(gwpm * 5);

		// Update scores struct
		scores.gwpm = round(gwpm);
		scores.wpm = round(wpm);
		scores.cpm = round(cpm);
	} else
	{
		// If failed due to sudden death, add new line for menu
		cout << "\n";
	}
	return;
}

Scores word(Config config, Scores &scores)
{
	// Define variables
	string s = "", current = "";
	int amtWords = config.configMap["words_per_test"];

 	vector<string> word_list, word_list2(amtWords);

	// Read words from file
    	ifstream words("words.txt");
    	do
	{
        	getline(words,current);
        	word_list.push_back(current);
    	} while(!words.eof());
   	words.close();
	cout << " ";

	// Select words for test
	for (int i = 0; i < amtWords; ++i)
	{
		string newWord = word_list[rand()%word_list.size()];
		word_list2[i] = newWord;
		cout << newWord << " ";
		if ((i+1)%config.configMap["words_per_line"]==0) cout << "\n ";
		s+=newWord, s+=" ";
	}
	s.pop_back();
	cout << "\n ";

	// Start test
	test(word_list2, scores, s, config);

	// Clear word arrays used for test
	word_list.clear();
	word_list2.clear();

	return scores;
}

Scores quote(Config config, Scores &scores)
{
	// Define variables
	int numLines = 0, randomLine = 0, i = 0;
	string s = "";
	vector<string> quoteWords;
	string token, quote="";
    	ifstream quotes("quotes.txt");

	// Find number of lines in quote.txt
	while (getline(quotes, quote))
		++numLines;

	// Select random line
	randomLine = rand()%(numLines+1);
	if (!randomLine) randomLine++;
	numLines = 0;
	quotes.clear(), quotes.seekg(0);
	cout << " \"";

	// Tokenize selected quote into words
	while (getline(quotes, quote))
	{
		numLines++;
		if (numLines==randomLine)
		{
			stringstream ss(quote);
			while (getline(ss,token,' '))
			{
				i++;
				quoteWords.push_back(token);
				cout << token << " ";
				if (i%config.
				    configMap["words_per_line"]==0) cout << "\n ";
				s+=token, s+=" ";
			}
		}
	}
	s.pop_back();
	cout << "\b\"\n\n  ";

	// Start test
	test(quoteWords, scores, s, config);

	// Clear word arrays used for test
	quoteWords.clear();

	return scores;
}

void readConfig(Config &config)
{
	string current = "";
	ifstream configFile("config.txt");
	do
	{
		getline(configFile, current);
		current.erase(remove_if(current.begin(), current.end(), ::isspace),
                              current.end());
		if (current[0]=='#'||current.empty())
			continue;
		auto delimiter = current.find("=");
		auto setting = current.substr(0, delimiter);
		int val = stoi( current.substr(delimiter+1) );
		config.configMap[setting]=val;
	} while (!configFile.eof());
	return;
}

char selectMode(Config &config)
{
	char modeSelection;
	string modes = "\n1. (q)uote\n2. (w)ords\n3. (t)imed\n4. (z)en\n\n5. (s)udden death (toggle)\n\n";
	cout << modes;
	cin >> modeSelection;

	// Toggles sudden death
	if (modeSelection=='s')
	{
		if (!config.configMap["sudden_death"])
			config.configMap["sudden_death"]=1;
		else
			config.configMap["sudden_death"]=0;
		cout << "\nToggled sudden death to " <<
			 config.configMap["sudden_death"];
		modeSelection = selectMode(config);
	}
	return modeSelection;
}

Scores startTest(char &mode, Scores &scores, Config &config)
{
	switch (mode)
	{
		case 'q':
			scores = quote(config, scores);
			break;
		case 'w':
			scores = word(config, scores);
			break;
		case 't':
			//scores = timed(config, scores);
			break;
		case 'z':
			//scores = zen(config, scores);
			break;
		default:
			cout << "\nInvalid mode!";
			mode = selectMode(config);
			break;
	}
	return scores;
}

int main()
{
	srand(time(NULL));

	// Read config file
	Config config;
	readConfig(config);

	Scores scores(0,0,0);

	string options = "(s)tart, (m)ode, (c)onfig, (e)xit\n";
	char mode = 'w';

	bool quit = false;

	while (!quit)
	{
		char choice;
		int configChoice;
		cout << options;
		cin >> choice;
		switch (choice)
		{
			case 's':
				startTest(mode, scores, config);
				break;
			case 'm':
				mode = selectMode(config);
				break;
			case 'e':
				quit = true;
				break;
			case 'c':
				// Print config values
				cout << "\n";
				for (auto &e: config.configMap)
					cout << e.first << ": " << e.second << "\n";
				cout << "\n";
				break;
			default:
				break;
		}
		// Only print scores if test taken, not quitting, not requesting config
		if (scores.gwpm&&!quit&&choice!='c')
			cout << scores;
		// Reset WPM score.
		scores.gwpm = 0, scores.cpm = 0;
	}
	return 0;
}
