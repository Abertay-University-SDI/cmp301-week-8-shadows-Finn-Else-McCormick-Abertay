#pragma once

#include <scene/Camera.h>
#include "input.h"

class CameraController : public Camera
{
public:
	CameraController(Input* in, HWND wnd, int width, int height, float nearPlane, float farPlane, float fieldOfView = XM_PI / 4.f);

	void Update(float dt);

private:
	Input* input;
	float m_moveSpeed = 5.f;
	float m_sensitivity = 0.2f;
	POINT cursor;
	HWND wnd;

	int m_windowWindth, m_windowHeight;
};