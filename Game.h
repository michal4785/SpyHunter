#pragma once
#include "InterfaceSDL.h"
class Game
{
public:
	Game(InterfaceSDL *interface) {
		this->interface = interface;
		positionX = 0;
		positionY = 0;
		moveDown = false;
		moveUp = false;
		moveLeft = false;
		moveRight = false;
		quit = 0;

		t1 = SDL_GetTicks();
		frames = 0;
		fpsTimer = 0;
		fps = 0;
		worldTime = 0;
		distance = 0;
		etiSpeed = 10;
	}
	void start() {
		while (!quit) {
			t2 = SDL_GetTicks();

			// w tym momencie t2-t1 to czas w milisekundach,
			// jaki uplyna³ od ostatniego narysowania ekranu
			// delta to ten sam czas w sekundach
			delta = (t2 - t1) * 0.001;
			t1 = t2;

			worldTime += delta;

			distance += etiSpeed * delta;


			if (moveUp && positionY > -SCREEN_HEIGHT / 2) positionY -= etiSpeed;
			if (moveDown && positionY < SCREEN_HEIGHT / 2) positionY += etiSpeed;
			if (moveLeft && positionX > -ROAD_WIDTH / 2) positionX -= etiSpeed;
			if (moveRight && positionX < ROAD_WIDTH / 2) positionX += etiSpeed;

			interface->drawBoard(positionY, positionX);

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};

			interface->drawInfoFrame(worldTime, fps);
			interface->listenEvents(quit, moveUp, moveDown, moveLeft, moveRight);

			frames++;
			system("cls");
			printf("Y: %d,\tX: %d\n", positionY, positionX);
		};
		interface->clear();
	}
private:

	int t1;
	int t2;
	int frames;
	double delta;
	double worldTime;
	double fpsTimer;
	double fps;
	double distance;
	double etiSpeed;
	int positionY;
	int positionX;
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;
	int quit;
	InterfaceSDL* interface;
};

