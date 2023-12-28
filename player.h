#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"


#define PLAYERSPEED 4

typedef struct Player {

	Vector2 position;
	int dir;
	int health;
	int coins;
	bool player_dir_up, player_dir_down, player_dir_left, player_dir_right;
	bool isPlayerMoving;

}Player;

typedef struct Inventory {

	int ITEM1 = 0;
	int ITEM2 = 0;
	int ITEM3 = 0;
	int ITEM4 = 0;
	
}Inventory;
#endif