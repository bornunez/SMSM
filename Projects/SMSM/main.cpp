#include <OgreRoot.h>
/*#include <btBulletDynamicsCommon.h>*/

using namespace Ogre;

int main() {

	Root* root;

#if _DEBUG
	root = new Root("plugins_d.cfg");
#else
	root = new Root("plugins.cfg");
#endif

	return 0;
}