#include "GUILoader.h"

GUILoader::~GUILoader()
{
}

void GUILoader::LoadFromFile(json obj)
{
#ifdef C_DEBUG
	cout << "Empieza a cargar el GUI" << endl;
#endif

	if (!obj["windows"].empty()) {
#ifdef C_DEBUG
		cout << "Existen ventanas a cargar" << endl << endl;
#endif
		for (auto &win : obj["windows"])	// Recorremos los botones
			if (win.is_object()) {

				string frameWndName = win["frameWndName"];
				string WndLook = win["frameWndLook"];
				float WndPosX = win["wndPosX"];
				float WndPosY = win["wndPosY"];
				float WndSizeX = win["wndSizeX"];
				float WndSizeY = win["wndSizeY"];

				string backgroundMatName = win["backgroundMat"];

				bool active = win["active"];

				CEGUI::FrameWindow* wnd = GUIManager::Instance()->AddWindow(frameWndName, WndLook, WndPosX, WndPosY, WndSizeX, WndSizeY, backgroundMatName);

				Windows[frameWndName] = wnd;
				WindowsStates[frameWndName] = active;

				wnd->hide();
			}
	}

	if (!obj["buttons"].empty()) {
#ifdef C_DEBUG
		cout << "Existen botones a cargar" << endl << endl;
#endif // C_DEBUG
		for (auto &but : obj["buttons"])	// Recorremos los botones
			if (but.is_object()) {				
				
				string frameWndName = but["frameWndParentName"];
				string buttonName = but["buttonName"];
				string buttonScheme = but["buttonScheme"];
				float buttonPosX = but["buttonPosX"];
				float buttonPosY = but["buttonPosY"];
				float buttonSizeX = but["buttonSizeX"];
				float buttonSizeY = but["buttonSizeY"];
				string buttonText = but["buttonText"];
				string buttonFunctionName = but["buttonFunctionName"];
				bool active = but["active"];
				bool interactable = but["interactable"];

				CEGUI::Window * button = GUIManager::Instance()->CreateButton(frameWndName, buttonName, buttonScheme, buttonPosX, buttonPosY, buttonSizeX, buttonSizeY, buttonText, buttonFunctionName);
				if (!interactable) {
					button->disable();
				}

				Buttons[buttonName] = button;
				ButtonsStates[buttonName] = active;
			}
	}
}

void GUILoader::Start()
{
	auto itWnd = Windows.begin();
	auto itWndStates = WindowsStates.begin();

	while (itWnd != Windows.end()) {

		if (itWndStates->second)
			itWnd->second->show();
		else
			itWnd->second->hide();		

		itWndStates++;
		itWnd++;
	}

	auto itBut = Buttons.begin();
	auto itButStates = ButtonsStates.begin();

	while (itBut != Buttons.end()) {

		if (itButStates->second)
			itBut->second->show();
		else
			itBut->second->hide();

		itButStates++;
		itBut++;
	}
}

void GUILoader::OnEnable()
{
	auto itWnd = Windows.begin();
	auto itWndStates = WindowsStates.begin();

	while (itWnd != Windows.end()) {

		if (itWndStates->second)
			itWnd->second->show();
		else
			itWnd->second->hide();

		itWndStates++;
		itWnd++;
	}

	auto itBut = Buttons.begin();
	auto itButStates = ButtonsStates.begin();

	while (itBut != Buttons.end()) {

		if (itButStates->second)
			itBut->second->show();
		else
			itBut->second->hide();

		itButStates++;
		itBut++;
	}
}

void GUILoader::OnDisable()
{
	auto itWnd = Windows.begin();
	auto itWndStates = WindowsStates.begin();

	while (itWnd != Windows.end()) {

		itWnd->second->hide();

		itWndStates++;
		itWnd++;
	}

	auto itBut = Buttons.begin();
	auto itButStates = ButtonsStates.begin();

	while (itBut != Buttons.end()) {

		itBut->second->hide();

		itButStates++;
		itBut++;
	}
}
