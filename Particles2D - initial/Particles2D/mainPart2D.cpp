#include "Game.h"

int main(int argc, char ** argv) {
	Game game("Game Particle2D", 800, 800, 60);

	try {
		game.run();
	}
	catch (std::exception e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}