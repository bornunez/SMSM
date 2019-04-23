#include "GUIManager.h"

#include "Game.h"

GUIManager* GUIManager::instance_ = nullptr;

GUIManager::GUIManager(Ogre::RenderWindow* w, Game* g)
{
	renderWindow_ = w;

	// Registramos las funciones de los botones con un nombre distintivo en un diccionario de funciones
	functions["exit"] = &GUIManager::Exit;
	functions["pause"] = &GUIManager::togglePause;
	functions["menu"] = &GUIManager::toggleMenu;
}

GUIManager::~GUIManager()
{
}

GUIManager * GUIManager::Instance(Ogre::RenderWindow* w, Game* g)
{
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
	else if (InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Left)) {
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
	if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_ESCAPE)) {		//ESCAPE es el unico boton de teclado que queremos reconocer en los menus (para entrar o salir de la pausa)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);
	}
	else if (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_ESCAPE)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);
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

	// Todo esto debe cargarse desde un archivo que modificamos desde un archivo


	////Main menu window
	//{
	//	menuWnd = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
	//	myRoot->addChild(menuWnd);
	//	menuWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.2f, -0.2f), CEGUI::UDim(-0.2f, -0.2f)));
	//	menuWnd->setSize(CEGUI::USize(CEGUI::UDim(1.3f, 1.3f), CEGUI::UDim(1.3f, 1.3f)));
	//	menuWnd->setTitleBarEnabled(false);
	//	menuWnd->setCloseButtonEnabled(false);

	//	//Main menu buttons
	//	{
	//		CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	//		menuWnd->addChild(quit);
	//		quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
	//		quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	//		quit->setText("Quit");
	//		quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::Exit, this));

	//		CEGUI::Window *start = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	//		menuWnd->addChild(start);
	//		start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	//		start->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	//		start->setText("Start");
	//		start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::toggleMenu, this));
	//	}
	//}
}



void GUIManager::Exit()
{
	g_->Quit();
	CEGUI::OgreRenderer::destroySystem();
}

void GUIManager::ToggleWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {

		if (stateWnds[wndName] == activeWnd) {	//Si es la que est� activa, lo que queremos es apagarla y poner la default (el HUD del juego)
			activeWnd->hide();
			activeWnd = lastWnd;
			activeWnd->show();
		}
		else {
			if(activeWnd!=nullptr)
				activeWnd->hide();					//Apagamos la ventana que estuviera activa	(Activar segun parametro de si comienza activa o no)	
			lastWnd = activeWnd;					//Nos guardamos la ultima
			activeWnd = stateWnds[wndName];			//Ponemos la nueva como la activa
			activeWnd->show();						//Activamos la nueva
		}
	}
}

void GUIManager::AddWindow(std::string wndName)
{
	if (stateWnds[wndName] == nullptr) {	// Si esa ventana no se ha creado ya
		stateWnds[wndName] = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow("TaharezLook/FrameWindow", "testWindow"));
		myRoot->addChild(stateWnds[wndName]);
		stateWnds[wndName]->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.2f, -0.2f), CEGUI::UDim(-0.2f, -0.2f)));
		stateWnds[wndName]->setSize(CEGUI::USize(CEGUI::UDim(1.3f, 1.3f), CEGUI::UDim(1.3f, 1.3f)));
		stateWnds[wndName]->setTitleBarEnabled(false);
		stateWnds[wndName]->setCloseButtonEnabled(false);
	}
}

// NOTA:

// El problema con la asignacion de los callbacks de los botones es la asignacion de parametros
// Es decir se puede hacer con diccionario de metodos sin parametros que llamen por debajo a metodos con
// parametros espeficicos para ese metodo

// Hay que pasar el nombre con el que esta mapeado el metodo dentro de la lista "functions"

void GUIManager::CreateButton(std::string stateWnd ,std::string buttonScheme, std::string buttonImage, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName)
{
	CEGUI::Window *temp = wmgr->createWindow(buttonScheme, buttonImage);
	stateWnds[stateWnd]->addChild(temp);
	temp->setPosition(CEGUI::UVector2(CEGUI::UDim(pos_x, 0.0f), CEGUI::UDim(pos_y, 0.0f)));
	temp->setSize(CEGUI::USize(CEGUI::UDim(size_x, 0), CEGUI::UDim(size_y, 0)));
	temp->setText(text);
	temp->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions[methodName], this));
}

void GUIManager::toggleMenu()
{
	ToggleWindow("menu");
}

void GUIManager::togglePause()
{
	ToggleWindow("pause");
}