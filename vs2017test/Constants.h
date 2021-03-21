#pragma once
#include <algorithm>

const long REFRESH_RATE_MILLISECOND = 50;

const int WIDTH = 600;
const int HEIGHT = 600;

const int MSZ = 120;
const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4;
const int GRAY = 5;
const int PATH = 6;
const int OBSTACLE = 7;

const int NUM_ROOMS = 10;
const int MAX_OBSTACLES_IN_ROOM = 10;
const int NUM_SOLDIERS = 10;

const double BULLET_DAMAGE = 0.4;

const int MAX_TURNS_TO_HIDE = 12;
const int DISTANCE_TO_SEARCH_ENEMY = 100;
const int DISTANCE_TO_SEARCH_KIT = 100;

const int MAX_GRENADES = 5;
const int GRENADE_KIT_AMOUNT = NUM_ROOMS * 2;
const int LOW_GRENADE_THRESHOLD = 2;

const int MAX_AMMO = 15;
const int AMMO_KIT_AMOUNT = NUM_ROOMS * 2;
const int LOW_AMMO_THRESHOLD = 5;

const int HP_KIT_AMOUNT = 20;
const int MAX_HP = 100;
const int LOW_HP_THRESHOLD = 50;

const int TEAM_RED = 100;
const int TEAM_BLUE = 101;
const int AMMO_KIT = 102;
const int HP_KIT = 103;
const int GRENADE_KIT = 104;
