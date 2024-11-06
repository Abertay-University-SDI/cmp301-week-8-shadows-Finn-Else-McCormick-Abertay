#pragma once

#include <windows.h>
#include "Input.h"

class IApp {
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual bool Frame() = 0;

	virtual void OnResized();

private:
	void Setup(LPCWSTR appName, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	friend class AppManager;

	LPCWSTR m_appName;
	void SetDimensions(int screenWidth, int screenHeight);

protected:
	HWND hwnd;
	int m_width, m_height;
	Input* p_input;
};