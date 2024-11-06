#include "AppManager.h"

#include <windowsx.h>
#include <memory>

AppManager::AppManager() {
	m_input = Input();
	hinstance = GetModuleHandle(NULL);
}

AppManager::~AppManager() {
	for (auto& app : m_apps) {
		app->Shutdown();
		CleanupAppWindow(app);
	}
}

AppManager& AppManager::Singleton() {
	static std::unique_ptr<AppManager> inst = std::unique_ptr<AppManager>(new AppManager);
	return *inst;
}

void AppManager::AddApp(std::unique_ptr<IApp>&& app) {
	auto& inst = Singleton();

	WNDCLASSEX wc;
	int posX, posY;

	LPCWSTR applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst.hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	int screenWidth, screenHeight;

	screenWidth = 1200;
	screenHeight = 675;

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
		posX, posY, screenWidth, screenHeight, NULL, NULL, inst.hinstance, NULL);
	
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	RECT rect;
	GetClientRect(hwnd, &rect);

	app->Setup(applicationName, hwnd, rect.right, rect.bottom, &inst.m_input);
	app->Init();

	inst.m_apps.insert(std::move(app));
}

void AppManager::CleanupAppWindow(const std::unique_ptr<IApp>& app) {
	// Remove the window.
	DestroyWindow(app->hwnd);

	// Remove the application instance.
	UnregisterClass(app->m_appName, hinstance);
}

void AppManager::OnWindowResized(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	for (auto& app : m_apps) {
		if (app->hwnd == hwnd) {
			int newWidth = LOWORD(lParam);
			int newHeight = HIWORD(lParam);
			if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) {
				app->SetDimensions(newWidth, newHeight);
			}
			break;
		}
	}
}

void AppManager::MainLoop() {
	MSG msg;
	bool done;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
		}
		for (auto& app : m_apps) {
			if (!app->Frame()) {
				done = true;
			}
		}

	}
	PostQuitMessage(0);
	return;
}

LRESULT CALLBACK AppManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK AppManager::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	auto& inst = Singleton();
	auto& input = inst.m_input;
	switch (umessage) {
	case WM_KEYDOWN:
	{
		input.SetKeyDown(wparam);
		break;
	}
	case WM_KEYUP:
	{
		input.SetKeyUp(wparam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		input.setMouseX(GET_X_LPARAM(lparam));
		input.setMouseY(GET_Y_LPARAM(lparam));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		input.setLeftMouse(true);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		input.setRightMouse(true);
		break;
	}
	case WM_LBUTTONUP:
	{
		input.setLeftMouse(false);
		break;
	}
	case WM_RBUTTONUP:
	{
		input.setRightMouse(false);
		break;
	}
	case WM_SIZE:
	{
		inst.OnWindowResized(hwnd, wparam, lparam);
		break;
	}
	// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	}
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	return inst.MessageHandler(hwnd, umessage, wparam, lparam);
}