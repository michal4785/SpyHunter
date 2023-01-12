#pragma once
#include<stdio.h>
#include<string.h>
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define ROAD_WIDTH		100

class InterfaceSDL
{
public:
	InterfaceSDL() {
		text = new char[128];
		
	}

	int initialize() {

		// okno konsoli nie jest widoczne, jeøeli chcemy zobaczyÊ
		// komunikaty wypisywane printf-em trzeba w opcjach:
		// project -> szablon2 properties -> Linker -> System -> Subsystem
		// zmieniÊ na "Console"
		printf("wyjscie printfa trafia do tego okienka\n");
		printf("printf output goes here\n");
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			printf("SDL_Init error: %s\n", SDL_GetError());
			return 1;
		}

		// tryb pe≥noekranowy / fullscreen mode
		//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
		//	                                 &window, &renderer);
		rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			&window, &renderer);
		if (rc != 0) {
			SDL_Quit();
			printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
			return 1;
		};

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2017");


		screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			SCREEN_WIDTH, SCREEN_HEIGHT);


		czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);


		// wy≥πczenie widocznoúci kursora myszy
		SDL_ShowCursor(SDL_DISABLE);

		// wczytanie obrazka cs8x8.bmp
		charset = SDL_LoadBMP("./cs8x8.bmp");
		if (charset == NULL) {
			printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(screen);
			SDL_DestroyTexture(scrtex);
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			SDL_Quit();
			return 1;
		};
		SDL_SetColorKey(charset, true, 0x000000);

		eti = SDL_LoadBMP("./eti.bmp");
		if (eti == NULL) {
			printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(charset);
			SDL_FreeSurface(screen);
			SDL_DestroyTexture(scrtex);
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			SDL_Quit();
			return 1;
		};
		return 0;
	}
	void drawBoard(int playerPositionY, int playerPositionX) {
		SDL_FillRect(screen, NULL, czarny);
		DrawSurface(screen, eti,
			SCREEN_WIDTH / 2 + playerPositionX,
			SCREEN_HEIGHT / 2 + playerPositionY);
	}

	void drawInfoFrame(int worldTime, int fps) {
		// tekst informacyjny
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
		sprintf(text, "Imie i nazwisko: Michal Ganczarenko, nr indeksu: 188818, \nczas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		sprintf(text, "Esc - wyjscie, \032, \030, \033, \031 - ruch");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void listenEvents(int &quit, bool & moveUp, bool& moveDown, bool& moveLeft, bool& moveRight) {
		// obs≥uga zdarzeÒ (o ile jakieú zasz≥y) / handling of events (if there were any)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_UP)  moveUp = true;
				else if (event.key.keysym.sym == SDLK_DOWN)  moveDown = true;
				else if (event.key.keysym.sym == SDLK_LEFT)  moveLeft = true;
				else if (event.key.keysym.sym == SDLK_RIGHT)  moveRight = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_UP) moveUp = false;
				else if (event.key.keysym.sym == SDLK_DOWN) moveDown = false;
				else if (event.key.keysym.sym == SDLK_LEFT) moveLeft = false;
				else if (event.key.keysym.sym == SDLK_RIGHT) moveRight = false;
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
	}

	void clear() {
		// zwolnienie powierzchni
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
	}

	// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
	// charset to bitmapa 128x128 zawierajπca znaki
	// draw a text txt on surface screen, starting from the point (x, y)
	// charset is a 128x128 bitmap containing character images

	void DrawString(SDL_Surface* screen, int x, int y, const char* text,
		SDL_Surface* charset) {
		int px, py, c;
		SDL_Rect s, d;
		s.w = 8;
		s.h = 8;
		d.w = 8;
		d.h = 8;
		while (*text) {
			c = *text & 255;
			px = (c % 16) * 8;
			py = (c / 16) * 8;
			s.x = px;
			s.y = py;
			d.x = x;
			d.y = y;
			SDL_BlitSurface(charset, &s, screen, &d);
			x += 8;
			text++;
		};
	};


	// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
	// (x, y) to punkt úrodka obrazka sprite na ekranie
	// draw a surface sprite on a surface screen in point (x, y)
	// (x, y) is the center of sprite on screen
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
		SDL_Rect dest;
		dest.x = x - sprite->w / 2;
		dest.y = y - sprite->h / 2;
		dest.w = sprite->w;
		dest.h = sprite->h;
		SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


	// rysowanie pojedynczego pixela
	// draw a single pixel
	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		*(Uint32*)p = color;
	};


	// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
	// bπdü poziomie (gdy dx = 1, dy = 0)
	// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
	void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
		for (int i = 0; i < l; i++) {
			DrawPixel(screen, x, y, color);
			x += dx;
			y += dy;
		};
	};


	// rysowanie prostokπta o d≥ugoúci bokÛw l i k
	// draw a rectangle of size l by k
	void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor) {
		int i;
		DrawLine(screen, x, y, k, 0, 1, outlineColor);
		DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
		DrawLine(screen, x, y, l, 1, 0, outlineColor);
		DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
		for (i = y + 1; i < y + k - 1; i++)
			DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

	private:
		SDL_Event event;
		SDL_Surface* screen, * charset;
		SDL_Surface* eti;
		SDL_Texture* scrtex;
		SDL_Window* window;
		SDL_Renderer* renderer;
		int rc;
		int czarny;
		int zielony;
		int czerwony;
		int niebieski;
		char* text;
};

