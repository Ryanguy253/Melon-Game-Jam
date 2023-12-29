#include "raylib.h"
#include <iostream>
#include "player.h"
#include "shop.h"
#include "raymath.h"
#include "trading.h"
#include "items.h"

//things to add
// logic for trading, (too exp) (accept) (number of trades)




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
Rectangle playerDest = { 0,0,100,100 };// 100 100 is size

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
items _itemsArray[11];
shop _shop[SHOP_ITEMS] = { 0 };

//npc
npc _npc[MAXNPC] = { 0 };
float _lastnpccreationtime = 1;

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

//trading game
bool market_scene = true;
// initialise trading scene
Texture2D trading_background;
bool teleport_flag = false;
Texture2D ghost;
bool collide_with_npc = false;
bool trading = false;
npc* npc_that_collide_with_player = NULL;
bool npc_quit_trading = false;
bool trades = 0;
int trade_count = 0;

#define RANDOM_ARRAY_SIZE 200
int random_array[RANDOM_ARRAY_SIZE] = { 0 };
bool item_match = false;
bool item_does_not_match = false;
bool npc_rejection = false;
bool npc_accept = false; 
int nego_price = 0;
int npc_acceptable_price_range;
bool closing_deal = false;
bool sucessful_deal = false;
int itemNum = 0;
bool pause_game = false;
int pause_x;
int pause_y;

//inventory
Inventory _player_inventory;
Texture slot1;
Texture slot2;
Texture slot3;
Texture slot4;

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

	//initalise trading
	trading_background = LoadTexture("assets/nightMarket/night_market.png");
	ghost = LoadTexture("assets/nightMarket/ghost.png");

	// initialise random array
	for (int i = 0; i < RANDOM_ARRAY_SIZE; i++) {
		random_array[i] = GetRandomValue(1, 10);
	}
	//initialise items
	
	items charm_of_wisdom;
	charm_of_wisdom.id = 1;
	charm_of_wisdom.base_price = 10;
	charm_of_wisdom.texture = LoadTexture("assets/Items/charm_of_wisdom.png");

	items aurora_veil;
	aurora_veil.id = 2;
	aurora_veil.base_price = 15;
	aurora_veil.texture = LoadTexture("assets/Items/aurora_veil.png");

	items soulweave_locket;
	soulweave_locket.id = 3;
	soulweave_locket.base_price = 13;
	soulweave_locket.texture = LoadTexture("assets/Items/soulweave_locket.png");

	items timekeepers_hourglass;
	timekeepers_hourglass.id = 4;
	timekeepers_hourglass.base_price = 11;
	timekeepers_hourglass.texture = LoadTexture("assets/Items/timekeepers_hourglass.png");

	items dreamweaver_dreamcatcher;
	dreamweaver_dreamcatcher.id = 5;
	dreamweaver_dreamcatcher.base_price = 12;
	dreamweaver_dreamcatcher.texture = LoadTexture("assets/Items/dreamweaver_dreamcatcher.png");

	items vitalis_pendant;
	vitalis_pendant.id = 6;
	vitalis_pendant.base_price = 16;
	vitalis_pendant.texture = LoadTexture("assets/Items/vitalis_pendant.png");

	items quicksilver_sylph_amulet;
	quicksilver_sylph_amulet.id = 7;
	quicksilver_sylph_amulet.base_price = 20;
	quicksilver_sylph_amulet.texture = LoadTexture("assets/Items/quicksilver_sylph_amulet.png");

	items oceanic_whisper_conch;
	oceanic_whisper_conch.id = 8;
	oceanic_whisper_conch.base_price = 25;
	oceanic_whisper_conch.texture = LoadTexture("assets/Items/oceanic_whisper_conch.png");

	items metamorphos_bloom_cocoon;
	metamorphos_bloom_cocoon.id = 9;
	metamorphos_bloom_cocoon.base_price = 23;
	metamorphos_bloom_cocoon.texture = LoadTexture("assets/Items/metamorphos_bloom_cocoon.png");

	items celestial_wayfarer_compass;
	celestial_wayfarer_compass.id = 10;
	celestial_wayfarer_compass.base_price = 30;
	celestial_wayfarer_compass.texture = LoadTexture("assets/Items/celestial_wayfarer_compass.png");
	

	//initialise itemsArray

	_itemsArray[0] = {0};
	_itemsArray[1] = aurora_veil;
	_itemsArray[2] = soulweave_locket;
	_itemsArray[3] = timekeepers_hourglass;
	_itemsArray[4] = dreamweaver_dreamcatcher;
	_itemsArray[5] = vitalis_pendant;
	_itemsArray[6] = quicksilver_sylph_amulet;
	_itemsArray[7] = oceanic_whisper_conch;
	_itemsArray[8] = metamorphos_bloom_cocoon;
	_itemsArray[9] = celestial_wayfarer_compass;
	_itemsArray[10] = charm_of_wisdom;

	//initialise inventory
	_player_inventory.ITEM1 = 0;
	_player_inventory.ITEM2 = 0;
	_player_inventory.ITEM3 = 9;
	_player_inventory.ITEM4 = 10;


}

