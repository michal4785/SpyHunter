#define _USE_MATH_DEFINES
#include "InterfaceSDL.h"
#include "Game.h"

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	
	InterfaceSDL *  interface = new InterfaceSDL();
	int result = interface->initialize();
	if (result) return result;
	Game game = Game(interface);
	game.start();
	return 0;
	};
