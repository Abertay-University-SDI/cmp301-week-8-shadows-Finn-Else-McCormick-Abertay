#pragma once

#include <windows.h>
#include "Input.h"
#include "IApp.h"
#include <set>
#include <memory>

#define WIN32_LEAN_AND_MEAN

class AppManager
{
private:
	AppManager();
public:
	~AppManager();

	static AppManager& Singleton();

	static void AddApp(std::unique_ptr<IApp>&&);

	void MainLoop();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	HINSTANCE hinstance;
	Input m_input;
	std::set<std::unique_ptr<IApp>> m_apps;
	void CleanupAppWindow(const std::unique_ptr<IApp>&);

	void OnWindowResized(HWND, WPARAM, LPARAM);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};