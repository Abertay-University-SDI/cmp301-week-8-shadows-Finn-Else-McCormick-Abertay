#include "CameraController.h"

#include <imgui/imgui.h>

CameraController::CameraController(Input* in, HWND wnd, int width, int height, float nearPlane, float farPlane, float fieldOfView)
	: input(in), wnd(wnd), cursor(POINT()), m_windowWindth(width), m_windowHeight(height), Camera(width, height, nearPlane, farPlane, fieldOfView) {}

void CameraController::Update(float deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();

	if (input->isMouseActive())
	{
		// mouse look is on
		float turnSpeed = m_sensitivity * deltaTime;
		SetYaw(Yaw() + turnSpeed * (input->getMouseX() - (m_windowWindth / 2)));
		SetPitch(Pitch() + turnSpeed * (input->getMouseY() - (m_windowHeight / 2)));

		cursor.x = m_windowWindth / 2;
		cursor.y = m_windowHeight / 2;
		ClientToScreen(wnd, &cursor);
		SetCursorPos(cursor.x, cursor.y);

		if (input->isKeyDown(VK_ESCAPE) || input->isRightMouseDown()) {
			// disable mouse tracking and show mouse cursor
			input->setMouseActive(false);
			ShowCursor(true);
		}
	}
	else {
		if (input->isLeftMouseDown() && !io.WantCaptureMouse) {
			// re-position cursor
			cursor.x = m_windowWindth / 2;
			cursor.y = m_windowHeight / 2;
			ClientToScreen(wnd, &cursor);
			SetCursorPos(cursor.x, cursor.y);
			input->setMouseX(m_windowWindth / 2);
			input->setMouseY(m_windowHeight / 2);

			// set mouse tracking as active and hide mouse cursor
			input->setMouseActive(true);
			ShowCursor(false);
		}
	}

	XMFLOAT3 moveDir = XMFLOAT3();
	XMFLOAT3 moveDirUpDown = XMFLOAT3();
	if (!io.WantCaptureKeyboard) {
		moveDir.z = ((float)input->isKeyDown('W')) - ((float)input->isKeyDown('S'));
		moveDir.x = ((float)input->isKeyDown('D')) - ((float)input->isKeyDown('A'));
		moveDirUpDown.y = ((float)input->isKeyDown(VK_SPACE)) - ((float)input->isKeyDown(VK_SHIFT));
	}
	XMVECTOR moveVec = XMLoadFloat3(&moveDir);
	XMVECTOR moveVecUpDown = XMLoadFloat3(&moveDirUpDown);
	moveVec = XMVector3Transform(moveVec, RotationMatrix());
	moveVec = moveVec + moveVecUpDown;
	moveVec = XMVectorScale(moveVec, m_moveSpeed * deltaTime);

	XMFLOAT3 pos = Position();
	XMVECTOR posVec = XMLoadFloat3(&pos);
	posVec = XMVectorAdd(posVec, moveVec);
	XMStoreFloat3(&pos, posVec);
	SetPosition(pos);

	RecalculateViewMatrix();
}