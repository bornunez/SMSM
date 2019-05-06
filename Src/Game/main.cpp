//#define C_DEBUG;

#include "Game.h"
#include "Loader/MyComponentLoader.h"
/*#include <btBulletDynamicsCommon.h>*/

int main() {

	MyComponentLoader* mCompLoader = new MyComponentLoader();
	Game game(mCompLoader);
	game.Play();

	return 0;
}