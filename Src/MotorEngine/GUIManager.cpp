#include "GUIManager.h"
#include "Game.h"
#include "Ogre.h"
#include "OgreRectangle2D.h"
#include "AudioManager.h"
#include "GameSceneManager.h"
#include "../../../Src/Game/Componentes/Player/PlayerController.h"

GUIManager* GUIManager::instance_ = nullptr;

GUIManager::GUIManager(Ogre::RenderWindow* w, Game* g)
{
	renderWindow_ = w;
	g_ = g;

	// Registramos las funciones de los botones con un nombre distintivo en un diccionario de funciones
	functions["exit"]			= &GUIManager::Exit;
	functions["mainScene"]		= &GUIManager::InitMainScene;
	functions["pause"]			= &GUIManager::togglePause;
	functions["menu"]			= &GUIManager::toggleMenu;
	functions["VolumeUp"]		= &GUIManager::VolumeUp;
	functions["VolumeDown"]		= &GUIManager::VolumeDown;
	functions["Mute"]			= &GUIManager::Mute;
	functions["SensitivityUp"]	= &GUIManager::SensitivityUp;
	functions["SensitivityDown"]= &GUIManager::SensitivityDown;
}

GUIManager::~GUIManager()
{
	// toDo: release memory

	CEGUI::OgreRenderer::destroySystem();
}

GUIManager * GUIManager::Instance(Ogre::RenderWindow* w, Game* g)
{
	// Para no sobreescribir una instancia del GUIManager
	if (instance_ == nullptr) {
		instance_ = new GUIManager(w, g);
	}
	return instance_;
}

void GUIManager::Update()
{
	checkKeys();
	checkMouse();	
}

void GUIManager::checkMouse()
{
	// Los inject permiten comprobar la posicion del mouse en el hud y cuando se hace click sobre las diferentes ventanas (botones)

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// Actualizamos la posicion del raton
	context.injectMousePosition(InputManager::getInstance()->getMouseX(), InputManager::getInstance()->getMouseY());

	// Click izquierdo
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Left)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	}
	else if (!gameEnded && InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Left)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}

	// Click rueda
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Middle)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
	}
	else if (InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Middle)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
	}

	// Click derecho
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Right)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
	}
	else if (InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Right)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
	}
}

void GUIManager::checkKeys()
{
	if (gameHUD) {
		if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_ESCAPE)) {		//ESCAPE es el unico boton de teclado que queremos reconocer en los menus (para entrar o salir de la pausa)
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);			
		}
		else if (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_ESCAPE)) {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);
			togglePause();
		}
	}
}

void GUIManager::Initialize()
{
	// Init CEGUI System
	renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(renderWindow_));

	// Load CEGUI Resources
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	// Set default resources
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");	

	// Create CEGUI root object
	wmgr = &CEGUI::WindowManager::getSingleton();
	myRoot = wmgr->createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);
}

// NOTA:

// El problema con la asignacion de los callbacks de los botones es la asignacion de parametros
// Es decir se puede hacer con diccionario de metodos sin parametros que llamen por debajo a metodos con
// parametros espeficicos para ese metodo

// Hay que pasar el nombre con el que esta mapeado el metodo dentro de la lista "functions"

CEGUI::Window*  GUIManager::CreateButton(std::string stateWnd, std::string buttonName, std::string buttonScheme, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName)
{
	if (mButtons[buttonName] == nullptr) {
		CEGUI::Window *temp = wmgr->createWindow(buttonScheme, buttonName);

		if (stateWnd != "null")
			stateWnds[stateWnd]->addChild(temp);
		else
			myRoot->addChild(temp);

		temp->setPosition(CEGUI::UVector2(CEGUI::UDim(pos_x, 0.0f), CEGUI::UDim(pos_y, 0.0f)));
		temp->setSize(CEGUI::USize(CEGUI::UDim(size_x, 0), CEGUI::UDim(size_y, 0)));
		temp->setText(text);

		//temp->setAlwaysOnTop(true);

		if (methodName != "null")
			temp->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions[methodName], this));

		mButtons[buttonName] = temp; // Se añade al diccionario

	}
	return mButtons[buttonName];
}

CEGUI::Window * GUIManager::CreateLifeIcon(std::string buttonName, float pos_x, float pos_y, float size_x, float size_y)
{
	if (mButtons[buttonName] == nullptr) {
		CEGUI::Window *temp = wmgr->createWindow("TaharezLook/HeartButton", buttonName);

		myRoot->addChild(temp);

		temp->setPosition(CEGUI::UVector2(CEGUI::UDim(pos_x, 0.0f), CEGUI::UDim(pos_y, 0.0f)));
		temp->setSize(CEGUI::USize(CEGUI::UDim(size_x, 0), CEGUI::UDim(size_y, 0)));		

		mButtons[buttonName] = temp; // Se añade al diccionario

	}
	return mButtons[buttonName];
}

