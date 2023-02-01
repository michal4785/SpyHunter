#pragma once
#include "InterfaceSDL.h"
class Game
{
public:
	Game(InterfaceSDL *interface) {
		this->interface = interface;
		initialize();
	}

	void initialize() {
		playerPositionX = 0;
		playerPositionY = 0;
		moveDown = false;
		moveUp = false;
		moveLeft = false;
		moveRight = false;
		quit = 0;
		newGame = false;
		pause = false;
		civilCarPostitionX = InterfaceSDL::SCREEN_WIDTH/2 - InterfaceSDL::ROAD_WIDTH/2 + 2* InterfaceSDL::ROAD_WIDTH/5 + InterfaceSDL::DEFAULT_CAR_WIDTH/2;
		civilCarPostitionY = -100;
		civilCarSpeed = 1;


		delta = 0;
		t1 = SDL_GetTicks();
		t2 = SDL_GetTicks();
		frames = 0;
		fpsTimer = 0;
		fps = 0;
		worldTime = 0;
		speed = 1;
		score = 0;
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


			if (moveUp && playerPositionY > -InterfaceSDL::MAX_PLAYER_Y_OFFSET) {
				playerPositionY -= InterfaceSDL::MOTION_SPEED;
				speed += 0.1;
			}
			if (moveDown && playerPositionY < InterfaceSDL::MAX_PLAYER_Y_OFFSET) {
				playerPositionY += InterfaceSDL::MOTION_SPEED;
				speed -= 0.1;
			}
			if (moveLeft && playerPositionX > -InterfaceSDL::ROAD_WIDTH / 2) playerPositionX -= InterfaceSDL::MOTION_SPEED;
			if (moveRight && playerPositionX < InterfaceSDL::ROAD_WIDTH / 2) playerPositionX += InterfaceSDL::MOTION_SPEED;
			score += speed/2;
			civilCarPostitionY += round((speed - civilCarSpeed) * 10);


			interface->drawBoard(playerPositionY, playerPositionX, speed, civilCarPostitionY, civilCarPostitionX);

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};

			interface->drawInfoFrames(worldTime, fps, speed, round(score/10)*10);
			interface->listenEvents(quit, moveUp, moveDown, moveLeft, moveRight, newGame, pause, playerPositionY, playerPositionX, civilCarPostitionY, civilCarPostitionX);
			if (newGame) initialize();
			if (pause) stop();

			frames++;
			system("cls");
			printf("Y1: %d,\tX1: %d\n", playerPositionY + 2 * InterfaceSDL::SCREEN_HEIGHT/3, playerPositionX + InterfaceSDL::SCREEN_WIDTH / 2);
			printf("Y2: %d,\tX2: %d\n", civilCarPostitionY, civilCarPostitionX);
		};
		interface->clear();
	}

	void stop() {
		interface->listenEvents(quit, moveUp, moveDown, moveLeft, moveRight, newGame, pause, playerPositionY, playerPositionX, civilCarPostitionY, civilCarPostitionX);
		t1 = SDL_GetTicks();
	}
private:

	int t1;
	int t2;
	int frames;
	double delta;
	double worldTime;
	double fpsTimer;
	double fps;
	int playerPositionY;
	int playerPositionX;
	int civilCarPostitionY;
	int civilCarPostitionX;
	double civilCarSpeed;
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;
	bool newGame;
	bool pause;
	int quit;
	double speed;
	double score;
	InterfaceSDL* interface;
};

