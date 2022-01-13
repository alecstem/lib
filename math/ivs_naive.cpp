#include <bits/stdc++.h>
#define ll long long
#define ull unsigned long long

using namespace std;

// Accepts graph input as adj. list
vector<vector<int>> graph = {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},{6,7}}; 
const int numVertices = 8; 
int indepNum = INT_MAX; // Finds actual independence num. later

void generateCombinations(vector<int> tmp, int len, vector<int> cur, int start, set<vector<int>> &curSet)
{
	if (cur.size()==len)
	{
		sort(cur.begin(), cur.end());
		curSet.insert(cur);
		return;
	}
	for (int i = start; i < tmp.size(); ++i)
	{
		cur.push_back(tmp[i]);
		generateCombinations(tmp, len, cur, i+1, curSet);
		cur.pop_back();
	}
}
void display(unordered_map<int, set<vector<int>>> idP)
{
	for (int i = 0; i <= indepNum; ++i)
	{
		cout << i << ": " << endl;
		for (auto &s : idP[i])
		{
			for (auto &e : s)		
				cout << e << " ";
			cout << endl;
		}
		cout << endl;		
	}
	string poly = "";
	for (int i = 0; i <= indepNum; ++i)
	{
		poly+=to_string(idP[i].size()), poly+=" ";;
		
	}
	poly.pop_back();
	cout << poly << endl;
	cout << "Independence number: " << indepNum;
}
int main()
{
	unordered_map<int, vector<int>> adj;
	unordered_map<int, set<vector<int>>> idP;
	for (auto &e: graph)
		adj[e[0]].push_back(e[1]), adj[e[1]].push_back(e[0]);
	int cur = 0;
	for (cur = 0; cur <= indepNum; ++cur)
	{
		set<vector<int>> curSet;	
		vector<int> permutations;
		for (int i = 0; i < numVertices; ++i) 
			permutations.push_back(i);
		do
		{
			vector<int> tmp;
			unordered_map<int, bool> hasBeenVisited;
			tmp.push_back(permutations[0]);
			hasBeenVisited[permutations[0]]=true;
			for (auto &e: adj[permutations[0]])
				hasBeenVisited[e]=true;
			for (int i = 1; i < permutations.size(); ++i)
				if (!hasBeenVisited[permutations[i]])
				{
					tmp.push_back(permutations[i]);
					for (auto &e: adj[permutations[i]]) hasBeenVisited[e]=true;
				}
			generateCombinations(tmp, cur, {}, 0, curSet);
		} while (next_permutation(permutations.begin(), permutations.end()));
		if (cur&&!curSet.size()) break;
		idP[cur]=curSet;
	} 
	indepNum = (cur-1);
	display(idP); // Displays every IVS & independence polynomial
	return 0;
}
