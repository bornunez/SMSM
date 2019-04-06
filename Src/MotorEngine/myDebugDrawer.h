#include <btBulletDynamicsCommon.h>
#include <Ogre.h>


class myDebugDrawer: public btIDebugDraw
{
public:
	myDebugDrawer();
	virtual ~myDebugDrawer() {};

	static myDebugDrawer* Instance(Ogre::SceneManager* m = nullptr);

	void init() {};

	void deinit() {};

	void debugPhysics(bool enable) {};

	virtual void drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar) { (void)v0; (void)v1; (void)v2; (void)color; }
	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) { (void)PointOnB; (void)normalOnB; (void)distance; (void)lifeTime; (void)color; }
	virtual void reportErrorWarning(const char *warningString) { (void)warningString; }
	virtual void draw3dText(const btVector3 &location, const char *textString) { (void)location; (void)textString; }
	virtual void setDebugMode(int debugMode) { debugModes_ = (DebugDrawModes)debugMode; }
	virtual int getDebugMode() const { return debugModes_; }

	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);

	void drawCube(btVector3 position, btVector3 scale);

	void resetLineNumber();

protected:
	bool frameStarted(const Ogre::FrameEvent& evt) {};
	bool frameEnded(const Ogre::FrameEvent& evt) {};

private:
	DebugDrawModes debugModes_;
	static myDebugDrawer* instance_;
	Ogre::SceneManager* sceneManager_;
	int lineCounter = 0;
};

