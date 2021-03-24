#pragma once
#include "glut.h"
#include <time.h>
#include <iostream>
#include <chrono>
#include "Bullet.h"
#include "Maze.h"
#include "Team.h"
#include "Kits.h"

using namespace std;


Maze* maze = nullptr;
Team* blueTeam = nullptr;
Team* redTeam = nullptr;
vector<Kit*> allKits;
bool continueGame = false;


void initTeams() {
	blueTeam = new Team(maze, NUM_SOLDIERS, TEAM_BLUE, TEAM_RED);
	redTeam = new Team(maze, NUM_SOLDIERS, TEAM_RED, TEAM_BLUE);
}

void initKits(bool hp, bool ammo, bool grenade) {
	if (hp && ammo && grenade) {
		allKits.clear();
	}
	if (hp) {
		Kits* hpKits = new Kits(maze, HP_KIT_AMOUNT, HP_KIT);
		allKits.insert(allKits.end(), hpKits->getKits().begin(), hpKits->getKits().end());
	}
	if (ammo) {
		Kits* ammoKits = new Kits(maze, AMMO_KIT_AMOUNT, AMMO_KIT);
		allKits.insert(allKits.end(), ammoKits->getKits().begin(), ammoKits->getKits().end());
	}
	if (grenade) {
		Kits* grenadeKits = new Kits(maze, GRENADE_KIT_AMOUNT, GRENADE_KIT);
		allKits.insert(allKits.end(), grenadeKits->getKits().begin(), grenadeKits->getKits().end());
	}
}

void refillKits() {
	int hpKitCount = 0;
	int ammoKitCount = 0;
	int grenadeKitCount = 0;
	for (auto kit : allKits) {
		if (kit->getType() == HP_KIT) {
			hpKitCount++;
		} else if (kit->getType() == AMMO_KIT) {
			ammoKitCount++;
		} else if (kit->getType() == GRENADE_KIT) {
			grenadeKitCount++;
		}
	}
	initKits(hpKitCount == 0, ammoKitCount == 0, grenadeKitCount == 0);
}

void init() {
	cout << "Initializing the game..." << endl;
	glClearColor(0.8, 0.7, 0.5, 0.0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand(time(0));
	maze = new Maze();
	initTeams();
	initKits(true, true, true);
	cout << "The game is ready! Right click anywhere on the map to launch the game!" << endl;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	maze->drawMe();
	for (auto kit : allKits) {
		kit->drawMe();
	}
	if (blueTeam == nullptr && redTeam == nullptr) {
		return;
	}
	if (blueTeam == nullptr) {
		redTeam->drawMe();
		cout << "The game is over - red team won!" << endl;
		redTeam = nullptr;
	} else if (redTeam == nullptr) {
		blueTeam->drawMe();
		cout << "The game is over - blue team won!" << endl;
		blueTeam = nullptr;
	} else {
		if (blueTeam->isAlive()) {
			blueTeam->drawMe();
		} else {
			blueTeam = nullptr;
		}
		if (redTeam->isAlive()) {
			redTeam->drawMe();
		} else {
			redTeam = nullptr;
		}	
	}
	glutSwapBuffers(); // show all
}

void idle() {
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	if (continueGame && blueTeam != nullptr && redTeam != nullptr) {
		refillKits();
		blueTeam->play(redTeam, allKits);
		redTeam->play(blueTeam, allKits);
	}
	glutPostRedisplay(); // indirect call to display
	long elapsed;
	do {
		chrono::steady_clock::time_point now = chrono::steady_clock::now();
		elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
	} while (elapsed < REFRESH_RATE_MILLISECOND);
}

void menu(int choice) {
	switch (choice) {
	case 1:
		continueGame = true;
		break;
	case 2:
		continueGame = false;
		break;
	case 3:
		continueGame = false;
		initTeams();
		initKits(true, true, true);
		break;
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	//glutMouseFunc(mouse);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("Stop", 2);
	glutAddMenuEntry("Reset", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
	return 0;
}