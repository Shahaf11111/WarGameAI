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
		priority_queue <Node, vector<Node>, CompareNodes>& pq, int maze[MSZ][MSZ]);
	Node* run(int c, int r, int tc, int tr, int maze[MSZ][MSZ]);

public:
	void colorPath(int maze[MSZ][MSZ], int color);
	void findPath(int c, int r, int tc, int tr, int maze[MSZ][MSZ]);
	stack<Node*> getPath();
};

