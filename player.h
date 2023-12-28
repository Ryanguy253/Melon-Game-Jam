#pragma once
#include "raylib.h"


#define PLAYERSPEED 6

typedef struct Player {

	Vector2 position;
	int dir;
	int health;
	bool player_dir_up, player_dir_down, player_dir_left, player_dir_right;
	bool isPlayerMoving;

}Player;