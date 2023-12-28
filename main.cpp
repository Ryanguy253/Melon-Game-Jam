#include "raylib.h"
#include <stdio.h>

//window
int window_height = 400;
int window_width = 800;

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


}

void input() {
	if (IsKeyPressed(KEY_F11)) {
		ToggleFullScreenWindow(window_width, window_height);
	}
}

void update() {

}

void render() {
	BeginDrawing();
	ClearBackground(BLACK);
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





