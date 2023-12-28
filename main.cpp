#include "raylib.h"
#include <iostream>
#include "player.h"
#include "shop.h"

//monitor
static int monitor;

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

//tile
Rectangle tileDest = { 0,0,100,100 };//100,100 is size
//tile debug
int current_tile;
bool showCurrentTile = false;

//camera

Camera2D camera;
Vector2 camera_offset = { 0 };
Vector2 camera_pos = {0 };

//game tick
int player_tick;
int game_tick;

//shop items
#define SHOP_ITEMS 4

shop _shop[SHOP_ITEMS] = { 0 };

//npc
npc _npc[MAXNPC] = { 0 };
float _lastnpccreationtime = -1;

//screen
/*void ToggleFullScreenWindow(int windowWidth, int windowHeight) {
	if (!IsWindowFullscreen()) {
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else {
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
	}
}*/

void initialise() {
	//init window must be first
	InitWindow(800, 450, "GAME JAM");

	//window
	monitor = GetCurrentMonitor();
	int current_monitor_height = GetMonitorHeight(monitor);
	int current_monitor_width = GetMonitorWidth(monitor);

	SetWindowSize(current_monitor_width, current_monitor_height);
	ToggleFullscreen();
	SetTargetFPS(60);

	//initialise player sprite
	player_sprite_up = LoadTexture("assets/character/Walk Up.png");
	player_sprite_down = LoadTexture("assets/character/Walk Down.png");
	player_sprite_left = LoadTexture("assets/character/Walk Left.png");
	player_sprite_right = LoadTexture("assets/character/Walk Right.png");

	//initialise camera
	camera = { camera_offset,camera_pos,0,1 };

}

void addNPC(Vector2 position) {
	//position of NPC spawn
	bool created = false;
	Vector2 velocity = Vector2Subtract(Vector2{-1000,position.y}, position);
	velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(NPC_SPEED_MIN, NPC_SPEED_MAX));

	npc npc = createNPC(position, velocity);
	for (int i = 0; i < MAXNPC; i++) {
		if (_npc[i].active) {
			continue;
		}
		_npc[i] = npc;
		break;
	}
	if (!created) {
		TraceLog(LOG_ERROR, "Failed to create an asteroid because there were no inactive spots in the array");
	}
}

Vector2 GetNextNPCPostion() {
	if (GetRandomValue(0, 1)) {
		return Vector2{ -100, 60 };
	}
	else {
		return Vector2{ -100, 200 };
	}
}


void drawShop() {
	//draw tile map
	for (int i = 0; i < SHOPSIZE; i++) {
		switch (shop_tile_map[i]) {
		case 0:
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);

			DrawRectangle(tileDest.x, tileDest.y, 100, 100, GRAY);
			break;
		case 1:
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);

			DrawRectangle(tileDest.x, tileDest.y, 100, 100, DARKBLUE);
			break;
		case 2:
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);

			DrawRectangle(tileDest.x, tileDest.y, 100, 100, RED);
			break;
		}
	}
	// draw npc
	for (int i = 0; i < MAXNPC; i++) {
		DrawNpc(_npc[i]);
	}
}

void drawInventoryUI() {
	Rectangle r { ((playerDest.x + (playerDest.width / 2))), float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight() / 2,400,100};
	DrawRectanglePro(r, Vector2{ 400/2,-50/2}, 0, BROWN);

	Rectangle r1{ ((playerDest.x + (playerDest.width / 2)) + 10), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2) + 30 ,80,80 };
	DrawRectanglePro(r1, Vector2{400/2,0 }, 0, DARKBROWN);

	Rectangle r2{ ((playerDest.x + (playerDest.width / 2)) + 30 + 80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2)+30,80,80 };
	DrawRectanglePro(r2, Vector2{400/2,0}, 0, DARKBROWN);

	Rectangle r3{ ((playerDest.x + (playerDest.width / 2)) + 50+80+80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2)+30,80,80 };
	DrawRectanglePro(r3, Vector2{ 400 / 2,0 }, 0, DARKBROWN);

	Rectangle r4{ ((playerDest.x + (playerDest.width / 2)) + 70+80+80+80), float(playerDest.y + (playerDest.height / 2)) - (GetScreenHeight() / 2)+30,80,80 };
	DrawRectanglePro(r4, Vector2{ 400 / 2,0 }, 0, DARKBROWN);
}



void input() {
	
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
		if (game_tick % 3 == 1) {
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

	//give player animation
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
	monitor = GetCurrentMonitor();
	camera_offset = { float(GetMonitorWidth(monitor) / 2), float(GetMonitorHeight(monitor) / 2) };
	camera_pos = { float(playerDest.x - (playerDest.width / 2)),float(playerDest.y - (playerDest.height / 2)) };
	camera.target = { ((playerDest.x + (playerDest.width / 2))-GetScreenWidth()/2), float(playerDest.y + (playerDest.height / 2)) - GetScreenHeight()/2 };
	
	//checktile
	for (int i = 0; i < (shop_tile_map_height * shop_tile_map_width); i++) {
		if (shop_tile_map[i] == 2) {
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);
			if (CheckCollisionPointRec(Vector2{ (playerDest.x + (playerSrc.width)) ,(playerDest.y + (playerSrc.width)) }, tileDest)) {
				current_tile = 2; // red tile
			}
		}
		else if (shop_tile_map[i] == 1) {
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);
			if (CheckCollisionPointRec(Vector2{ (playerDest.x + (playerSrc.width)) ,(playerDest.y + (playerSrc.width)) }, tileDest)) {
				current_tile = 1; // black tile
			}
		}
		else if (shop_tile_map[i] == 0) {
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);
			if (CheckCollisionPointRec(Vector2{ (playerDest.x + (playerSrc.width)) ,(playerDest.y + (playerSrc.width)) }, tileDest)) {
				current_tile = 0; // white tile
			}
		}
	}


	//spawn npc
	if (GetTime() > _lastnpccreationtime + NPCDELAY) {
		addNPC(GetNextNPCPostion());
		_lastnpccreationtime = GetTime();
	}
	//update and despawn npc
	for (int i = 0; i < MAXNPC; i++) {
		UpdateNPC(&_npc[i],GetFrameTime());
	}
	
	int count = 0;
	for (int i = 0; i < MAXNPC; i++) {
		if (_npc[i].active) {
			count++;
		}
	}
	std::cout << "NPC COUNT : " << count << std::endl;

	//movement set to false
	_player.isPlayerMoving = false;
	_player.player_dir_up = false;
	_player.player_dir_down = false;
	_player.player_dir_left = false;
	_player.player_dir_right = false;

}

void render() {
	BeginDrawing();
	ClearBackground(SKYBLUE);
	BeginMode2D(camera);

	drawShop();

	DrawTexturePro(player_texture, playerSrc, playerDest, playerCenter, 0, WHITE);

	//debug for current tile
	if (showCurrentTile) {
		DrawText(TextFormat("Current Tile: %i", current_tile), playerDest.x - 60, playerDest.y - 30, 30, GREEN);
	}
	drawInventoryUI();


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





