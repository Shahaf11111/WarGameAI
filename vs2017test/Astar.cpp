#include "Astar.h"
#include <iostream>

// the Node pn is already in pq. we need to replace it with a new copy (pn) because
// the new copy has a better f 
void AStar::updatePQ(priority_queue <Node, vector<Node>, CompareNodes>& pq, Node* pn) {
	vector <Node> tmp;
	Node best = pq.top();

	while (!(best.operator==(*pn) && !pq.empty()))
	{
		pq.pop();
		tmp.push_back(best);
		best = pq.top();
	}

	// we either found pn in pq or pq got empty
	if (!pq.empty())
	{
		pq.push(*pn);
	}

	while (!tmp.empty())
	{
		best = tmp.at(tmp.size() - 1);
		pq.push(best);
		tmp.pop_back();
	}
}

double AStar::getCost(int color, int friendColor, int enemyColor) {
	if (color == WALL) {
		return 1.5;
	}
	if (color == SPACE) {
		return 0.1;
	}
	if (color == friendColor) {
		return 0.5;
	}
	if (color == enemyColor) {
		return 2.0;
	}
	return 1.0;
}

// row, col are the coordinates of a new Node,
// trow, tcol are the coordinates of a target
void AStar::checkNeighbor(Node* pcurrent, int row, int col, int trow, int tcol,
	vector <Node>& grays, vector <Node>& blacks,
	priority_queue <Node, vector<Node>, CompareNodes>& pq, int friendColor, int enemyColor,
	int maze[MSZ][MSZ]) {
	Node* pneighbor;
	vector <Node>::iterator it_gray;
	vector <Node>::iterator it_black;

	// the cost of motion to the new Node depends on its color:
	double cost = this->getCost(maze[col][row], friendColor, enemyColor);
	pneighbor = new Node(row, col, trow, tcol, pcurrent->GetG() + cost, pcurrent);
	// now add it to pq if 
	// 1. it is white
	// 2. it is gray but it improves f of this Node 
	// 3. it is "white" target
	// 4. it is "gray" target and it has f better than the previous target
	it_gray = find(grays.begin(), grays.end(), *pneighbor);
	it_black = find(blacks.begin(), blacks.end(), *pneighbor);
	// it is white if it is not black and not grays and it is not target
	if (it_gray != grays.end())// && it_black == blacks.end()) // it is gray
	{
		if (pneighbor->GetF() < (*it_gray).GetF())
		{
			updatePQ(pq, pneighbor);
			// update this neighbor in grays
			grays.erase(it_gray);
			grays.push_back(*pneighbor);
		}
	}
	else if (it_black != blacks.end())
	{
		delete(pneighbor);
	}
	else // it is white
	{
		// paint it gray
		grays.push_back(*pneighbor);
		// add it to pq
		pq.push(*pneighbor);
	}
}

void AStar::colorPath(int maze[MSZ][MSZ], int color) {
	stack<Node*> temp;
	while (!this->path.empty()) {
		Node* node = this->path.top();
		temp.push(node);
		maze[node->getRow()][node->getCol()] = color;
		this->path.pop();
	}
	swap(this->path, temp);
}

void AStar::findPath(int c, int r, int tc, int tr, 
	int friendColor, int enemyColor, int maze[MSZ][MSZ]) {
	// Clear previous queue:
	while (!this->path.empty()) {
		this->path.pop();
	}
	// Find path with AStar:
	Node* node = this->run(c, r, tc, tr, friendColor, enemyColor, maze);
	// Populate the 'path' attribute:
	while (node != nullptr && fabs(node->GetG()) > 0.01) { // only at START g=0
		this->path.push(node);
		node = node->GetParent();
	}
}

// run A* that finds the "best" path from rooms[index1] to rooms[index2]
Node* AStar::run(int c, int r, int tc, int tr, int friendColor, int enemyColor, int maze[MSZ][MSZ]) {
	bool target_found = false;
	Node* pcurrent = nullptr;
	vector <Node> grays;
	vector <Node> blacks;
	priority_queue <Node, vector<Node>, CompareNodes> pq;
	vector <Node>::iterator it_gray;
	// create start Node
	Node pn = *(new Node(r, c, tr, tc, 0, nullptr));
	// add it to pq and paint it gray
	pq.push(pn);
	grays.push_back(pn);
	while (!pq.empty() && !target_found) {
		// remove the top Node from pq
		pcurrent = new Node(pq.top());
		// check that it is not a target. If it is target than we just stop A*
		if (fabs(pcurrent->GetH()) < 0.01) { // this is target
			return pcurrent;
		}
		pq.pop();
		// and paint it black
		it_gray = find(grays.begin(), grays.end(), *pcurrent);
		// remove pcurrent from grays
		if (it_gray != grays.end()) // pcurrent has been found
			grays.erase(it_gray);
		// and add it to blacks
		blacks.push_back(*pcurrent);
		// check the neighbor Nodes of pcurrent
		// check UP
		if (pcurrent->getRow() < MSZ - 1)
		{
			checkNeighbor(pcurrent, pcurrent->getRow() + 1, pcurrent->getCol(), 
				tr, tc, grays, blacks, pq, friendColor, enemyColor, maze);
		}
		// check DOWN
		if (pcurrent->getRow() > 0)
		{
			checkNeighbor(pcurrent, pcurrent->getRow() - 1, pcurrent->getCol(),
				tr, tc, grays, blacks, pq, friendColor, enemyColor, maze);
		}
		// check left
		if (pcurrent->getCol() > 0)
		{
			checkNeighbor(pcurrent, pcurrent->getRow(), pcurrent->getCol() - 1,
				tr, tc, grays, blacks, pq, friendColor, enemyColor, maze);
		}
		// check right
		if (pcurrent->getCol() < MSZ - 1)
		{
			checkNeighbor(pcurrent, pcurrent->getRow(), pcurrent->getCol() + 1,
				tr, tc, grays, blacks, pq, friendColor, enemyColor, maze);
		}
	}
	return pcurrent;
}

stack<Node*> AStar::getPath() {
	return this->path;
}
