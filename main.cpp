#include "raylib.h"
#include <stdio.h>
#include "player.h"

//player
Player _player;
Texture player_texture;
Texture player_sprite_up; 
Texture player_sprite_down;
Texture player_sprite_left;
Texture player_sprite_right;
Rectangle playerSrc = {0,0,24,24 };
Rectangle playerDest = { 0,0,100,100 };// 10 10 is size
Vector2 playerCenter = { playerSrc.width / 2,playerSrc.height / 2 };

//window
int window_height = 400;
int window_width = 800;

//camera
Camera2D camera;
Vector2 camera_offset = { float(window_width / 2), float(window_height / 2) };
Vector2 camera_pos = { float(playerDest.x - (playerDest.width / 2)),float(playerDest.y - (playerDest.height / 2)) };

//game tick
int player_tick;
int game_tick;


void ToggleFullScreenWindow(int windowWidth, int windowHeight) {
	if (!IsWindowFullscreen()) {
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else {
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
	}
}

void initialise() {
	InitWindow(window_width, window_height, "GAME JAM");
	SetTargetFPS(60);

	//initialise player sprite
	player_sprite_up = LoadTexture("assets/character/Walk Up.png");
	player_sprite_down = LoadTexture("assets/character/Walk Down.png");
	player_sprite_left = LoadTexture("assets/character/Walk Left.png");
	player_sprite_right = LoadTexture("assets/character/Walk Right.png");

	//initialise camera
	camera = { camera_offset,camera_pos,0,1 };

}

void input() {
	if (IsKeyPressed(KEY_F11)) {
		ToggleFullScreenWindow(window_width, window_height);
	}
	if (IsKeyDown(KEY_W)) {
		_player.isPlayerMoving = true;
		_player.player_dir_up = true;
		_player.dir = 1;
	}
	if (IsKeyDown(KEY_A)) {
		_player.isPlayerMoving = true;
		_player.player_dir_left = true;
		_player.dir = 0;
	}
	if (IsKeyDown(KEY_S)) {
		_player.isPlayerMoving = true;
		_player.player_dir_down = true;
		_player.dir = 2;
	}
	if (IsKeyDown(KEY_D)) {
		_player.isPlayerMoving = true;
		_player.player_dir_right = true;
		_player.dir = 3;
	}
}

void update() {
	if (_player.isPlayerMoving) {
		if (_player.player_dir_up) {
			_player.position.y -= PLAYERSPEED;
		}
		if (_player.player_dir_down) {
			_player.position.y += PLAYERSPEED;
		}
		if (_player.player_dir_left) {
			_player.position.x -= PLAYERSPEED;
		}
		if (_player.player_dir_right) {
			_player.position.x += PLAYERSPEED;
		}
		//update player tick
		if (game_tick % 8 == 1) {
			player_tick++;
		}
	}
	playerDest.x = _player.position.x;
	playerDest.y = _player.position.y;

	//update tick counter
	game_tick++;

	// reset player tick
	if (player_tick > 12) {
		player_tick = 0;
	}

	
	switch (_player.dir) {
	case 1: 
		player_texture = player_sprite_up;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 0: 
		player_texture = player_sprite_left;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 2:
		player_texture = player_sprite_down;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	case 3:
		player_texture = player_sprite_right;
		playerSrc.x = playerSrc.width * player_tick;
		playerSrc.y = 0;
		break;
	}
	
	//set to default animation if player not moving
	if (!_player.isPlayerMoving) {
		player_texture = player_sprite_down;
		playerSrc.x = 0;
		playerSrc.y = 0;
	}
	//camera
	camera.target = { (playerDest.x + (playerDest.width / 2)), float(playerDest.y + (playerDest.height / 2)) };

	//movement set to false
	_player.isPlayerMoving = false;
	_player.player_dir_up = false;
	_player.player_dir_down = false;
	_player.player_dir_left = false;
	_player.player_dir_right = false;

}

void render() {
	BeginDrawing();
	ClearBackground(WHITE);
	BeginMode2D(camera);
	
	DrawTexturePro(player_texture, playerSrc, playerDest, playerCenter, 0, WHITE);
	
	EndMode2D();
	EndDrawing();
}

void quit() {
	CloseWindow();
}




int main() {
	initialise();

	while (!WindowShouldClose()) {
		input();
		update();
		render();
	}

	quit();
	return 0;
}





