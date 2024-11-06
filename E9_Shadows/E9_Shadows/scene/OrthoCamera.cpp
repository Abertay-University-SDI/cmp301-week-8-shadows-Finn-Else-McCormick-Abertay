#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float width, float height, float nearPlane, float farPlane, bool invertUp) : m_invertUp(invertUp), Camera(width, height, nearPlane, farPlane) {
	RecalculateProjectionMatrix();
	// Have to call this again cause the virtual call in the base class constructor doesn't call our version cause C++ is a busted-ass language
}

XMVECTOR OrthoCamera::CameraUpVector() const {
	XMFLOAT3 up = XMFLOAT3(0.f, m_invertUp ? -1.f : 1.f, 0.f);
	return XMLoadFloat3(&up);
}

XMMATRIX OrthoCamera::__Internal_CalculateProjectionMatrix() const {
	return XMMatrixOrthographicLH(m_width, m_height, m_near, m_far);
}