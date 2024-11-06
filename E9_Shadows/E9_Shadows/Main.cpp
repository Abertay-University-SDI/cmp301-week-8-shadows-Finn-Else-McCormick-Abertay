// Main.cpp
#include <system/AppManager.h>
#include <AppMain.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	auto& appManager = AppManager::Singleton();
	appManager.AddApp(std::make_unique<AppMain>());

	appManager.MainLoop();

	return 0;
}