void addNPC(Vector2 position) {
	//position of NPC spawn
	bool created = false;
	Vector2 velocity = Vector2Subtract(Vector2{-10000,position.y}, position);
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
		TraceLog(LOG_ERROR, "Failed to create an NPC because there were no inactive spots in the array");
	}

}

Vector2 GetNextNPCPostion() {
	
	return Vector2{ -100, 1330 };
	
}

void DrawNpc(npc npc) {

	if (!npc.active) {
		return;
	}
	
	DrawTexturePro(ghost, {0,0,16,16},{npc.position.x,npc.position.y,100,100},{0,0}, 0, WHITE);
}

void drawShop() {
	/*//draw tile map
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

			DrawRectangle(tileDest.x, tileDest.y, 100, 100, DARKGRAY);
			break;
		case 2:
			tileDest.x = tileDest.width * float(i % shop_tile_map_width);
			tileDest.y = tileDest.height * float(i / shop_tile_map_width);

			DrawRectangle(tileDest.x, tileDest.y, 100, 100, DARKBLUE);
			break;
		}
	}*/

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

	//initialise and draw textures for slots only if inventory contains something
	if (_player_inventory.ITEM1 != 0) {
		slot1 = _itemsArray[_player_inventory.ITEM1].texture;
	}
	if (_player_inventory.ITEM2 != 0) {
		slot2 = _itemsArray[_player_inventory.ITEM2].texture;
	}
	if (_player_inventory.ITEM3 != 0) {
		slot3 = _itemsArray[_player_inventory.ITEM3].texture;
	}
	if (_player_inventory.ITEM4 != 0) {
		slot4 = _itemsArray[_player_inventory.ITEM4].texture;
	}
	
	
	DrawTexturePro(slot1, { 0,0,16,16 }, r1, Vector2{ 400 / 2,0 }, 0,WHITE);
	DrawTexturePro(slot2, { 0,0,16,16 }, r2, Vector2{ 400 / 2,0 }, 0, WHITE);
	DrawTexturePro(slot3, { 0,0,16,16 }, r3, Vector2{ 400 / 2,0 }, 0, WHITE);
	DrawTexturePro(slot4, { 0,0,16,16 }, r4, Vector2{ 400 / 2,0 }, 0, WHITE);




}

