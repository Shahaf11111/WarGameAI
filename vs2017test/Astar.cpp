#include "Astar.h"
#include <iostream>

// the Node pn is already in pq. we need to replace it with a new copy (pn) because
// the new copy has a better f 
void AStar::updatePQ(priority_queue <Node, vector<Node>, CompareNodes>& pq, Node* node) {
	vector<Node> tmp;
	Node best = pq.top();
	while (!(best.operator==(*node) && !pq.empty())) {
		pq.pop();
		tmp.push_back(best);
		best = pq.top();
	}
	// we either found pn in pq or pq got empty
	if (!pq.empty()) {
		pq.push(*node);
	}
	while (!tmp.empty()) {
		best = tmp.at(tmp.size() - 1);
		pq.push(best);
		tmp.pop_back();
	}
}

double AStar::getCellCost(int color, int friendColor, int enemyColor) {
	if (color == WALL || color == OBSTACLE) {
		return 1.5;
	}
	if (color == SPACE) {
		return 0.1;
	}
	if (color == friendColor) {
		return 0.2;
	}
	if (color == enemyColor) {
		return 3.0;
	}
	return 1.0;
}

// row, col are the coordinates of a new Node,
// trow, tcol are the coordinates of a target
void AStar::checkNeighbor(Node* pcurrent, int row, int col, int trow, int tcol,
	vector <Node>& grays, vector <Node>& blacks,
	priority_queue <Node, vector<Node>, CompareNodes>& pq, int friendColor, int enemyColor,
	int maze[MSZ][MSZ]) {
	vector <Node>::iterator grayIterator;
	vector <Node>::iterator blackIterator;
	// the cost of motion to the new Node depends on its color:
	double cost = this->getCellCost(maze[row][col], friendColor, enemyColor);
	Node* neighbor = new Node(row, col, trow, tcol, pcurrent->getG() + cost, pcurrent);
	// now add it to pq if 
	// 1. it is white
	// 2. it is gray but it improves f of this Node 
	// 3. it is "white" target
	// 4. it is "gray" target and it has f better than the previous target
	grayIterator = find(grays.begin(), grays.end(), *neighbor);
	blackIterator = find(blacks.begin(), blacks.end(), *neighbor);
	// it is white if it is not black and not grays and it is not target
	if (grayIterator != grays.end()) { // && blackIterator == blacks.end()) // it is gray
		if (neighbor->getF() < (*grayIterator).getF()) {
			updatePQ(pq, neighbor);
			// update this neighbor in grays
			grays.erase(grayIterator);
			grays.push_back(*neighbor);
		}
	} else if (blackIterator != blacks.end()) {
		delete(neighbor);
	} else { // it is white
		// paint it gray
		grays.push_back(*neighbor);
		// add it to pq
		pq.push(*neighbor);
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

void AStar::findPath(int startCol, int startRow, int endCol, int endRow, 
	int friendColor, int enemyColor, int maze[MSZ][MSZ]) {
	// Clear previous queue:
	while (!this->path.empty()) {
		this->path.pop();
	}
	// Find path with AStar:
	Node* result = this->run(startCol, startRow, endCol, endRow, friendColor, enemyColor, maze);
	// Populate the 'path' attribute:
	while (result != nullptr && fabs(result->getG()) > 0.01) { // only at START the 'g' is 0
		this->path.push(result);
		result = result->getParent();
	}
}

Node* AStar::run(int startCol, int startRow, int endCol, int endRow, 
	int friendColor, int enemyColor, int maze[MSZ][MSZ]) {
	bool targetFound = false;
	Node* current = nullptr;
	vector <Node> grays;
	vector <Node> blacks;
	priority_queue <Node, vector<Node>, CompareNodes> priorityQueue;
	vector <Node>::iterator grayIterator;
	// create start Node
	Node node = *(new Node(startRow, startCol, endRow, endCol, 0, nullptr));
	// add it to pq and paint it gray
	priorityQueue.push(node);
	grays.push_back(node);
	while (!priorityQueue.empty() && !targetFound) {
		// remove the top Node from pq
		current = new Node(priorityQueue.top());
		// check that it is not a target. If it is target than we just stop A*
		if (fabs(current->getH()) < 0.01) { // this is target
			return current;
		}
		priorityQueue.pop();
		// and paint it black
		grayIterator = find(grays.begin(), grays.end(), *current);
		// remove pcurrent from grays
		if (grayIterator != grays.end()) { // pcurrent has been found
			grays.erase(grayIterator);
		}
		// and add it to blacks
		blacks.push_back(*current);
		// check the neighbor Nodes of pcurrent
		// check up
		if (current->getRow() < MSZ - 1) {
			checkNeighbor(current, current->getRow() + 1, current->getCol(), 
				endRow, endCol, grays, blacks, priorityQueue, friendColor, enemyColor, maze);
		}
		// check down
		if (current->getRow() > 0) {
			checkNeighbor(current, current->getRow() - 1, current->getCol(),
				endRow, endCol, grays, blacks, priorityQueue, friendColor, enemyColor, maze);
		}
		// check left
		if (current->getCol() > 0) {
			checkNeighbor(current, current->getRow(), current->getCol() - 1,
				endRow, endCol, grays, blacks, priorityQueue, friendColor, enemyColor, maze);
		}
		// check right
		if (current->getCol() < MSZ - 1) {
			checkNeighbor(current, current->getRow(), current->getCol() + 1,
				endRow, endCol, grays, blacks, priorityQueue, friendColor, enemyColor, maze);
		}
	}
	return current;
}

stack<Node*> AStar::getPath() {
	return this->path;
}
