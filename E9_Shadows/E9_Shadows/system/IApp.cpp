#include "IApp.h"

void IApp::Setup(LPCWSTR appName, HWND wnd, int screenWidth, int screenHeight, Input* in) {
	m_appName = appName;
	hwnd = wnd;
	m_width = screenWidth;
	m_height = screenHeight;
	p_input = in;
}

void IApp::SetDimensions(int screenWidth, int screenHeight) {
	m_width = screenWidth;
	m_height = screenHeight;
	OnResized();
}

void IApp::OnResized() {}