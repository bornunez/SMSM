//#define C_DEBUG;

#include "Game.h"
#include "Loader/MyComponentLoader.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
/*#include <btBulletDynamicsCommon.h>*/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MyComponentLoader* mCompLoader = new MyComponentLoader();
	Game game(mCompLoader);
	game.Play();
	
	//delete mCompLoader;
	return 0;
}