CEGUI::FrameWindow * GUIManager::AddWindow(std::string wndName, std::string frameWindowLook, float posX, float posY, float sizeX, float sizeY, std::string backgroundMatName){
	
	if (stateWnds[wndName] == nullptr) {	// Si esa ventana no se ha creado ya
		stateWnds[wndName] = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow(frameWindowLook, wndName));	//
		myRoot->addChild(stateWnds[wndName]);
		stateWnds[wndName]->setPosition(CEGUI::UVector2(CEGUI::UDim(posX, posX), CEGUI::UDim(posY, posY)));
		stateWnds[wndName]->setSize(CEGUI::USize(CEGUI::UDim(sizeX, sizeX), CEGUI::UDim(sizeY, sizeY)));
		stateWnds[wndName]->setTitleBarEnabled(false);
		stateWnds[wndName]->setCloseButtonEnabled(false);
		stateWnds[wndName]->setSizingEnabled(false);


		// Si el material está definido
		/*if (backgroundMatName != " ") {
			FrameWndImage(backgroundMatName);
		}*/
		mWindows[wndName] = stateWnds[wndName]; // Se añade al diccionario
	}

	return stateWnds[wndName];
}

// Metodo para poder utilizar una imagen como fondo de una ventana de menu
void GUIManager::FrameWndImage(std::string name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real down)
{
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name, "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);

	rect->setCorners(left, top, right, down);
	rect->setMaterial(material);

	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	Ogre::SceneNode* node = g_->getRoot()->createSceneManager()->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(rect);
}


void GUIManager::Exit()
{
	if (g_ != nullptr) {
		gameEnded = true;
		g_->Quit();
	}
}

void GUIManager::InitMainScene()
{
	if (g_ != nullptr) {
		gameHUD = true;
		menuHUD = false;
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		g_->ReLoadScene("mainScene");		
	}
}

void GUIManager::VolumeUp()
{
	AudioManager::getInstance()->modifyVolume(true);
}

void GUIManager::VolumeDown()
{
	AudioManager::getInstance()->modifyVolume(false);
}

void GUIManager::Mute()
{
	AudioManager::getInstance()->muteVolume();
}

void GUIManager::SensitivityUp()
{
	g_->getActiveScene()->getGameObject("Player")->getComponent<PlayerController>()->modifySensitivity(true);
	//cout << "sensibilidad subida" << endl;
}

void GUIManager::SensitivityDown()
{
	g_->getActiveScene()->getGameObject("Player")->getComponent<PlayerController>()->modifySensitivity(false);
	//cout << "sensibilidad bajada" << endl;
}

void GUIManager::ToggleWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {

		if (stateWnds[wndName] == activeWnd) {	//Si es la que está activa, lo que queremos es apagarla y poner la default (el HUD del juego)
			activeWnd->hide();
			activeWnd = lastWnd;
			activeWnd->show();
		}
		else {
			if (activeWnd != nullptr)
				activeWnd->hide();					//Apagamos la ventana que estuviera activa	(Activar segun parametro de si comienza activa o no)	
			lastWnd = activeWnd;					//Nos guardamos la ultima
			activeWnd = stateWnds[wndName];			//Ponemos la nueva como la activa
			activeWnd->show();						//Activamos la nueva
		}
	}
}

void GUIManager::HideWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {
		stateWnds[wndName]->hide();
	}
	else {
		std::cout << "No se encuentra la ventana en el diccionario" << std::endl;
	}
}

void GUIManager::ShowWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {
		stateWnds[wndName]->show();
	}
	else {
		std::cout << "No se encuentra la ventana en el diccionario" << std::endl;
	}
}

void GUIManager::toggleMenu()
{
	if (g_ != nullptr) {
		//ShowWindow("MenuWnd");
		HideWindow("PauseWnd");
		gameHUD = false;
		menuHUD = true;
		pauseHUD = false;
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
		
		g_->ChangeScene("menuScene");
	}
}

void GUIManager::togglePause()
{	
	if (pauseHUD) {
		HideWindow("PauseWnd");		
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	}
	else {
		ShowWindow("PauseWnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();		
	}

	gameHUD = !gameHUD;
	pauseHUD = !pauseHUD;
}