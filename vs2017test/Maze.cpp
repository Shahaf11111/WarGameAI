#include "Maze.h"
#include "Grenade.h"

using namespace std;

Maze::Maze() {
	this->initMaze();
	this->initRooms();
	this->digTunnels();
	this->initSecurityMap();
}

// init the whole space with WALLS
void Maze::initMaze() {
	int i, j, obstacleCounter = 0;
	for (i = 0; i < MSZ; i++) {
		for (j = 0; j < MSZ; j++) {
			this->maze[i][j] = WALL;
		}
	}
	for (i = 0; i < MSZ; i++) {
		j = 0;
		this->maze[i][j] = WALL;
		j = MSZ - 1;
		this->maze[i][j] = WALL;
	}
	for (j = 0; j < MSZ; j++) {
		i = 0;
		this->maze[i][j] = WALL;
		i = MSZ - 1;
		this->maze[i][j] = WALL;
	}
}

void Maze::initRooms() {
	int i, j;
	int w, h, row, col;
	int min = 10;
	bool overlap;
	cout << "Initializing the rooms..." << endl;
	for (i = 0; i < NUM_ROOMS; i++) {
		do {
			overlap = false;
			w = min + rand() % NUM_ROOMS;
			h = min + rand() % NUM_ROOMS;
			col = 2 + w / 2 + rand() % (MSZ - w - 4);
			row = 2 + h / 2 + rand() % (MSZ - h - 4);
			// check if the above definitions don't make a room to overlap with any other room
			for (j = 0; j < i && !overlap; j++)
				if (rooms[j].isOverlapping(w, h, row, col))
					overlap = true;
		} while (overlap);

		rooms[i].setWidth(w);
		rooms[i].setHeight(h);
		rooms[i].setCenterX(col);
		rooms[i].setCenterY(row);
		rooms[i].fillMaze(this->maze);
	}
	cout << "All rooms have been created!" << endl;
}

void Maze::runAStar(int c, int r, int tc, int tr, int friendColor, int enemyColor) {
	this->aStar.findPath(c, r, tc, tr, friendColor, enemyColor, this->maze);
}

// run A* that finds the "best" path from rooms[index1] to rooms[index2]
void Maze::digTunnel(int index1, int index2) {
	int r, c, tr, tc;
	// create start Node
	r = rooms[index1].getCenterX();
	c = rooms[index1].getCenterY();
	tr = rooms[index2].getCenterX();
	tc = rooms[index2].getCenterY();
	this->aStar.findPath(c, r, tc, tr, SPACE, SPACE, this->maze);
	this->aStar.colorPath(this->maze, SPACE);
}

void Maze::digTunnels() {
	cout << "Initializing the tunnels..." << endl;
	for (int i = 0; i < NUM_ROOMS; i++) {
		for (int j = i + 1; j < NUM_ROOMS; j++) {
			this->digTunnel(i, j); // A*
			//cout << "Tunnel from " << i << " to " << j << " is ready!\n";
		}
	}
	cout << "All tunnels have been created!" << endl;
}

void Maze::initSecurityMap() {
	cout << "Initializing the security map..." << endl;
	int securityMap[MSZ][MSZ] = { 0 };
	for (int i = 0; i < MSZ; i++) {
		for (int j = 0; j < MSZ; j++) {
			if (this->maze[i][j] != WALL && this->maze[i][j] != OBSTACLE) {
				Grenade* grenade = new Grenade(j, i, j, i, WALL, SPACE);
				for (auto cell : grenade->updateSecurityMap(this->maze)) {
					securityMap[cell[1]][cell[0]] += 1;
				}
			}
		}
	}
	this->initSafeCells(securityMap);
	cout << "Security map have been created!" << endl;
}

int* Maze::getSafeCellWith(int colToExclude, int rowToExclude, int color) {
	int max = 0;
	int* safeCell = nullptr;
	for (int i = 0; i < NUM_ROOMS; i++) {
		if (!this->rooms[i].contains(colToExclude, rowToExclude)) {
			int count = this->rooms[i].getAmountOf(this->maze, colToExclude, rowToExclude, color);
			if (max < count) {
				safeCell = this->rooms[i].getSafeCell();
			}
		}
	}
	return safeCell;
}

void Maze::initSafeCells(int securityMap[MSZ][MSZ]) {
	for (int i = 0; i < NUM_ROOMS; i++) {
		this->rooms[i].initSafeCells(securityMap);
	}
}

void Maze::drawMe() {
	int i, j;
	double sx, sy; // cell size
	double x, y;

	sx = 2.0 / MSZ;
	sy = 2.0 / MSZ;
	int factor = MSZ * MSZ;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++) {
			switch (this->maze[i][j]) {
			case SPACE:
				glColor3d(1, 1, 1);   // white
				break;
			case WALL:
				glColor3d(0.4, 0.0, 0.2);   // dark-red
				break;
			case START:
				glColor3d(0, 1, 1);   // cyan
				break;
			case TARGET:
				glColor3d(1, 0, 0);   // red
				break;
			case GRAY:
				glColor3d(0, 0.6, 0);   // dark green
				break;
			case BLACK:
				glColor3d(0.6, 1, 0.8);   // green
				break;
			case PATH:
				glColor3d(0.8, 0.4, 1);   // magenta
				break;
			case OBSTACLE:
				glColor3d(0.4, 0.0, 0.2);   // dark-red
				break;
			default:
				glColor3d(1, 1, 1);   // white
			}
			// draw square maze[i][j]
			x = 2 * (j / (double)MSZ) - 1;
			y = 2 * (i / (double)MSZ) - 1;

			glBegin(GL_POLYGON);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();
			glColor3d(0, 0, 0);
		}
}

bool Maze::isInsideTunnel(int col, int row) {
	// Check nearest cells:
	for (int i = -1; i <= 1; i++) { 
		for (int j = -1; j <= 1; j++) {
			int aCol = min(max(0, col + i), MSZ - 1);
			int aRow = min(max(0, row + i), MSZ - 1);
			if (maze[aRow][aCol] == WALL) {
				return true;
			}
		}
	}
	return false;
}

bool Maze::isSafe(int col, int row) {
	return !this->isInsideTunnel(col, row);
}

int Maze::get(int col, int row) {
	return this->maze[row][col];
}

void Maze::set(int col, int row, int color) {
	this->maze[row][col] = color;
}

stack<Node*> Maze::getPath() {
	return this->aStar.getPath();
}

int Maze::getAmountOfColorInRoom(int col, int row, int color) {
	// Receives cell to find out if there's a room that contains this cell.
	// If so, counts (and returns) the amount of 'color' cells in that room.
	// Exclude 'col' and 'row' from the count!
	for (int i = 0; i < NUM_ROOMS; i++) {
		Room room = this->rooms[i];
		if (room.contains(col, row)) {
			return room.getAmountOf(this->maze, col, row, color);
		}
	}
	return 0;
}

Room Maze::getRoomAt(int index) {
	return this->rooms[index];
}