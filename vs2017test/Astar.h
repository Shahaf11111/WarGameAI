#pragma once
#include "Constants.h"
#include "Node.h"
#include "CompareNodes.h"
#include "EqualNodes.h"
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class AStar {

private:
	stack<Node*> path;
	void updatePQ(priority_queue <Node, vector<Node>, CompareNodes>& pq, Node* pn);
	void checkNeighbor(Node* pcurrent, int row, int col, int trow, int tcol,
		vector <Node>& grays, vector <Node>& blacks,
		priority_queue <Node, vector<Node>, CompareNodes>& pq,
		int friendColor, int enemyColor, int maze[MSZ][MSZ]);
	Node* run(int startCol, int startRow, int endCol, int endRow,
		int friendColor, int enemyColor, int maze[MSZ][MSZ]);

public:
	void colorPath(int maze[MSZ][MSZ], int color);
	void findPath(int startCol, int startRow, int endCol, int endRow,
		int friendColor, int enemyColor, int maze[MSZ][MSZ]);
	stack<Node*> getPath();
	double getCellCost(int color, int friendColor, int enemyColor);
};