void trading_game_update() {

	if (GetTime() > _lastnpccreationtime + NPCDELAY) {
		addNPC(GetNextNPCPostion());
		_lastnpccreationtime = GetTime();
	}

	//update npc
	for (int i = 0; i < MAXNPC; i++) {
		UpdateNPC(&_npc[i], GetFrameTime());
	}

	//check for collision with NPC
	for (int i = 0; i < MAXNPC; i++) {
		if (!_npc->active) {
			continue;
		}
		if (CheckCollisionPointRec({ playerDest.x,playerDest.y }, { _npc[i].position.x,_npc[i].position.y,100,100 })) {
			collide_with_npc = true;
			npc_that_collide_with_player = &_npc[i];
			if (market_scene && collide_with_npc &&!trading&&!item_does_not_match&&!item_match) {
				DrawText("Press E to interact", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
				break;
			}
		}
	}

	if (IsKeyPressed(KEY_E) && !trading) {
		pause_game = true;
		npc_quit_trading = false;
		trading = true;
		npc_said_first_sentence = false;
		npc_said_second_sentence = false;
		npc_that_collide_with_player->velocity = { 0 };
		
	}

	if (trading) {
		if (GetTime() > (npc_that_collide_with_player->creationTime) + (NPCLIFE - 10)) {
			npc_quit_trading = true;
			trading = false;
			npc_that_collide_with_player->velocity = { 100,-100 };
		}
	}
	


}

void trading_game_render() {
	drawShop();
	drawInventoryUI();
	
	if (trading) {
		if (!npc_said_first_sentence) {
			DrawText("Ghost : I'm looking for a charm that will guide me in my next life",playerDest.x - GetMonitorWidth(monitor) / 4, 1600, 35, WHITE);
			DrawText("Press Enter to continue", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
			if (IsKeyDown(KEY_ENTER)) {
				npc_said_first_sentence = true;	
				npc_said_second_sentence = false;
			}
		}
		if (!npc_said_second_sentence && npc_said_first_sentence && !item_does_not_match && !item_match) {
			//change hardcoded values
			DrawText(getDialog(random_array[trade_count]), playerDest.x - GetMonitorWidth(monitor) / 4, 1600, 35, WHITE);
			DrawText("Use Number Keys 1-4 to offer items", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, RED);
			
			DrawText("I don't have it... -> Press KEY F (Ghost will leave)", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
			if (IsKeyPressed(KEY_F)) {
				pause_game = true;
				npc_said_second_sentence = true;
				npc_quit_trading = true;
				trading = false;
				trade_count++;
			}
			
			

			if (IsKeyPressed(KEY_ONE)) {

				if (_player_inventory.ITEM1 == random_array[trade_count]) {
					itemNum = 1;
					item_match = true;
					item_does_not_match = false;

					trading = false;
				}
				else {
					item_match = false;
					item_does_not_match = true;
					trading = false;
				}
			}
			if (IsKeyPressed(KEY_TWO)) {
				if (_player_inventory.ITEM2 == random_array[trade_count]) {
					itemNum = 2;
					item_match = true;
					item_does_not_match = false;

					trading = false;
				}
				else {
					item_match = false;
					item_does_not_match = true;
					trading = false;
				}
			}
			if (IsKeyPressed(KEY_THREE)) {
				if (_player_inventory.ITEM3 == random_array[trade_count]) {
					itemNum = 3;
					item_match = true;
					item_does_not_match = false;

					trading = false;
				}
				else {
					item_match = false;
					item_does_not_match = true;
					trading = false;
				}
			}
			if (IsKeyPressed(KEY_FOUR)) {
				
				if (_player_inventory.ITEM4 == random_array[trade_count]) {
					itemNum = 4;
					item_match = true;
					item_does_not_match = false;

					trading = false;
				}
				else {
					item_match = false;
					item_does_not_match = true;
					trading = false;
				}
			}
		}
	}

	
	if (item_match&&!trading&&!closing_deal&&!npc_rejection) {
		DrawText("Ghost : Yes, that's it. How much?", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		//cout << "YES HOW MUCH!!" << endl;
		DrawText(TextFormat("Price: %i (Use arrow keys to change) (Use ENTER to confirm)",nego_price), playerDest.x - GetMonitorWidth(monitor) / 4, 1800, 30, {255,126,0,255});
		DrawText(TextFormat("Base Price: %i ", _itemsArray[random_array[trade_count] - 1].base_price), playerDest.x - GetMonitorWidth(monitor) / 4, 1900, 30, { 255,126,0,255 });

		if (IsKeyPressed(KEY_UP)) {
			nego_price++;
		}

		if (IsKeyPressed(KEY_DOWN)) {
			nego_price--;
		}

		if (nego_price < 0) {
			nego_price = 0;
		}

		if (IsKeyPressed(KEY_ENTER)) {
			npc_acceptable_price_range = GetRandomValue(_itemsArray[random_array[trade_count] - 1].base_price - 30, _itemsArray[random_array[trade_count] - 1].base_price + 15);
			if (nego_price < _itemsArray[random_array[trade_count] - 1].base_price) {
				closing_deal = true;
			}
		}
	}

	if (closing_deal&&!sucessful_deal) {

		ClearBackground(BLACK);
		DrawText("Ghost : Thats a great deal !! I'll take it.", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		//cout << "THATS A GREAT DEAL!!" << endl;
		DrawText("Press SPACE to continue ", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
		if (IsKeyPressed(KEY_SPACE)) {
			sucessful_deal = true;
		}

	}

	if (sucessful_deal) {
		trade_count++;
		npc_quit_trading = true;
	}

	if (item_does_not_match&&!trading) {
		trade_count++;
		npc_rejection = true;
		npc_quit_trading = true;
	}

	if (npc_rejection&&!trading) {
		DrawText("Ghost : No, that's not it.", playerDest.x - GetMonitorWidth(monitor) / 4, 1700, 35, WHITE);
		//cout << "NO THATS NOT IT" << endl;
		DrawText("Press SPACE to continue ", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
		pause_game = true;
		//DrawText("Press SPACE to continue.", playerDest.x - 150, playerDest.y - 100, 35, WHITE);
		
		if (IsKeyPressed(KEY_SPACE)) {
			pause_game = false;
			npc_rejection = false;
			npc_quit_trading = true;
		}
	}

	if (npc_quit_trading) {
		npc_that_collide_with_player->velocity = { 100,-100 };

		trading = false;
		npc_said_first_sentence = false;
		npc_said_second_sentence = false;
		npc_quit_trading = false;
		item_does_not_match = false;
		item_match = false;
		closing_deal = false;
		sucessful_deal = false;
		pause_game = false;
	}
}

void trading_game() {
	DrawTexture(trading_background, 0, 0, WHITE);
	if (!teleport_flag) {
		_player.position.x = 0;
		_player.position.y = 1350;
		teleport_flag = true;
	}
	//collision with side
	if (_player.position.x < trading_background.width/3 ) {
		_player.position.x = trading_background.width/3;
	}
	if (_player.position.x > trading_background.width - trading_background.width / 2.5) {
		_player.position.x = trading_background.width - trading_background.width / 2.5;
	}
	
	if (!pause_game) {
		trading_game_update();
	}
	trading_game_render();
	
	//set collide to false
	collide_with_npc = false;
	
}

void input() {

	//disable up and down for market
	if (!market_scene) {
		if (IsKeyDown(KEY_W)) {
			_player.isPlayerMoving = true;
			_player.player_dir_up = true;
			_player.dir = 1;
		}
		if (IsKeyDown(KEY_S)) {
			_player.isPlayerMoving = true;
			_player.player_dir_down = true;
			_player.dir = 2;
		}
	}
	if (IsKeyDown(KEY_A)) {
		_player.isPlayerMoving = true;
		_player.player_dir_left = true;
		_player.dir = 0;
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
	/*for (int i = 0; i < (shop_tile_map_height * shop_tile_map_width); i++) {
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
	*/
	//movement set to false
	_player.isPlayerMoving = false;
	_player.player_dir_up = false;
	_player.player_dir_down = false;
	_player.player_dir_left = false;
	_player.player_dir_right = false;

}

void render() {
	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode2D(camera);

	trading_game();
	DrawTexturePro(player_texture, playerSrc, playerDest, playerCenter, 0, WHITE);
	//std::cout << "X : " << _player.position.x << "Y : " << _player.position.y << std::endl;
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





